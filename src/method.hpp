#pragma once

#include "callback.hpp"

#include <phosphor-logging/lg2.hpp>

#include <experimental/tuple>
#include <string>
#include <tuple>

namespace phosphor
{
namespace dbus
{
namespace monitoring
{
namespace detail
{

/** @class CallDBusMethod
 *  @brief Provide explicit call forwarding to
 *     DBusInterface::callMethodNoReply.
 *
 *  @tparam DBusInterface - The DBus interface to use.
 *  @tparam MethodArgs - DBus method argument types.
 */
template <typename DBusInterface, typename... MethodArgs>
struct CallDBusMethod
{
    static void op(const std::string& bus, const std::string& path,
                   const std::string& iface, const std::string& method,
                   MethodArgs&&... args)
    {
        try
        {
            DBusInterface::callMethodNoReply(bus, path, iface, method,
                                             std::forward<MethodArgs>(args)...);
        }
        catch (const sdbusplus::exception_t& e)
        {
            lg2::error(
                "Unable to call DBus method: {ERROR}. {BUS}, {PATH}, {INTF}, {METHOD}",
                "ERROR", e, "BUS", bus, "PATH", path, "INTF", iface, "METHOD",
                method);
        }
    }
};
} // namespace detail

/** @class MethodBase
 *  @brief Invoke DBus method callback implementation.
 *
 *  The method callback invokes the client supplied DBus method.
 */
class MethodBase : public Callback
{
  public:
    MethodBase() = delete;
    MethodBase(const MethodBase&) = delete;
    MethodBase(MethodBase&&) = default;
    MethodBase& operator=(const MethodBase&) = delete;
    MethodBase& operator=(MethodBase&&) = default;
    virtual ~MethodBase() = default;
    MethodBase(const std::string& b, const std::string& p, const std::string& i,
               const std::string& m) :
        Callback(), bus(b), path(p), interface(i), method(m)
    {}

    /** @brief Callback interface implementation. */
    void operator()(Context ctx) override = 0;

  protected:
    const std::string& bus;
    const std::string& path;
    const std::string& interface;
    const std::string& method;
};

/** @class Method
 *  @brief C++ type specific logic for the method callback.
 *
 *  @tparam DBusInterface - The DBus interface to use to call the method.
 *  @tparam MethodArgs - DBus method argument types.
 */
template <typename DBusInterface, typename... MethodArgs>
class Method : public MethodBase
{
  public:
    Method() = delete;
    Method(const Method&) = default;
    Method(Method&&) = default;
    Method& operator=(const Method&) = default;
    Method& operator=(Method&&) = default;
    ~Method() = default;
    Method(const std::string& bus, const std::string& path,
           const std::string& iface, const std::string& method,
           MethodArgs&&... arguments) :
        MethodBase(bus, path, iface, method),
        args(std::forward<MethodArgs>(arguments)...)
    {}

    /** @brief Callback interface implementation. */
    void operator()(Context /* ctx */) override
    {
        std::apply(detail::CallDBusMethod<DBusInterface, MethodArgs...>::op,
                   std::tuple_cat(std::make_tuple(bus), std::make_tuple(path),
                                  std::make_tuple(interface),
                                  std::make_tuple(method), args));
    }

  private:
    std::tuple<MethodArgs...> args;
};

/** @brief Argument type deduction for constructing Method instances. */
template <typename DBusInterface, typename... MethodArgs>
auto makeMethod(const std::string& bus, const std::string& path,
                const std::string& iface, const std::string& method,
                MethodArgs&&... arguments)
{
    return std::make_unique<Method<DBusInterface, MethodArgs...>>(
        bus, path, iface, method, std::forward<MethodArgs>(arguments)...);
}

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
