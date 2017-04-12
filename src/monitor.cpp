#include "monitor.hpp"

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

Monitor::Monitor(sdbusplus::bus::bus& bus) :
    bus(bus)
{

}

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
