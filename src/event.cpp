#include "event.hpp"
namespace phosphor
{
namespace dbus
{
namespace monitoring
{
void SNMPEvent::operator()(Context ctx)
{
}

void SNMPEvent::operator()(Context ctx, sdbusplus::message::message& msg)
{
    if (ctx == Context::SIGNAL)
    {
        // TODO: parsing and invoking SNMP will be taken in new patches
        // TODO openbmc/openbmc#3059
        // Send out SNMP traps corresponding to BMC error logs
    }
}
} // namespace monitoring
} // namespace dbus
} // namespace phosphor
