#pragma once

#include <phosphor-logging/log.hpp>
#include "callback.hpp"

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

/** @class EventBase
 *  @brief Event callback implementation.
 *
 *  The event callback creates the event dbus object.
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
        EventBase(const PropertyIndex& index) :
            IndexedCallback(index) {}

        /** @brief Callback interface implementation. */
        void operator()() override;

    private:

        /** @brief Create the event Dbus Object.
         *  @param[in] path - Dbus Object Path for which the
         *                    property has changed.
         *  @param[in] property - Name of the property whose value
         *                        has been changed.
         *  @param[in] value - Changed property value.
         */
        virtual void createEvent(
            const std::string& path,
            const std::string& property,
            const any_ns::any& value) const = 0;


};


/** @class Event
 *  @brief C++ type specific logic for the event callback.
 */
class Event : public EventBase
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
        Event(std::string eventName,
              std::string eventMessage,
              const PropertyIndex& index) :
            EventBase(index),
            name(eventName),
            message(eventMessage) {}

    private:

        void createEvent(
            const std::string& path,
            const std::string& property,
            const any_ns::any& value) const override;

       std::string name;
       std::string message;
};

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
