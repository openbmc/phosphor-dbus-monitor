## This file is a template, the comment below is emitted into the generated file
/* This is an auto generated file. Do not edit. */
#pragma once

#include <array>
#include <chrono>
#include <string>
#include "count.hpp"
#include "data_types.hpp"
#include "journal.hpp"
#include "method.hpp"
#include "propertywatchimpl.hpp"
#include "sdbusplus.hpp"
#include "sdevent.hpp"

using namespace std::string_literals;
using namespace std::chrono_literals;

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

struct ConfigMeta
{
    using Meta = std::array<std::string, ${len(meta)}>;

    static auto& get()
    {
        static const Meta meta =
        {
% for m in meta:
            "${m.name}"s,
% endfor
        };
        return meta;
    }
};

struct ConfigPaths
{
    using Paths = std::array<std::string, ${len(paths)}>;

    static auto& get()
    {
        static const Paths paths =
        {
% for p in paths:
            "${p.name}"s,
% endfor
        };
        return paths;
    }
};

struct ConfigInterfaces
{
    using Interfaces = std::array<std::string, ${len(interfaces)}>;

    static auto& get()
    {
        static const Interfaces interfaces =
        {
% for i in interfaces:
            "${i.name}"s,
% endfor
        };
        return interfaces;
    }
};

struct ConfigProperties
{
    using Properties = std::array<std::string, ${len(propertynames)}>;

    static auto& get()
    {
        static const Properties properties =
        {
% for p in propertynames:
            "${p.name}"s,
% endfor
        };
        return properties;
    }
};

struct ConfigPropertyStorage
{
    using Storage = std::array<any_ns::any, ${len(instances)}>;

    static auto& get()
    {
        static Storage storage;
        return storage;
    }
};

struct ConfigPropertyIndicies
{
    using PropertyIndicies = std::array<PropertyIndex, ${len(instancegroups)}>;

    static auto& get()
    {
        static const PropertyIndicies propertyIndicies =
        {
            {
% for g in instancegroups:
                {
    % for i in g.members:
                    {
                        PropertyIndex::key_type
                        {
                            std::cref(ConfigPaths::get()[${i[0]}]),
                            std::cref(ConfigInterfaces::get()[${i[2]}]),
                            std::cref(ConfigProperties::get()[${i[3]}])
                        },
                        PropertyIndex::mapped_type
                        {
                            std::cref(ConfigMeta::get()[${i[1]}]),
                            std::cref(ConfigMeta::get()[${i[4]}]),
                            std::ref(ConfigPropertyStorage::get()[${i[5]}])
                        },
                    },
    % endfor
                },
% endfor
            }
        };
        return propertyIndicies;
    }
};

struct ConfigPropertyCallbackGroups
{
    using CallbackGroups = std::array<std::vector<size_t>, ${len(callbackgroups)}>;
    static auto& get()
    {
        static const CallbackGroups propertyCallbackGraph =
        {
            {
% for g in callbackgroups:
                {${', '.join([str(x) for x in g.members])}},
% endfor
            }
        };
        return propertyCallbackGraph;
    }
};

struct ConfigConditions
{
    using Conditions = std::array<std::unique_ptr<Conditional>, ${len(conditions)}>;

    static auto& get()
    {
        static const Conditions propertyConditions =
        {
% for c in conditions:
            ${c.construct(loader, indent=indent +3)},
% endfor
        };
        return propertyConditions;
    }
};

struct ConfigPropertyCallbacks
{
    using Callbacks = std::array<std::unique_ptr<Callback>, ${len(callbacks)}>;

    static auto& get()
    {
        static const Callbacks propertyCallbacks =
        {
% for c in callbacks:
            ${c.construct(loader, indent=indent +3)},
% endfor
        };
        return propertyCallbacks;
    }
};

struct ConfigPropertyWatches
{
    using PropertyWatches = std::array<std::unique_ptr<Watch>, ${len(watches)}>;

    static auto& get()
    {
        static const PropertyWatches propertyWatches =
        {
% for w in watches:
            std::make_unique<PropertyWatchOfType<${w.datatype}, SDBusPlus>>(
    % if w.callback is None:
                ConfigPropertyIndicies::get()[${w.instances}]),
    % else:
                ConfigPropertyIndicies::get()[${w.instances}],
                *ConfigPropertyCallbacks::get()[${w.callback}]),
    % endif
% endfor
        };
        return propertyWatches;
    }
};
} // namespace monitoring
} // namespace dbus
} // namespace phosphor
