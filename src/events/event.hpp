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
        /** @brief Delegate type specific calls to subclasses. */
        virtual void createEvent(
            const std::string& path,
            const std::string& property,
            const any_ns::any& value) const = 0;


};


/** @class Event
 *  @brief C++ type specific logic for the event  callback.
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
        Event(std::string type, const PropertyIndex& index) :
            EventBase(index), eventType(type) {}

    private:
        /** @brief log interface implementation. */
        void createEvent(
            const std::string& path,
            const std::string& property,
            const any_ns::any& value) const override;

       std::string eventType;
};

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
