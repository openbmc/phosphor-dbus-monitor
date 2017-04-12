#include <sdbusplus/bus.hpp>
#include "monitor.hpp"

int main(void)
{
    auto bus = sdbusplus::bus::new_default();

    phosphor::dbus::monitoring::Monitor monitor(bus);

    // Keep application running
    while (true)
    {
        bus.process_discard();
        bus.wait();
    }

    return -1;
}
