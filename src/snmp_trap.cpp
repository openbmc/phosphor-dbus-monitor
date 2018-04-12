#include "snmp_trap.hpp"
namespace phosphor
{
namespace dbus
{
namespace monitoring
{
void ErrorTrap::trap(sdbusplus::message::message& msg) const
{
    // TODO openbmc/openbmc#3059
    // TODO: Send trap to SNMP to be added after phoshpor-snmp is merged
}
} // namespace monitoring
} // namespace dbus
} // namespace phosphor
