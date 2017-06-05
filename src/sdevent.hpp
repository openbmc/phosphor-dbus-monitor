#pragma once

#include "sdevent/event.hpp"
#include "sdevent/timer.hpp"

struct Loop;

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

/** @class SDEvent
 *  @brief SDEventType access delegate implementation for sdevent.
 */
class SDEvent
{
    protected:
        /** @brief Share a single event loop amongst users. */
        static auto& getEvent()
        {
            static auto event = sdevent::event::newDefault();
            return event;
        }

    public:
        /** @brief Wrapper for sd_event_now. */
        static auto now()
        {
            return getEvent().now();
        }

        friend Loop;
};

/** @class SDEventTimer
 *  @brief TimerType access delegate implementation for sdevent.
 */
class SDEventTimer : public SDEvent
{
    public:
        SDEventTimer() = delete;
        SDEventTimer(const SDEventTimer&) = default;
        SDEventTimer(SDEventTimer&&) = default;
        SDEventTimer& operator=(const SDEventTimer&) = default;
        SDEventTimer& operator=(SDEventTimer&&) = default;
        ~SDEventTimer() = default;

        explicit SDEventTimer(
            const sdevent::event::timer::Timer::Callback& callback)
            : timer(getEvent(), callback) {}

        /** @brief Update a timer expiration. */
        void update(
            const std::chrono::steady_clock::time_point& expires)
        {
            timer.setTime(expires);
        }

        /** @brief Query timer state. */
        auto enabled()
        {
            return timer.enabled() != SD_EVENT_OFF;
        }

        /** @brief Enable a timer. */
        void enable()
        {
            timer.enable(SD_EVENT_ONESHOT);
        }

        /** @brief Disable a timer. */
        void disable()
        {
            timer.enable(SD_EVENT_OFF);
        }

    private:
        sdevent::event::timer::Timer timer;
};

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
