#pragma once

#include <phosphor-logging/elog-errors.hpp>
#include <phosphor-logging/elog.hpp>
#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/exception.hpp>
#include <sdbusplus/message.hpp>
#include <xyz/openbmc_project/Common/error.hpp>

#include <string>

namespace util
{
namespace detail
{
namespace errors = sdbusplus::xyz::openbmc_project::Common::Error;
} // namespace detail

/** @brief Alias for PropertiesChanged signal callbacks. */
template <typename... T>
using Properties = std::map<std::string, std::variant<T...>>;

namespace sdbusplus
{

/** @brief Get the bus connection. */
static auto& getBus() __attribute__((pure));
static auto& getBus()
{
    static auto bus = ::sdbusplus::bus::new_default();
    return bus;
}

/** @brief Invoke a method. */
template <typename... Args>
static auto callMethod(::sdbusplus::bus_t& bus, const std::string& busName,
                       const std::string& path, const std::string& interface,
                       const std::string& method, Args&&... args)
{
    auto reqMsg = bus.new_method_call(busName.c_str(), path.c_str(),
                                      interface.c_str(), method.c_str());
    reqMsg.append(std::forward<Args>(args)...);
    try
    {
        return bus.call(reqMsg);
    }
    catch (const std::exception& e)
    {
        lg2::error("Failed to invoke DBus method: {PATH}, {INTF}, {METHOD}",
                   "PATH", path, "INTF", interface, "METHOD", method);
        phosphor::logging::elog<detail::errors::InternalFailure>();
    }
}

/** @brief Invoke a method. */
template <typename... Args>
static auto callMethod(const std::string& busName, const std::string& path,
                       const std::string& interface, const std::string& method,
                       Args&&... args)
{
    return callMethod(getBus(), busName, path, interface, method,
                      std::forward<Args>(args)...);
}

/** @brief Invoke a method and read the response. */
template <typename Ret, typename... Args>
static auto callMethodAndRead(
    ::sdbusplus::bus_t& bus, const std::string& busName,
    const std::string& path, const std::string& interface,
    const std::string& method, Args&&... args)
{
    ::sdbusplus::message_t respMsg = callMethod<Args...>(
        bus, busName, path, interface, method, std::forward<Args>(args)...);
    Ret resp;
    respMsg.read(resp);
    return resp;
}

/** @brief Invoke a method and read the response. */
template <typename Ret, typename... Args>
static auto callMethodAndRead(
    const std::string& busName, const std::string& path,
    const std::string& interface, const std::string& method, Args&&... args)
{
    return callMethodAndRead<Ret>(getBus(), busName, path, interface, method,
                                  std::forward<Args>(args)...);
}

/** @brief Get service from the mapper. */
static auto getService(::sdbusplus::bus_t& bus, const std::string& path,
                       const std::string& interface)
{
    using namespace std::literals::string_literals;
    using GetObject = std::map<std::string, std::vector<std::string>>;

    auto mapperResp = callMethodAndRead<GetObject>(
        bus, "xyz.openbmc_project.ObjectMapper"s,
        "/xyz/openbmc_project/object_mapper"s,
        "xyz.openbmc_project.ObjectMapper"s, "GetObject"s, path,
        GetObject::mapped_type{interface});

    if (mapperResp.empty())
    {
        lg2::error("Object not found. {PATH}, {INTF}", "PATH", path, "INTF",
                   interface);
        phosphor::logging::elog<detail::errors::InternalFailure>();
    }
    return mapperResp.begin()->first;
}

/** @brief Get a property without mapper lookup. */
template <typename Property>
static auto getProperty(::sdbusplus::bus_t& bus, const std::string& busName,
                        const std::string& path, const std::string& interface,
                        const std::string& property)
{
    using namespace std::literals::string_literals;

    auto msg =
        callMethod(bus, busName, path, "org.freedesktop.DBus.Properties"s,
                   "Get"s, interface, property);
    ::std::variant<Property> value;
    msg.read(value);
    return std::get<Property>(value);
}

/** @brief Get a property without mapper lookup. */
template <typename Property>
static auto getProperty(const std::string& busName, const std::string& path,
                        const std::string& interface,
                        const std::string& property)
{
    return getProperty<Property>(getBus(), busName, path, interface, property);
}

/** @brief Get a property with mapper lookup. */
template <typename Property>
static auto getProperty(::sdbusplus::bus_t& bus, const std::string& path,
                        const std::string& interface,
                        const std::string& property)
{
    return getProperty<Property>(bus, getService(bus, path, interface), path,
                                 interface, property);
}

/** @brief Get a property with mapper lookup. */
template <typename Property>
static auto getProperty(const std::string& path, const std::string& interface,
                        const std::string& property)
{
    return getProperty<Property>(getBus(), path, interface, property);
}

} // namespace sdbusplus
} // namespace util
