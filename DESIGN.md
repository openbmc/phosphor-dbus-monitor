# phosphor-dbus-monitor (PDM) — Requirements & Design Document

> OpenBMC Project · phosphor-dbus-monitor
> Apache License 2.0

---

## Table of Contents

1. [Overview & Purpose](#1-overview--purpose)
2. [Requirements](#2-requirements)
3. [System Architecture](#3-system-architecture)
4. [Code-Generation Pipeline](#4-code-generation-pipeline)
5. [Class Hierarchy](#5-class-hierarchy)
6. [Runtime Data-Flow](#6-runtime-data-flow)
7. [Watch Subsystem](#7-watch-subsystem)
8. [Callback Subsystem](#8-callback-subsystem)
9. [Condition Subsystem](#9-condition-subsystem)
10. [Event Manager](#10-event-manager)
11. [Startup Sequence](#11-startup-sequence)
12. [Signal-Triggered Sequence](#12-signal-triggered-sequence)
13. [YAML Configuration Reference](#13-yaml-configuration-reference)
14. [Cross-Service Dependencies](#14-cross-service-dependencies)
15. [Who Uses PDM — Consumers & Producers](#15-who-uses-pdm--consumers--producers)
16. [File Inventory](#16-file-inventory)

---

## 1. Overview & Purpose

**phosphor-dbus-monitor** (PDM) is an OpenBMC daemon that **declaratively watches
D-Bus object properties and paths, evaluates configurable conditions against them,
and fires configurable actions (callbacks) when those conditions are met**.

It is a _data-driven_ system: operators describe _what_ to watch and _what to do_
entirely in YAML files. A build-time Python code-generator
([`pdmgen.py`](src/pdmgen.py)) translates the YAML into a C++ source file
(`generated.hpp`), which is compiled into the binary. **The binary itself has no
hard-coded policy.**

> **Key insight:** PDM separates _policy_ (YAML) from _mechanism_ (C++ runtime).
> Adding a new monitoring rule requires only a YAML change — no C++ must be
> modified.

PDM is **not** a debug-only tool. It is a production firmware component used by
platform vendors (IBM Power, Ampere, and others) to wire together fault detection,
error logging, SNMP notification, service restarts, and inventory management
entirely through configuration, without writing new daemons.

---

## 2. Requirements

### 2.1 Functional Requirements

| ID    | Requirement                                                             | Satisfied by                            |
| ----- | ----------------------------------------------------------------------- | --------------------------------------- |
| FR-1  | Monitor one or more D-Bus **property values** and cache their state     | `PropertyWatch` / `PropertyWatchOfType` |
| FR-2  | Monitor D-Bus **object path** existence (InterfacesAdded/Removed)       | `PathWatch`                             |
| FR-3  | Allow boolean **conditions** to gate callbacks (count, median)          | `CountCondition`, `MedianCondition`     |
| FR-4  | Support **deferred** (timer-based) callback execution                   | `DeferrableCallback`                    |
| FR-5  | Support **one-shot** conditions (fire once until condition clears)      | `oneshot` flag in conditions            |
| FR-6  | **Filter** property values before they enter the cache                  | `Filters` / `OperandFilters`            |
| FR-7  | Write messages to the **systemd journal** on trigger                    | `Journal` callback                      |
| FR-8  | Log **phosphor error-log** entries (elog) on trigger                    | `Elog`, `ElogWithMetadataCapture`       |
| FR-9  | Create **D-Bus Event** objects on trigger                               | `Event` callback + `EventManager`       |
| FR-10 | Invoke an arbitrary **D-Bus method** on trigger                         | `Method`, `PathMethod`                  |
| FR-11 | **Resolve** (clear) error-log callouts on trigger                       | `ResolveCallout`                        |
| FR-12 | Send **SNMP traps** on trigger                                          | `SNMPTrap` / `ErrorTrap`                |
| FR-13 | **Group** multiple callbacks and fire them together                     | `GroupOfCallbacks`                      |
| FR-14 | **Persist** Event objects across daemon restarts                        | `EventManager::restore()`               |
| FR-15 | Re-evaluate all configured rules **at startup** (initial state check)  | `Context::START` in `main()`            |

### 2.2 Non-Functional Requirements

- **Configurability** — All monitoring policy expressed in YAML; zero C++ changes
  required for new rules.
- **Type safety** — Property types resolved at compile time via C++ templates.
- **Minimal footprint** — Single daemon, event-loop driven (`sdeventplus`), no
  polling.
- **Testability** — DBus interface abstracted via template parameter
  (`DBusInterfaceType`); mock injection used in unit tests.
- **Extensibility** — New callback types or condition types require only a new C++
  class and a new Mako template; the YAML schema and code-generator extend
  naturally.

---

## 3. System Architecture

```text
┌──────────────────────────────────────┐
│             BUILD TIME               │
│                                      │
│  *.yaml ──► pdmgen.py                │
│             + Mako templates         │
│             ──► generated.hpp        │
└──────────────────┬───────────────────┘
                   │ #includes
                   ▼
┌──────────────────────────────────────────────────────────────────┐
│                          RUNTIME                                 │
│                                                                  │
│  main()                                                          │
│   ├─ EventManager::restore()          (re-hydrate persisted DB)  │
│   ├─ PropertyWatch::start()  ×N                                  │
│   │    ├─ ObjectMapper::GetObject()   (find bus names)           │
│   │    ├─ Properties::GetAll()        (prime property cache)     │
│   │    └─ register PropertiesChanged + InterfacesAdded matches   │
│   ├─ PropertyWatch::callback(START)  ×N                          │
│   │    └─ evaluate conditions with cached values                 │
│   ├─ PathWatch::start() + callback(START)  ×M                    │
│   ├─ bus.request_name("xyz.openbmc_project.Events")              │
│   └─ sdeventplus event.loop()  ◄─── all further work is here    │
│                                                                  │
│  On PropertiesChanged / InterfacesAdded signal:                  │
│   parse ──► filter ──► cache ──► condition ──► callback(s)       │
└──────────────────────────────────────────────────────────────────┘
```

**D-Bus name registered:** `xyz.openbmc_project.Events`
**Object manager root:** `/xyz/openbmc_project/events`
**Persist path:** `/var/lib/phosphor-dbus-monitor/events`
**Max events per name:** 20 (capped; oldest evicted)

---

## 4. Code-Generation Pipeline

```text
*.yaml ──► pdmgen.py ──► Mako templates ──► generated.hpp ──► C++ compiler ──► pdm binary
           (parse +        (render C++)      (static arrays
            build IR)                         of Watch/Callback instances)
```

`pdmgen.py` performs **three phases** for each YAML file:

1. **Factory phase** — instantiates Python objects for each YAML directive; calls
   `factory()` to expand derived objects (e.g., an `Instance` per path × property
   cross-product).
2. **Setup phase** — resolves symbolic names to integer array indices that the
   templates use as C++ array references.
3. **Render phase** — passes the resolved object graph to Mako templates, emitting
   C++ initialiser lists for watches, callbacks, conditions, and filters.

The `YAML_PATH` meson option points the generator at the platform's YAML directory.
If unset, the built-in `src/example/` directory is used (produces a binary that
compiles but does nothing useful in production).

### 4.1 pdmgen.py Python Class Map

| YAML class / subclass        | Python class in pdmgen.py | Mako template                |
| ---------------------------- | ------------------------- | ---------------------------- |
| `group: path`                | `GroupOfPaths`            | (inline)                     |
| `group: property`            | `GroupOfProperties`       | (inline)                     |
| `watch: property`            | `PropertyWatch`           | `generated.hpp.mako`         |
| `pathwatch: path`            | `PathWatch`               | `generated.hpp.mako`         |
| `callback: journal`          | `Journal`                 | `journal.cpp.mako`           |
| `callback: elog`             | `Elog`                    | `elog.cpp.mako`              |
| `callback: elog_with_metadata` | `ElogWithMetadata`      | `elog_with_metadata.cpp.mako` |
| `callback: event`            | `Event`                   | `event.cpp.mako`             |
| `callback: method`           | `Method`                  | `method.cpp.mako`            |
| `pathcallback: method`       | `PathMethod`              | `pathmethod.cpp.mako`        |
| `callback: resolve callout`  | `ResolveCallout`          | `resolve_errors.cpp.mako`    |
| `callback: group`            | `GroupOfCallbacks`        | `callbackgroup.cpp.mako`     |
| `pathcallback: eventpath`    | `EventPath`               | `eventpath.cpp.mako`         |
| `condition: count`           | `CountCondition`          | `count.cpp.mako`             |
| `condition: median`          | `MedianCondition`         | `median.cpp.mako`            |

---

## 5. Class Hierarchy

### 5.1 Watch Hierarchy

```text
Watch  (abstract)                               [src/watch.hpp]
  + start() = 0
  + callback(Context) = 0
  + callback(Context, message_t&) {}
  │
  ├── PropertyWatch<DBusInterfaceType>          [src/propertywatch.hpp]
  │     - index: PropertyIndex&
  │     - cb: Callback*
  │     - alreadyRan: bool
  │     - ignoreStartCallback: bool
  │     + start()            queries ObjectMapper, primes cache, registers matches
  │     + callback(ctx)      invokes cb if alreadyRan && cb present
  │     + updateProperties() = 0
  │     + propertiesChanged() = 0
  │     + interfacesAdded()  = 0
  │     │
  │     └── PropertyWatchOfType<T, DBusInterfaceType>
  │           - filterOps: Filters*
  │           + updateProperties()   calls Properties.GetAll; feeds propertiesChanged()
  │           + propertiesChanged()  applies filter, updates cache, calls callback(SIGNAL)
  │           + interfacesAdded()    delegates to propertiesChanged()
  │
  └── PathWatch<DBusInterfaceType>              [src/pathwatch.hpp]
        - paths: RefVector<const string>&
        - cb: Callback&
        - alreadyRan: bool
        - event: ObjectManagerEvent
        - match_object_path: bool
        + start()            registers InterfacesAdded or InterfacesRemoved match
        + callback(ctx)      forwards to cb(ctx)
        + callback(ctx,msg)  forwards to cb(ctx,msg)
```

### 5.2 Callback Hierarchy

```text
Callback  (abstract)                            [src/callback.hpp]
  + operator()(Context) = 0
  + operator()(Context, message_t&) {}
  │
  ├── IndexedCallback                           holds PropertyIndex&
  │     + operator()(Context) = 0
  │     │
  │     ├── JournalBase                         [src/journal.hpp]
  │     │     + operator()(Context)   iterates index, calls log() for each value
  │     │     └── Journal<T, Severity>
  │     │           + log()  phosphor::logging::log<Severity>(...)
  │     │
  │     ├── ElogBase                            [src/elog.hpp]
  │     │     + operator()(Context)   calls log() only on SIGNAL
  │     │     └── Elog<T, Args...>
  │     │           + log()  phosphor::logging::report<T>(args...)
  │     │
  │     ├── EventBase                           [src/event.hpp]
  │     │     + operator()(Context)   iterates index, calls createEvent() on SIGNAL
  │     │     └── Event<T>
  │     │           + createEvent()   EventManager::create(name,msg,path,prop,val)
  │     │
  │     └── ElogWithMetadataCapture<E,M,P>      [src/elog.hpp]
  │           + operator()(Context)   builds path:prop=val string; reports<E>(M(str))
  │
  ├── MethodBase                                [src/method.hpp]
  │     └── Method<DBusInterface, Args...>
  │           + operator()(Context)   std::apply → callMethodNoReply(bus,path,iface,method)
  │           └── PathMethod<DBusInterface, Args...>
  │                 + operator()(Context)      only fires on SIGNAL
  │                 + operator()(Context,msg)  delegates to operator()(ctx)
  │
  ├── ResolveCallout                            [src/resolve_errors.hpp]
  │     - callout: string
  │     + operator()(Context)  reads <callout>/fault endpoints; sets Resolved=true
  │
  ├── SNMPTrap<T>                               [src/snmp_trap.hpp]
  │     - event: T  (e.g. ErrorTrap)
  │     + operator()(Context)       no-op
  │     + operator()(Context,msg)   event.trap(msg)
  │         └── ErrorTrap::trap(msg)  unpacks Logging.Entry; sendTrap<OBMCErrorNotification>()
  │
  ├── GroupOfCallbacks<CallbackAccess>          [src/callback.hpp]
  │     - graph: vector<size_t>
  │     + operator()(Context)  fires each indexed callback in order
  │
  ├── ConditionalCallback<CallbackAccess>       [src/callback.hpp]
  │     - graph: vector<size_t>
  │     - condition: Conditional&
  │     + operator()(Context)  condition() == true → fires graph[0]
  │
  └── DeferrableCallback<CallbackAccess>        extends ConditionalCallback
        - delayInterval: microseconds
        - timer: unique_ptr<TimerType>
        - ctx: Context
        + operator()(Context)
            condition true  && timer idle    → start timer
            condition false                  → disable timer
            timer fires                      → ConditionalCallback::operator()(saved ctx)
```

### 5.3 Conditional Hierarchy

```text
Conditional  (abstract)                        [src/callback.hpp]
  + operator()(): bool = 0
  │
  └── IndexedConditional                       holds PropertyIndex&
        │
        ├── CountCondition<T>                  [src/count.hpp]
        │     - countOp:    function<bool(size_t)>
        │     - propertyOp: function<bool(T)>
        │     - oneshot: bool
        │     + operator()()
        │         pass1: count properties where propertyOp(value)==true
        │                store result in storage[resultIndex]
        │         pass2: return countOp(count)
        │         suppress if oneshot && lastResult==true
        │
        └── MedianCondition<T>                 [src/median.hpp]
              - medianOp: function<bool(T)>
              - oneshot: bool
              + operator()()
                  collect values → compute median
                  2 values  → max (worst-case)
                  N even    → average of middle two
                  N odd     → nth_element at N/2
                  default true if no values
```

### 5.4 Filter Hierarchy

```text
Filters  (abstract)                            [src/filters.hpp]
  + operator()(const any& value): bool  — true = FILTERED OUT
  │
  └── OperandFilters<T>
        - ops: vector<function<bool(T)>>
        + operator()(const any& value)
            for each op: if !op(any_cast<T>(value)) → return true (filtered)
            all pass → return false (accepted)
```

### 5.5 Supporting Types

| Class / Type          | File                    | Role                                                                           |
| --------------------- | ----------------------- | ------------------------------------------------------------------------------ |
| `SDBusPlus`           | `src/sdbusplus.hpp`     | Static singleton: `getBus()`, `addMatch()`, `callMethodAndRead()`, `getBusName()` |
| `PropertyIndex`       | `src/data_types.hpp`    | `TupleRefMap<(path,iface,prop) → (pathMeta,propMeta,storage)>` — central cache |
| `Context`             | `src/data_types.hpp`    | Enum: `START` \| `SIGNAL`                                                      |
| `EventManager`        | `src/event_manager.hpp` | Creates, caps (max 20), and restores D-Bus Event entry objects                 |
| `Entry`               | `src/event_entry.hpp`   | D-Bus object implementing `xyz.openbmc_project.Logging.Event`                  |
| `ConfigPropertyWatches` | `generated.hpp`       | Static accessor for the array of `PropertyWatch` instances                     |
| `ConfigPathWatches`   | `generated.hpp`         | Static accessor for the array of `PathWatch` instances                         |

---

## 6. Runtime Data-Flow

### 6.1 Property Watch Flow

```text
D-Bus PropertiesChanged signal
    │
    ▼
sdbusplus match lambda  [registered per (path, interface) pair in PropertyWatch::start()]
    │
    ▼
PropertyWatchOfType<T>::propertiesChanged(msg, path, iface)
    │  unpack PropertiesChanged<T> from message
    │
    ▼  for each property in message:
    ├── lookup (path, iface, prop) in PropertyIndex
    │    └── not in index → skip
    │
    └── in index:
         │
         ├─ filterOps present?
         │   ├── YES: (*filterOps)(value)
         │   │    ├── filtered  → storage[valueIndex].reset()  stop here
         │   │    └── not filtered → continue
         │   └── NO: continue
         │
         └─ storage[valueIndex] = value     ← cache updated
              │
              ▼
         PropertyWatch::callback(Context::SIGNAL)
              ▼
         (*cb)(Context::SIGNAL)
              │
              ├─ GroupOfCallbacks   → fires each indexed callback
              ├─ ConditionalCallback
              │    └── condition() → true?  → (*target)(SIGNAL)
              └─ DeferrableCallback
                   ├── condition() true  + timer idle    → start countdown
                   ├── condition() true  + timer running → nothing
                   ├── condition() false                 → cancel timer
                   └── timer fires → ConditionalCallback(saved ctx) → leaf callback
```

### 6.2 Path Watch Flow

```text
D-Bus InterfacesAdded / InterfacesRemoved signal
    │
    ▼
sdbusplus match lambda  [registered per path in PathWatch::start()]
    │
    ▼
PathWatch::callback(Context::SIGNAL, msg)
    │
    ▼
(*cb)(Context::SIGNAL, msg)
    │
    ├── EventPath / SNMPTrap<ErrorTrap> → ErrorTrap::trap(msg) → sendTrap<OBMCErrorNotification>()
    └── PathMethod → callMethodNoReply(bus, path, iface, method)
```

### 6.3 Storage Layout (PropertyIndex)

```text
PropertyIndex key:   tuple<ref<path>, ref<interface>, ref<property>>
PropertyIndex value: tuple<ref<pathMeta>, ref<propMeta>, ref<storage>>

storage = tuple<any, any>
          [0] valueIndex  — current cached property value (std::any, empty = not present)
          [1] resultIndex — last condition pass/fail (used by ElogWithMetadataCapture)
```

---

## 7. Watch Subsystem

| Class | D-Bus signal(s) | Startup behaviour | Signal behaviour |
| ----- | --------------- | ----------------- | ---------------- |
| `PropertyWatch<T>` | `PropertiesChanged`, `InterfacesAdded` | Queries ObjectMapper; calls `Properties.GetAll` to prime every watched (path, interface) pair; registers one `PropertiesChanged` + one `InterfacesAdded` match per pair | Calls `propertiesChanged()` or `interfacesAdded()` which updates cache then calls `callback(SIGNAL)` |
| `PropertyWatchOfType<T,I>` | same | same (concrete type `T`) | Applies `OperandFilters`, updates `storage[valueIndex]`, fires callback |
| `PathWatch<T>` | `InterfacesAdded` or `InterfacesRemoved` | Registers one match per path; can match by sender path or object path | Calls `cb(SIGNAL, msg)` — no cache involved |

> **Idempotency:** Both watch types guard `start()` with `alreadyRan` so repeated calls are safe.
> **Late arrivals:** If a path doesn't exist at startup (ObjectMapper returns empty), the `InterfacesAdded` match handles the property when it appears later.

---

## 8. Callback Subsystem

| Class | Action | Fires on |
| ----- | ------ | -------- |
| `Journal<T, Severity>` | Writes structured message to systemd journal with path + property key-value metadata for every index entry that has a cached value | START and SIGNAL |
| `Elog<T, Args...>` | Emits a phosphor error log entry (`phosphor::logging::report<T>`) with typed, statically-declared metadata | SIGNAL only |
| `ElogWithMetadataCapture<E,M,P>` | Emits an error log; builds `path:prop=val` metadata string from condition result flags | SIGNAL only |
| `Event<T>` | Calls `EventManager::create()` — new `Entry` D-Bus object; persisted to disk | SIGNAL only |
| `Method<I, Args...>` | Calls `SDBusPlus::callMethodNoReply()` — fire-and-forget; logs error on exception | START and SIGNAL |
| `PathMethod<I, Args...>` | Same as Method but gated on `Context::SIGNAL` only; handles `(ctx, msg)` overload | SIGNAL only |
| `ResolveCallout` | Reads `/fault` association from `<callout>` path; iterates endpoints; sets `Resolved=true` on each `xyz.openbmc_project.Logging.Entry` | START and SIGNAL |
| `SNMPTrap<ErrorTrap>` | Parses `InterfacesAdded` message for `Logging.Entry` fields; calls `sendTrap<OBMCErrorNotification>()` via phosphor-snmp | SIGNAL only (msg overload) |
| `GroupOfCallbacks<A>` | Iterates a list of callback indices and calls each one | Delegates |
| `ConditionalCallback<A>` | Evaluates `Conditional::operator()()`; fires `graph[0]` only if true | Delegates |
| `DeferrableCallback<A>` | On first true condition starts a monotonic timer; on false cancels it; on expiry fires the conditional callback with saved context | Delegates + timer |

---

## 9. Condition Subsystem

### 9.1 CountCondition\<T\>

Performs a two-pass evaluation over the `PropertyIndex`:

```text
Pass 1: for each (path, iface, prop) in index:
          if storage[valueIndex] is empty → skip
          result = propertyOp(any_cast<T>(storage[valueIndex]))
          storage[resultIndex] = result    ← used by ElogWithMetadataCapture
          count += result

Pass 2: return countOp(count)

Oneshot guard:
  if oneshot && result==true && lastResult==true → return false
  lastResult = result
```

**Example:** "fire if at least 3 of 4 temperature sensors exceed 115 °C"

```yaml
op: ">="
bound: 115
countop: ">="
countbound: 3
oneshot: true
```

### 9.2 MedianCondition\<T\>

```text
Collect all non-empty storage[valueIndex] values into vector<T>

Compute median:
  0 values → default result = true (no data → pass)
  1 value  → median = values[0]
  2 values → median = max(values)          # worst-case for 2
  N odd    → nth_element at N/2
  N even   → average of nth_element(N/2) and nth_element(N/2 - 1)

return medianOp(median)
Oneshot guard: same as CountCondition
```

**Example:** "fire if median fan speed falls below 1000 RPM"

```yaml
op: "<"
bound: 1000
oneshot: true
```

### 9.3 Oneshot Behaviour

When `oneshot: true`:

```text
First evaluation:  condition → true  → callback fires  → lastResult = true
Second evaluation: condition → true  → suppressed (return false)
Any evaluation:    condition → false → lastResult = false  (rearmed)
Next true:         condition → true  → callback fires again
```

This prevents flooding callbacks for a persistent fault.

### 9.4 Deferred Conditions

The `defer` YAML field wraps any condition in a `DeferrableCallback`:

```yaml
defer: 5000000us # 5 seconds
```

```text
condition true  + timer idle    → start countdown
condition false + timer running → cancel countdown
timer fires                     → evaluate condition once more
                                  → if still true: fire target callback
```

---

## 10. Event Manager

`phosphor::events::Manager` maintains an in-memory `map<EventName, queue<Entry>>`.

| Aspect | Detail |
| ------ | ------ |
| D-Bus path pattern | `/xyz/openbmc_project/events/<name>/<id>` |
| D-Bus interface | `xyz.openbmc_project.Logging.Event` |
| Bus name | `xyz.openbmc_project.Events` |
| Object manager | Registered at `/xyz/openbmc_project/events` via `sdbusplus::server::manager_t` |
| Capacity | Max **20** entries per event name; when full the oldest entry is evicted and its persisted file removed |
| Persistence | Cereal-serialised to `/var/lib/phosphor-dbus-monitor/events/<name>/<timestamp>` |
| Restore | `Manager::restore()` called at startup re-creates D-Bus objects from persisted files |

**Entry fields** (`xyz.openbmc_project.Logging.Event`):

| Field | Content |
| ----- | ------- |
| `Timestamp` | Milliseconds since epoch at time of creation |
| `Message` | Event message string from YAML `eventMessage` |
| `AdditionalData` | `["path=<objectPath>", "<propertyName>=<value>"]` |

---

## 11. Startup Sequence

```text
main()
  │
  ├─1─► EventManager::restore()
  │         └─ reads /var/lib/phosphor-dbus-monitor/events/**
  │            deserializes Cereal archives → re-creates Entry D-Bus objects
  │            → emit_object_added() on each restored entry
  │
  ├─2─► for each PropertyWatch in ConfigPropertyWatches::get():
  │       watch->start()
  │         ├─ already ran? → skip
  │         ├─ convert PropertyIndex to nested map<path, map<iface, props>>
  │         ├─ for each path:
  │         │    addMatch(InterfacesAdded(path), λ → interfacesAdded(msg))
  │         │    GetObject(path) via ObjectMapper
  │         │    for each interface:
  │         │      addMatch(PropertiesChanged(path,iface), λ → propertiesChanged(msg,path,iface))
  │         │      if interface in mapper response:
  │         │        updateProperties(busName, path, iface)   ← primes cache
  │         └─ alreadyRan = true
  │
  ├─3─► for each PropertyWatch: watch->callback(Context::START)
  │         └─ condition evaluated against primed cache
  │            → callback fires if condition passes at startup
  │
  ├─4─► for each PathWatch: watch->start()
  │         └─ addMatch(InterfacesAdded|Removed per path)
  │
  ├─5─► for each PathWatch: watch->callback(Context::START)
  │
  ├─6─► bus.request_name("xyz.openbmc_project.Events")
  │
  └─7─► event.loop()
          ← blocking; all further work is signal-driven via sdeventplus
```

---

## 12. Signal-Triggered Sequence

### 12.1 PropertyWatch (PropertiesChanged)

```text
D-Bus daemon emits: org.freedesktop.DBus.Properties.PropertiesChanged
    │  (path=/xyz/…, interface=xyz.openbmc_project.Sensor.Value)
    ▼
sdbusplus match lambda  [keyed to (path, interface)]
    ▼
PropertyWatchOfType<T>::propertiesChanged(msg, path, iface)
    │  msg.unpack<PropertiesChanged<T>>()  → map<propName, variant<T>>
    │
    ▼  for each (propName, value) in message:
    ├── not in PropertyIndex → skip
    └── in index:
          ├─ filterOps != nullptr?
          │    YES → (*filterOps)(value)
          │          → true  (filtered): storage[valueIndex].reset()  ← stop
          │          → false (accepted): fall through
          │    NO  → fall through
          └─ storage[valueIndex] = value    ← cache updated
                ▼
           PropertyWatch::callback(Context::SIGNAL)
                ▼
           (*cb)(Context::SIGNAL)          ← wired by generated.hpp
                │
                ├─ GroupOfCallbacks        → each member(SIGNAL)
                ├─ ConditionalCallback
                │    condition()? YES → (*target)(SIGNAL)
                └─ DeferrableCallback
                     YES + timer idle   → restartOnce(delayInterval)
                     YES + timer active → nothing
                     NO                → setEnabled(false)
                     timer expires     → leaf: Journal/Elog/Event/Method/ResolveCallout
```

### 12.2 PathWatch (InterfacesAdded)

```text
D-Bus daemon emits: org.freedesktop.DBus.ObjectManager.InterfacesAdded
    │  (object path = /xyz/openbmc_project/logging)
    ▼
sdbusplus match lambda  [keyed to path or sender path]
    ▼
PathWatch::callback(Context::SIGNAL, msg)
    ▼
(*cb)(Context::SIGNAL, msg)
    │
    ├─ EventPath / SNMPTrap<ErrorTrap>
    │    └── ErrorTrap::trap(msg)
    │          unpack object_path + PathInterfacesAdded
    │          find "xyz.openbmc_project.Logging.Entry"
    │          extract: Id, Timestamp, Severity, Message, AdditionalData
    │          sendTrap<OBMCErrorNotification>(errorID, timestamp, sev, msg)
    │
    └─ PathMethod → callMethodNoReply(bus, path, iface, method, args...)
```

---

## 13. YAML Configuration Reference

### 13.1 Directive Types

| Directive (class)             | Sub-type         | Purpose                                        | Key fields                                                       |
| ----------------------------- | ---------------- | ---------------------------------------------- | ---------------------------------------------------------------- |
| `group: path`                 | pathgroup        | Named list of D-Bus paths with metadata labels | `members[].path`, `members[].meta`                               |
| `group: property`             | propertygroup    | Named list of properties (same D-Bus type)     | `type`, `members[].interface`, `members[].property`, `members[].meta` |
| `watch: property`             | PropertyWatch    | Cache and react to property changes            | `paths`, `properties`, `callback`, `filters`, `ignore_start_callback` |
| `pathwatch: path`             | PathWatch        | React to InterfacesAdded / Removed on path(s)  | `paths`, `pathcallback`, `event`, `match_object_path`            |
| `callback: journal`           | Journal          | Log message to systemd journal                 | `severity`, `message`, `paths`, `properties`                     |
| `callback: elog`              | Elog             | Create phosphor error log                      | `error`, `metadata[]`                                            |
| `callback: elog_with_metadata` | ElogWithMetadata | Error log with captured property values       | `error`, `metadata` (single string field)                        |
| `callback: event`             | Event            | Create D-Bus event object (persisted)          | `eventName`, `eventMessage`                                      |
| `callback: method`            | Method           | Invoke D-Bus method                            | `service`, `path`, `interface`, `method`, `args[]`               |
| `callback: resolve callout`   | ResolveCallout   | Resolve all error logs for inventory callout   | `callout`                                                        |
| `callback: group`             | GroupOfCallbacks | Invoke multiple callbacks together             | `members[]`                                                      |
| `pathcallback: eventpath`     | EventPath/SNMPTrap | Send SNMP trap on path event                 | `eventType`                                                      |
| `condition: count`            | CountCondition   | Count properties matching op; test count vs countbound | `op`, `bound`, `countop`, `countbound`, `oneshot`, `defer` |
| `condition: median`           | MedianCondition  | Test median of property values vs bound        | `op`, `bound`, `oneshot`, `defer`                                |

### 13.2 Filter Configuration

Filters are specified inline on a `watch: property` directive. All filters are
AND-ed. A value failing _any_ filter is cleared from the cache (not stored) and
the callback is not triggered for that update.

```yaml
filters:
  - op: ">="
    bound: 0
  - op: "<="
    bound: 100
```

### 13.3 Supported Property Types

`byte`, `uint16`, `uint32`, `uint64`, `int16`, `int32`, `int64`, `string`,
`boolean`, `double`

### 13.4 Supported Severity Levels (Journal)

`DEBUG`, `INFO`, `NOTICE`, `WARNING`, `ERR`, `CRIT`, `ALERT`, `EMERG`

### 13.5 Supported Relational Operators (Conditions / Filters)

`<`, `<=`, `>`, `>=`, `==`, `!=`

### 13.6 Complete Working Example

```yaml
# 1. Path group
- name: fan paths
  class: group
  group: path
  members:
    - meta: FAN
      path: /xyz/openbmc_project/sensors/fan_tach/fan0
    - meta: FAN
      path: /xyz/openbmc_project/sensors/fan_tach/fan1

# 2. Property group
- name: fan speed props
  class: group
  group: property
  type: int64
  members:
    - interface: xyz.openbmc_project.Sensor.Value
      meta: SPEED
      property: Value

# 3. Leaf callback — journal
- name: fan stall journal
  class: callback
  callback: journal
  paths: fan paths
  properties: fan speed props
  severity: ERR
  message: Fan stall detected

# 4. Leaf callback — elog
- name: fan stall elog
  class: callback
  callback: elog
  paths: fan paths
  properties: fan speed props
  error: xyz::openbmc_project::Sensor::Error::SensorThresholdCriticalLowGoingLow

# 5. Callback group
- name: fan stall actions
  class: callback
  callback: group
  members:
    - fan stall journal
    - fan stall elog

# 6. Condition
- name: fan stall condition
  class: condition
  condition: count
  paths: fan paths
  properties: fan speed props
  callback: fan stall actions
  op: "<"
  bound: 100
  countop: ">="
  countbound: 1
  oneshot: true
  defer: 5000000us

# 7. Watch
- name: fan speed watch
  class: watch
  watch: property
  paths: fan paths
  properties: fan speed props
  callback: fan stall condition
  filters:
    - op: ">="
      bound: 0
  ignore_start_callback: false
```

---

## 14. Cross-Service Dependencies

PDM is neither a debug tool nor an isolated service. It actively **reads from** and
**drives** other OpenBMC services.

### 14.1 Services PDM reads from (watches)

| D-Bus service | What PDM reads | Typical use |
| ------------- | -------------- | ----------- |
| `phosphor-hwmon`, `dbus-sensors` | `xyz.openbmc_project.Sensor.Value::Value` | Thermal / fan / voltage threshold monitoring |
| `phosphor-inventory-manager` | `xyz.openbmc_project.Inventory.Item::Present` | Hardware presence detection |
| `phosphor-logging` | `xyz.openbmc_project.Logging.Entry` (via `InterfacesAdded`) | SNMP trap on new error log creation |
| Any service | Any watched property | Generic platform policy via YAML |

### 14.2 Services PDM drives (callbacks)

| Target service | Callback type | What PDM does |
| -------------- | ------------- | ------------- |
| **phosphor-logging** | `Elog`, `ElogWithMetadataCapture` | Creates new error log entries with typed metadata when conditions are met |
| **phosphor-logging** | `ResolveCallout` | Resolves existing error log entries — sets `Resolved=true` on entries linked via the `<callout>/fault` association |
| **systemd journal** | `Journal` | Writes structured log messages with property key-value metadata at any severity |
| **phosphor-snmp** | `SNMPTrap<ErrorTrap>` | Bridges phosphor-logging to SNMP: parses new `Logging.Entry` and calls `sendTrap<OBMCErrorNotification>()` |
| **Any D-Bus service** | `Method`, `PathMethod` | Invokes arbitrary methods — e.g., `systemd1.Manager::StartUnit` to restart a service on fault |
| **PDM itself** | `Event` via `EventManager` | Publishes `xyz.openbmc_project.Logging.Event` objects under `xyz.openbmc_project.Events` |
| **bmcweb / Redfish** | (reads EventManager output) | Redfish event log endpoints read the Event objects PDM publishes |

---

## 15. Who Uses PDM — Consumers & Producers

### 15.1 PDM is not a debug tool

PDM sits on the **critical path** of BMC fault management. It is the glue daemon
that converts raw sensor/inventory state changes into platform-level actions. On
IBM Power systems and other OpenBMC platforms it is always running in production.

### 15.2 YAML config producers (who writes the YAML)

Platform vendors ship PDM YAML configuration files as part of their machine layer
(e.g., `meta-ibm`, `meta-ampere`, `meta-facebook` layers in Yocto). These files
are specific to each hardware platform and encode the platform's fault policy:

- Which sensors trigger which error codes
- Which FRU present/absent transitions resolve which error logs
- Which conditions trigger SNMP traps
- Which service to restart on which fault

The `YAML_PATH` meson build option tells the code-generator where to find these
platform YAML files at build time.

### 15.3 D-Bus consumers of PDM's output

| Consumer | What it reads | Where it reads from |
| -------- | ------------- | ------------------- |
| **bmcweb** | Event objects | `xyz.openbmc_project.Events` bus, `/xyz/openbmc_project/events/**` |
| **Redfish event log** | `Timestamp`, `Message`, `AdditionalData` | Same D-Bus path tree |
| **SNMP managers** | OBMCErrorNotification traps | Network — sent by `phosphor-snmp` on PDM's behalf |
| **phosphor-logging** | Error log entries created by `Elog` callbacks | Own D-Bus interface; stored in `/var/lib/phosphor-logging/errors/` |
| **Any D-Bus service** | `Resolved=true` property change on `Logging.Entry` | Set by `ResolveCallout` callback |

### 15.4 Relationship to other monitoring daemons

| Daemon | Relationship to PDM |
| ------ | ------------------- |
| `phosphor-hwmon` / `dbus-sensors` | **Producer** — publishes sensor values PDM watches |
| `phosphor-fan-presence` | **Producer** — publishes fan presence inventory properties PDM can watch |
| `phosphor-logging` | **Producer and Consumer** — PDM creates error logs in it; PDM also watches it for new entries to SNMP-trap |
| `phosphor-snmp` | **Consumer** — PDM calls its library to send traps |
| `systemd` | **Consumer** — PDM can call `StartUnit`/`StopUnit` via Method callback |
| `bmcweb` | **Consumer** — reads Event objects PDM publishes |
| `phosphor-inventory-manager` | **Producer** — publishes `Present` property PDM watches for `ResolveCallout` use cases |

### 15.5 End-to-End Example: Fan Stall → Error Log → SNMP Trap → Resolution

```text
[hardware fault]
     │
     ▼
dbus-sensors publishes:
  /xyz/openbmc_project/sensors/fan_tach/fan0
  xyz.openbmc_project.Sensor.Value::Value = 0
     │
     ▼  PropertiesChanged signal
PDM PropertyWatch primes cache
PDM CountCondition: Value < 100, count >= 1  →  TRUE
PDM DeferrableCallback: wait 5 s
     │  condition still true after 5 s
     ▼
PDM GroupOfCallbacks fires:
  ├── Journal callback → writes to systemd journal
  └── Elog callback → phosphor-logging creates error log entry
           /xyz/openbmc_project/logging/entry/42
           Severity = Critical, CalloutInventoryPath = /xyz/.../fan0
     │
     ▼  InterfacesAdded signal on /xyz/openbmc_project/logging
PDM PathWatch triggers:
  └── SNMPTrap<ErrorTrap> → sendTrap<OBMCErrorNotification>(42, ts, Critical, msg)
           → SNMP manager receives trap on the network
     │
     ▼  [fan replaced, hardware recovers]
dbus-sensors publishes Value = 3000
     │
     ▼  PropertiesChanged signal
PDM CountCondition: Value >= 100  → FALSE
PDM inventory watch: Present = true on fan0
     │
     ▼
PDM ResolveCallout callback:
  reads /xyz/.../fan0/fault endpoints → [/xyz/.../logging/entry/42]
  sets Resolved = true on entry/42
     │
     ▼
bmcweb / Redfish sees entry/42 Resolved = true → removes from active alert list
```

---

## 16. File Inventory

| File | Role |
| ---- | ---- |
| `src/main.cpp` | Entry point; iterates watches, starts event loop |
| `src/watch.hpp` | Abstract `Watch` base class |
| `src/propertywatch.hpp` | `PropertyWatch<T>` and `PropertyWatchOfType<T,I>` declarations |
| `src/propertywatch.cpp` | Link unit; actual impl in `propertywatchimpl.hpp` |
| `src/propertywatchimpl.hpp` | Template implementation: start / callback / updateProperties / propertiesChanged / interfacesAdded |
| `src/pathwatch.hpp` | `PathWatch<T>` declaration |
| `src/pathwatch.cpp` | Link unit |
| `src/pathwatchimpl.hpp` | Template implementation of `PathWatch` start / callback |
| `src/callback.hpp` | Abstract `Callback`, `Conditional`, `IndexedCallback`, `IndexedConditional`, `GroupOfCallbacks`, `ConditionalCallback`, `DeferrableCallback` |
| `src/filters.hpp` | `Filters` abstract + `OperandFilters<T>` |
| `src/count.hpp` | `CountCondition<T>` |
| `src/median.hpp` | `MedianCondition<T>` |
| `src/journal.hpp` | `JournalBase` + `Journal<T, Severity>` |
| `src/journal.cpp` | `JournalBase::operator()` — iterates index, calls virtual `log()` |
| `src/elog.hpp` | `ElogBase`, `Elog<T,...>`, `ElogWithMetadataCapture<E,M,P>` |
| `src/elog.cpp` | `ElogBase::operator()` — SIGNAL guard, delegates to `log()` |
| `src/event.hpp` | `EventBase` + `Event<T>` |
| `src/event_entry.hpp` | `Entry` — D-Bus object wrapping `xyz.openbmc_project.Logging.Event` |
| `src/event_manager.hpp` | `Manager` — creates, queues (max 20), and restores event objects |
| `src/event_manager.cpp` | `Manager::create()` and `Manager::restore()` implementations |
| `src/event_serialize.hpp` | `serialize()` / `deserialize()` declarations (Cereal) |
| `src/event_serialize.cpp` | Cereal-based persistence to `/var/lib/phosphor-dbus-monitor/events/` |
| `src/method.hpp` | `MethodBase`, `Method<I,...>`, `PathMethod<I,...>` |
| `src/resolve_errors.hpp` | `ResolveCallout` declaration |
| `src/resolve_errors.cpp` | Reads `<callout>/fault` association; sets `Resolved=true` on log entries |
| `src/snmp_trap.hpp` | `Trap`, `ErrorTrap`, `SNMPTrap<T>` |
| `src/snmp_trap.cpp` | `ErrorTrap::trap()` — unpacks message, calls `sendTrap<OBMCErrorNotification>()` |
| `src/sdbusplus.hpp` | `SDBusPlus` — static wrapper around sdbusplus bus, method calls, match registration |
| `src/data_types.hpp` | Type aliases: `PropertyIndex`, `Context`, `Value`, `RefKeyMap`, `TupleRefMap`, constants |
| `src/tupleref.hpp` | `TupleOfRefs` and `TupleOfRefsLess` comparator for map keys |
| `src/format.hpp` | Journal format string helpers (`GetFormat<T>`) |
| `src/pdmgen.py` | YAML parser + C++ code generator (build-time only) |
| `src/templates/generated.hpp.mako` | Top-level template; emits static arrays of watches/callbacks/conditions |
| `src/templates/journal.cpp.mako` | Emits `Journal<T, Severity>` constructor call |
| `src/templates/elog.cpp.mako` | Emits `Elog<T, Args...>` constructor call |
| `src/templates/elog_with_metadata.cpp.mako` | Emits `ElogWithMetadataCapture<E,M,P>` |
| `src/templates/event.cpp.mako` | Emits `Event<T>` constructor call |
| `src/templates/eventpath.cpp.mako` | Emits `SNMPTrap<ErrorTrap>` constructor call |
| `src/templates/method.cpp.mako` | Emits `Method<I,Args...>` constructor call |
| `src/templates/pathmethod.cpp.mako` | Emits `PathMethod<I,Args...>` constructor call |
| `src/templates/resolve_errors.cpp.mako` | Emits `ResolveCallout` constructor call |
| `src/templates/callbackgroup.cpp.mako` | Emits `GroupOfCallbacks<A>` constructor call |
| `src/templates/callbackpathgroup.cpp.mako` | Emits path callback group constructor |
| `src/templates/count.cpp.mako` | Emits `CountCondition<T>` constructor call |
| `src/templates/median.cpp.mako` | Emits `MedianCondition<T>` constructor call |
| `src/templates/conditional.cpp.mako` | Emits `ConditionalCallback` / `DeferrableCallback` constructor call |
| `src/templates/errors.hpp.mako` | Emits `#include` directives for elog error headers |
| `src/example/example.yaml` | Reference YAML — documents every directive type with comments |
| `src/example/test.yaml` | Build-validation YAML — exercises every type and operator combination |
| `src/test/` | Unit tests (gtest) for watch, callback, and codegen correctness |
| `meson.build` | Top-level build; defines `BUSNAME_EVENT`, `OBJ_EVENT`, `EVENTS_PERSIST_PATH`, `MAX_EVENTS` |
| `meson.options` | Build options: `YAML_PATH`, `tests`, `snmp` |

---

_Document generated from source analysis of `phosphor-dbus-monitor` commit tree._
