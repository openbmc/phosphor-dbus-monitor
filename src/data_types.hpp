#pragma once

#include <functional>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/message.hpp>

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

class Monitor;

/** @brief The possible item value types */
using Value = int64_t;

/** @brief A list of what constructs a unique item and its value */
using Group = std::vector<std::tuple<std::string, Value>>;

/** @brief A conditional function type for item(s) conditions */
using Condition = std::function<bool(sdbusplus::bus::bus&,
                                     sdbusplus::message::message&,
                                     Monitor&)>;

/** @brief A void function type for actions based condition(s) */
using Action = std::function<void(sdbusplus::bus::bus&,
                                  Monitor&)>;

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
