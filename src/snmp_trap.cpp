#include "snmp_trap.hpp"

#include <phosphor-logging/elog-errors.hpp>
#include <phosphor-logging/elog.hpp>
#include <phosphor-logging/lg2.hpp>
#include <snmp.hpp>
#include <snmp_notification.hpp>
#include <xyz/openbmc_project/Common/error.hpp>
#include <xyz/openbmc_project/Logging/Entry/server.hpp>
namespace phosphor
{
namespace dbus
{
namespace monitoring
{
using namespace sdbusplus::xyz::openbmc_project::Logging::server;
using namespace phosphor::network::snmp;
using InternalFailure =
    sdbusplus::xyz::openbmc_project::Common::Error::InternalFailure;

static constexpr auto entry = "xyz.openbmc_project.Logging.Entry";

void ErrorTrap::trap(sdbusplus::message_t& msg) const
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
    auto errorID = std::get<uint32_t>(propMap.at("Id"));
    auto timestamp = std::get<uint64_t>(propMap.at("Timestamp"));
    auto sev = std::get<Entry::Level>(propMap.at("Severity"));
    auto isev = static_cast<uint8_t>(sev);
    auto message = std::get<std::string>(propMap.at("Message"));
    auto additionalData = std::get<std::map<std::string, std::string>>(
        propMap.at("AdditionalData"));
    for (auto& [k, v] : additionalData)
    {
        message.append(" ");
        message.append(k + "=" + v);
    }
    try
    {
        sendTrap<OBMCErrorNotification>(errorID, timestamp, isev, message);
    }
    catch (const InternalFailure& e)
    {
        lg2::error(
            "Failed to send SNMP trap: {ERROR}, ERROR_ID = {EID}, TIMESTAMP = {TSP}, SEVERITY = {SEVERITY}, MESSAGE = {MSG}",
            "ERROR", e, "EID", errorID, "TSP", timestamp, "SEVERITY",
            convertForMessage(sev), "MSG", message);
    }
}
} // namespace monitoring
} // namespace dbus
} // namespace phosphor
