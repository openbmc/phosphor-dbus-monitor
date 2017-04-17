#pragma once

#include <algorithm>
#include "data_types.hpp"

namespace phosphor
{
namespace dbus
{
namespace monitoring
{
namespace condition
{

/**
 * @brief A condition used to trigger an action when a number of items are at
 * or above a given value
 * @details A given group of items is updated with their last known item
 * value, which then the entire group is checked if there are a given number of
 * them at or above a value which would cause the condition to be true
 *
 * @param[in] items - Group of items
 * @param[in] path - Path of a item within the group
 * @param[in] count - Number of items needed at or above value
 * @param[in] value - Value of items to be at or above
 *
 * @return Lambda function
 *     A lambda function to determine if the number of items within the group
 *     are at or above the given value
 */
template <typename T>
auto countAtOrAbove(Group& items, const char* path, size_t count, T&& value)
{
    return [&items,
            path,
            count,
            value = std::forward<T>(value)](T&& arg)
    {
        Group::iterator it =
            std::find_if(items.begin(),
                         items.end(),
                         [&path](auto const& item)
                         {
                             return std::get<0>(item) == path;
                         });
        if (it != std::end(items))
        {
            std::get<1>(*it) = arg;
        }
        size_t condCount =
            std::count_if(items.begin(),
                          items.end(),
                          [&value](auto const& item)
                          {
                              return std::get<1>(item) >= value;
                          });
        return condCount >= count;
    };
}

} // namespace condition
} // namespace monitoring
} // namespace dbus
} // namespace phosphor
