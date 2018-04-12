#pragma once

#include <phosphor-logging/log.hpp>
#include "callback.hpp"
#include "event_manager.hpp"
#include <sstream>

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

/** @class EventBase
 *  @brief Event callback implementation.
 *
 *  The event callback creates the event dbus object
 *  which has event message and metadata as key value pairs
 *  as specified by the client supplied property index.
 */
class EventBase : public IndexedCallback
{
  public:
    EventBase() = delete;
    EventBase(const EventBase&) = delete;
    EventBase(EventBase&&) = default;
    EventBase& operator=(const EventBase&) = delete;
    EventBase& operator=(EventBase&&) = default;
    virtual ~EventBase() = default;
    EventBase(const PropertyIndex& index) : IndexedCallback(index)
    {
    }

    /** @brief Callback interface implementation. */
    void operator()(Context ctx) override
    {
        if (ctx == Context::START)
        {
            // No action should be taken
            // as this call back is being called from
            // daemon Startup.
            return;
        }

        for (const auto& n : index)
        {
            const auto& path = std::get<pathIndex>(n.first);
            const auto& propertyMeta = std::get<propertyIndex>(n.first);
            const auto& storage = std::get<storageIndex>(n.second);
            const auto& value = std::get<valueIndex>(storage.get());

            if (!value.empty())
            {
                createEvent(path, propertyMeta, value);
            }
        }
    }

  private:
    /** @brief Create the event Dbus Object.
     *  @param[in] path - Dbus Object Path for which the
     *                    property has changed.
     *  @param[in] property - Name of the property whose value
     *                        has been changed.
     *  @param[in] value - Changed property value.
     */
    virtual void createEvent(const std::string& path,
                             const std::string& property,
                             const any_ns::any& value) const = 0;
};

/** @class Event
 *  @brief C++ type specific logic for the event callback.
 *
 *  @tparam T - The C++ type of the property values being traced.
 */
template <typename T> class Event : public EventBase
{
  public:
    Event() = delete;
    Event(const Event&) = delete;
    Event(Event&&) = default;
    Event& operator=(const Event&) = delete;
    Event& operator=(Event&&) = default;
    ~Event() = default;

    /** @brief Constructor.
     *  @param[in] eventName - Name of the event.
     *  @param[in] eventMessage- Event Message.
     *  @param[in] index - look up index for the properties.
     */
    Event(std::string eventName, std::string eventMessage,
          const PropertyIndex& index) :
        EventBase(index),
        name(eventName), message(eventMessage)
    {
    }

  private:
    /** @brief Create the event Dbus Object.
     *  @param[in] path - Dbus Object Path for which the
     *                    property has changed.
     *  @param[in] property - Name of the property whose value
     *                        has been changed.
     *  @param[in] value - Changed property value.
     */
    void createEvent(const std::string& path, const std::string& property,
                     const any_ns::any& value) const override
    {
        std::stringstream ss{};
        ss << any_ns::any_cast<T>(value);
        phosphor::events::getManager().create(name, message, path, property,
                                              ss.str());
    }

    /** @brief Event Name */
    std::string name;

    /** @brief Event Message */
    std::string message;
};

/** @class SNMPEvent
 *  @brief SNMP event callback implementation.
 *
 *  The SNMP event callback raises a SNMP Trap for the event
 *  which has event message and metadata as key value pairs
 *  as specified by the client supplied object path.
 *  The SNMP event is invoked when new interfaces are
 *  added to the object path specified by the client
 */
class SNMPEvent : public Callback
{
  public:
    SNMPEvent() = delete;
    SNMPEvent(const SNMPEvent&) = delete;
    SNMPEvent(SNMPEvent&&) = default;
    SNMPEvent& operator=(const SNMPEvent&) = delete;
    SNMPEvent& operator=(SNMPEvent&&) = default;
    virtual ~SNMPEvent() = default;
    SNMPEvent(std::string iType) :
        Callback(), type(iType)
    {
    }

    /** @brief Callback interface implementation. */
    void operator()(Context ctx) override;

    /** @brief Callback interface implementation. */
    void operator()(Context ctx, sdbusplus::message::message& msg) override;

  private:
    std::string type;
};

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
