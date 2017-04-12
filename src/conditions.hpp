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
