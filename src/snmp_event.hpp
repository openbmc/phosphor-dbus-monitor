#pragma once
#include "callback.hpp"
#include <xyz/openbmc_project/Logging/Entry/server.hpp>

namespace phosphor
{
namespace dbus
{
namespace monitoring
{
using namespace sdbusplus::xyz::openbmc_project::Logging::server;
static constexpr auto entry = "xyz.openbmc_project.Logging.Entry";

// TODO openbmc/openbmc#3059
// TODO: temporarily added sendTrap method and ErrorNotification class
// they will be used from SNMP repo once available.
template<typename T, typename... ArgTypes>
void sendTrap(ArgTypes&&... tArgs)
{
}

class ErrorNotification
{
  public:
    ErrorNotification() = default;
    ErrorNotification(const ErrorNotification&) = delete;
    ErrorNotification(ErrorNotification&&) = default;
    ErrorNotification& operator=(const ErrorNotification&) = delete;
    ErrorNotification& operator=(ErrorNotification &&) = default;
    ~ErrorNotification() = default;
    ErrorNotification(uint32_t id, uint64_t ts, uint8_t sev, std::string msg,
                      bool reslv) 
    {
    }
};

/** @class Trap
 *  @brief Raises SNMP trap
 */
class Trap
{
  public:
    Trap() = default;
    Trap(const Trap&) = delete;
    Trap(Trap&&) = default;
    Trap& operator=(const Trap&) = delete;
    Trap& operator=(Trap&&) = default;
    virtual ~Trap() = default;
    virtual void trap(sdbusplus::message::message& msg) const = 0;
};

/** @class ErrorTrap
 *  @brief Sends SNMP trap for the elog error
 */
class ErrorTrap : public Trap
{
  public:
    ErrorTrap() = default;
    ErrorTrap(const ErrorTrap&) = delete;
    ErrorTrap(ErrorTrap&&) = default;
    ErrorTrap& operator=(const ErrorTrap&) = delete;
    ErrorTrap& operator=(ErrorTrap&&) = default;
    ~ErrorTrap() = default;
    void trap(sdbusplus::message::message& msg) const
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
            uint8_t isev =
                static_cast<uint8_t>(Entry::convertLevelFromString(sev));
            std::string message = propMap.at("Message").get<std::string>();
            bool resolved = propMap.at("Resolved").get<bool>();
            sendTrap<ErrorNotification>(errorID, timestamp, isev, message,
                                            resolved);
        }
    }
};

/** @class SNMPEvent
 *  @brief SNMP event callback implementation.
 */
template <typename T> class SNMPEvent : public Callback
{
  public:
    SNMPEvent(const SNMPEvent&) = delete;
    SNMPEvent(SNMPEvent&&) = default;
    SNMPEvent& operator=(const SNMPEvent&) = delete;
    SNMPEvent& operator=(SNMPEvent&&) = default;
    virtual ~SNMPEvent() = default;
    SNMPEvent() : Callback()
    {
    }

    /** @brief Callback interface implementation. */
    void operator()(Context ctx)
    {
    }

    /** @brief Callback interface implementation. */
    void operator()(Context ctx, sdbusplus::message::message& msg)
    {
        event.trap(msg);
    }
  private:
    T event;
};

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
