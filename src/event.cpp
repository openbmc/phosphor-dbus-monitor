#include "event.hpp"
#include "snmp_notification.hpp"
#include <xyz/openbmc_project/Logging/Entry/server.hpp>
namespace phosphor
{
namespace dbus
{
namespace monitoring
{
using namespace phosphor::snmp;
using namespace sdbusplus::xyz::openbmc_project::Logging::server;
static constexpr auto entry = "xyz.openbmc_project.Logging.Entry";

void SNMPEvent::operator()(Context ctx)
{
}

void SNMPEvent::operator()(Context ctx, sdbusplus::message::message& msg)
{
    if (ctx == Context::SIGNAL)
    {
        if (type == "ErrorNotification")
        {
            sdbusplus::message::object_path path;
            msg.read(path);
            PathInterfacesAdded intfs;
            msg.read(intfs);
            auto it = intfs.find(entry);
            if (it != intfs.end())
            {
                auto& propMap = it->second;
                uint32_t errorID = propMap.at("Id").get<uint32_t>();
                uint64_t timestamp = propMap.at("Timestamp").get<uint64_t>();
                std::string sev = propMap.at("Severity").get<std::string>();
                uint8_t isev = static_cast<uint8_t>(
                    Entry::convertLevelFromString(sev));
                std::string message = propMap.at("Message").get<std::string>();
                bool resolved = propMap.at("Resolved").get<bool>();
                sendTrap<ErrorNotification>(errorID, timestamp, isev,
                    message, resolved);
            }
        }
    }
}
} // namespace monitoring
} // namespace dbus
} // namespace phosphor
