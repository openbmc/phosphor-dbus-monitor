#pragma once

#include <experimental/any>
#include <sdbusplus/message.hpp>
#include "tupleref.hpp"

namespace any_ns = std::experimental;

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

constexpr auto MAPPER_BUSNAME = "xyz.openbmc_project.ObjectMapper";
constexpr auto MAPPER_PATH = "/xyz/openbmc_project/object_mapper";
constexpr auto MAPPER_INTERFACE = "xyz.openbmc_project.ObjectMapper";

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

/** @brief Lookup index for properties . */
// *INDENT-OFF*
using PropertyIndex = TupleRefMap <
        TupleOfRefs<
            const std::string,
            const std::string,
            any_ns::any>,
        const std::string,
        const std::string,
        const std::string >;
// *INDENT-ON*

/** @brief Convert some C++ types to others.
 *
 *  Remove type decorators to reduce template specializations.
 *
 *  1. Remove references.
 *  2. Remove 'const' and 'volatile'.
 */
template <typename T> struct Downcast
{
    using Type = std::remove_cv_t<std::remove_reference_t<T>>;
};
template <typename T> using DowncastType =
    typename Downcast<T>::Type;

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
