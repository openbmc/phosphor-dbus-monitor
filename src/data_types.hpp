#pragma once

#include <functional>
#include <experimental/any>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/message.hpp>
#include "tupleref.hpp"

namespace any_ns = std::experimental;

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

/** @brief A map with references as keys. */
template <typename Key, typename Value>
using RefKeyMap = std::map<std::reference_wrapper<Key>, Value, std::less<Key>>;

/** @brief A map with a tuple of references as keys. */
template <typename Value, typename ...Keys>
using TupleRefMap = std::map<TupleOfRefs<Keys...>, Value, TupleOfRefsLess>;

/** @brief A vector of references. */
template <typename T>
using RefVector = std::vector<std::reference_wrapper<T>>;

/** @brief
 *
 *  The mapper has a defect such that it provides strings
 *  rather than object paths.  Use an alias for easy refactoring
 *  when the mapper is fixed.
 */
using MapperPath = std::string;

/** @brief ObjectManager.InterfacesAdded signal signature alias. */
template <typename T>
using InterfacesAdded = std::map <
                        std::string,
                        std::map <
                        std::string,
                        sdbusplus::message::variant<T >>>;

/** @brief ObjectMapper.GetObject response signature alias. */
using GetObject = std::map<MapperPath, std::vector<std::string>>;

/** @brief Properties.GetAll response signature alias. */
template <typename T>
using PropertiesChanged = std::map <
                          std::string,
                          sdbusplus::message::variant<T >>;

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
