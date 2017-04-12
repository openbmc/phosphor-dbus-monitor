#include "monitor.hpp"

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

// TODO Remove when generated.cpp included
const std::vector<std::tuple<std::vector<std::shared_ptr<Event>>,
                             std::vector<Action>>>
    Monitor::events
{};

Monitor::Monitor(sdbusplus::bus::bus& bus) :
    bus(bus)
{

}

void Monitor::processStart() noexcept
{
    sdbusplus::message::message nullMsg{nullptr};

    // Process thru given events that are type 'start'
    for (auto& event : events)
    {
        for (auto pEvent : std::get<std::vector<std::shared_ptr<Event>>>(event))
        {
            if (pEvent->trigger == Event::Trigger::START)
            {
                handleEvent(nullMsg, *pEvent, event);
            }
        }
    }
}

void Monitor::handleEvent(sdbusplus::message::message& msg,
                          const Event& event,
                          const std::tuple<std::vector<std::shared_ptr<Event>>,
                                           std::vector<Action>>& eventDef)
{
    // Iterate over conditions
    for (auto& cond : event)
    {
        if (!cond(bus, msg, *this))
        {
            continue;
        }
        // Perform defined actions
        for (auto& act : std::get<1>(eventDef))
        {
            act(bus, *this);
        }
        return;
    }
}

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
