#pragma once

#include <sdbusplus/bus.hpp>
#include <sdbusplus/server.hpp>
#include "events.hpp"

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

class Monitor
{
    public:
        Monitor() = delete;
        Monitor(const Monitor&) = delete;
        Monitor(Monitor&&) = default;
        Monitor& operator=(const Monitor&) = delete;
        Monitor& operator=(Monitor&&) = default;
        ~Monitor() = default;

        explicit Monitor(sdbusplus::bus::bus& bus);

        void processStart() noexcept;

        void handleEvent(sdbusplus::message::message& msg,
                         const Event& event,
                         const std::tuple<std::vector<std::shared_ptr<Event>>,
                                    std::vector<Action>>& eventDef);

        using eventArg = std::tuple<Monitor*,
                                    const SignalEvent*,
                                    const std::tuple<
                                        std::vector<std::shared_ptr<Event>>,
                                        std::vector<Action>>*>;

    private:
        sdbusplus::bus::bus& bus;

        static const std::vector<
            std::tuple<std::vector<std::shared_ptr<Event>>,
                       std::vector<Action>>> events;

        std::vector<std::unique_ptr<eventArg>> eventArgs;

        std::vector<sdbusplus::server::match::match> matches;

        static int handleSignal(sd_bus_message* msg,
                                void* data,
                                sd_bus_error* err);

};

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
