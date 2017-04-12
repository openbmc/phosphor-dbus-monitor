#pragma once

#include <phosphor-logging/log.hpp>

namespace phosphor
{
namespace dbus
{
namespace monitoring
{
namespace action
{

using namespace phosphor::logging;

inline auto log_error(const char* msg)
{
    return [=](auto&, auto&)
    {
        log<level::ERR>(msg);
    };
}

} // namespace action
} // namespace monitoring
} // namespace dbus
} // namespace phosphor
