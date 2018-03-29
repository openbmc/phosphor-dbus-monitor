#pragma once

#include <sdbusplus/bus.hpp>
#include <sdbusplus/message.hpp>
#include <sdbusplus/bus/match.hpp>
#include "data_types.hpp"

struct Loop;

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

/** @class SDBusPlus
 *  @brief DBus access delegate implementation for sdbusplus.
 */
class SDBusPlus
{
  private:
    static auto& getWatches()
    {
        static std::vector<sdbusplus::bus::match::match> watches;
        return watches;
    }

  public:
    static auto& getBus()
    {
        static auto bus = sdbusplus::bus::new_default();
        return bus;
    }

    /** @brief Invoke a method; ignore reply. */
    template <typename... Args>
    static void callMethodNoReply(const std::string& busName,
                                  const std::string& path,
                                  const std::string& interface,
                                  const std::string& method, Args&&... args)
    {
        auto reqMsg = getBus().new_method_call(
            busName.c_str(), path.c_str(), interface.c_str(), method.c_str());
        reqMsg.append(std::forward<Args>(args)...);
        getBus().call_noreply(reqMsg);

        // TODO: openbmc/openbmc#1719
        // invoke these methods async, with a callback
        // handler that checks for errors and logs.
    }

    /** @brief Invoke a method. */
    template <typename... Args>
    static auto callMethod(const std::string& busName, const std::string& path,
                           const std::string& interface,
                           const std::string& method, Args&&... args)
    {
        auto reqMsg = getBus().new_method_call(
            busName.c_str(), path.c_str(), interface.c_str(), method.c_str());
        reqMsg.append(std::forward<Args>(args)...);
        return getBus().call(reqMsg);
    }

    /** @brief Invoke a method and read the response. */
    template <typename Ret, typename... Args>
    static auto callMethodAndRead(const std::string& busName,
                                  const std::string& path,
                                  const std::string& interface,
                                  const std::string& method, Args&&... args)
    {
        Ret resp;
        sdbusplus::message::message respMsg = callMethod<Args...>(
            busName, path, interface, method, std::forward<Args>(args)...);
        respMsg.read(resp);
        return resp;
    }

    /** @brief Register a DBus signal callback. */
    static auto
        addMatch(const std::string& match,
                 const sdbusplus::bus::match::match::callback_t& callback)
    {
        getWatches().emplace_back(getBus(), match, callback);
    }

    /** @brief Look up the bus name for a path and interface */
    static auto getBusName(const std::string& path,
                           const std::string& interface)
    {
        std::vector<std::string> interfaces{interface};

        auto object = callMethodAndRead<GetObject>(
            MAPPER_BUSNAME, MAPPER_PATH, MAPPER_INTERFACE, "GetObject", path,
            interfaces);

        std::string name;
        if (!object.empty())
        {
            name = object.begin()->first;
        }
        return name;
    }

    friend Loop;
};

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
