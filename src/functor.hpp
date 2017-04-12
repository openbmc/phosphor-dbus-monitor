#pragma once

#include "data_types.hpp"

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

class Monitor;

template <typename T>
auto make_condition(T&& condition)
{
    return Condition(std::forward<T>(condition));
}

template <typename T>
auto make_action(T&& action)
{
    return Action(std::forward<T>(action));
}

struct PropertyConditionBase
{
    PropertyConditionBase() = delete;
    virtual ~PropertyConditionBase() = default;
    PropertyConditionBase(const PropertyConditionBase&) = default;
    PropertyConditionBase& operator=(const PropertyConditionBase&) = default;
    PropertyConditionBase(PropertyConditionBase&&) = default;
    PropertyConditionBase& operator=(PropertyConditionBase&&) = default;

    /** @brief Constructor
     *
     *  The service argument can be nullptr.  If something
     *  else is provided the function will call the the
     *  service directly.  If omitted, the function will
     *  look up the service in the ObjectMapper.
     *
     *  @param path - The path of the object containing
     *     the property to be tested.
     *  @param iface - The interface hosting the property
     *     to be tested.
     *  @param property - The property to be tested.
     *  @param service - The DBus service hosting the object.
     */
    PropertyConditionBase(
        const char* path,
        const char* iface,
        const char* property,
        const char* service) :
        _path(path ? path : std::string()),
        _iface(iface),
        _property(property),
        _service(service) {}

    /** @brief Forward comparison to type specific implementation. */
    virtual bool eval(sdbusplus::message::message&) const = 0;

    /** @brief Test a property value.
     *
     * Make a DBus call and test the value of any property.
     */
    bool operator()(
        sdbusplus::bus::bus&,
        sdbusplus::message::message&,
        Monitor&) const;

private:
    std::string _path;
    std::string _iface;
    std::string _property;
    const char* _service;
};

template <typename T, typename U>
struct PropertyCondition final : public PropertyConditionBase
{
    PropertyCondition() = delete;
    ~PropertyCondition() = default;
    PropertyCondition(const PropertyCondition&) = default;
    PropertyCondition& operator=(const PropertyCondition&) = default;
    PropertyCondition(PropertyCondition&&) = default;
    PropertyCondition& operator=(PropertyCondition&&) = default;

    /** @brief Constructor
     *
     *  The service argument can be nullptr.  If something
     *  else is provided the function will call the the
     *  service directly.  If omitted, the function will
     *  look up the service in the ObjectMapper.
     *
     *  @param path - The path of the object containing
     *     the property to be tested.
     *  @param iface - The interface hosting the property
     *     to be tested.
     *  @param property - The property to be tested.
     *  @param condition - The test to run on the property.
     *  @param service - The DBus service hosting the object.
     */
    PropertyCondition(
        const char* path,
        const char* iface,
        const char* property,
        U&& condition,
        const char* service) :
        PropertyConditionBase(path, iface, property, service),
        _condition(std::forward<decltype(condition)>(condition)) {}

    /** @brief Test a property value.
     *
     * Make a DBus call and test the value of any property.
     */
    bool eval(sdbusplus::message::message& msg) const override
    {
        sdbusplus::message::variant<T> value;
        msg.read(value);
        return _condition(std::forward<T>(value.template get<T>()));
    }

private:
    U _condition;
};

template <typename T, typename U>
auto propertyStart(const char* path,
                   const char* iface,
                   const char* property,
                   U&& condition,
                   const char* service = nullptr)
{
    return PropertyCondition<T, U>(path,
                                   iface,
                                   property,
                                   std::move(condition),
                                   service);
}

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
