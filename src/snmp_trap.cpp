#include "snmp_trap.hpp"
#include "snmp_notification.hpp"
#include <phosphor-logging/log.hpp>
#include <phosphor-logging/elog.hpp>
#include <phosphor-logging/elog-errors.hpp>
#include <xyz/openbmc_project/Logging/Entry/server.hpp>
#include <xyz/openbmc_project/Common/error.hpp>
namespace phosphor
{
namespace dbus
{
namespace monitoring
{
using namespace phosphor::logging;
using namespace sdbusplus::xyz::openbmc_project::Logging::server;
using namespace phosphor::snmp;
using InternalFailure =
    sdbusplus::xyz::openbmc_project::Common::Error::InternalFailure;

static constexpr auto entry = "xyz.openbmc_project.Logging.Entry";

void ErrorTrap::trap(sdbusplus::message::message& msg) const
{
    sdbusplus::message::object_path path;
    msg.read(path);
    PathInterfacesAdded intfs;
    msg.read(intfs);
    auto it = intfs.find(entry);
    if (it == intfs.end())
    {
        return;
    }
    auto& propMap = it->second;
    auto errorID = propMap.at("Id").get<uint32_t>();
    auto timestamp = propMap.at("Timestamp").get<uint64_t>();
    auto sev = propMap.at("Severity").get<std::string>();
    auto isev = static_cast<uint8_t>(Entry::convertLevelFromString(sev));
    auto message = propMap.at("Message").get<std::string>();
    auto resolved = propMap.at("Resolved").get<bool>();
    try
    {
        sendTrap<ErrorNotification>(errorID, timestamp, isev, message,
                                    resolved);
    }
    catch (const InternalFailure& e)
    {
        log<level::INFO>(
            "Failed to send SNMP trap",
            phosphor::logging::entry("ERROR_ID=%d", errorID),
            phosphor::logging::entry("TIMESTAMP=%llu", timestamp),
            phosphor::logging::entry("SEVERITY=%s", sev.c_str()),
            phosphor::logging::entry("MESSAGE=%s", message.c_str()),
            phosphor::logging::entry("RESOLVED=%d", resolved));
    }
}
} // namespace monitoring
} // namespace dbus
} // namespace phosphor
