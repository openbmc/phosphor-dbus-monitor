/**
 * @file PathWatchimpl.hpp
 * @brief Add interfaces added watch for the specified path
 *
 */
#pragma once

#include <sdbusplus/message.hpp>
#include <sdbusplus/bus/match.hpp>
#include <vector>
#include "callback.hpp"
#include "data_types.hpp"
#include "pathwatch.hpp"

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

template <typename DBusInterfaceType>
void PathWatch<DBusInterfaceType>::start()
{
    if (alreadyRan)
    {
        return;
    }
    // Watch for new interfaces added on this path.
    DBusInterfaceType::addMatch(
        sdbusplus::bus::match::rules::interfacesAdded(objectPath),
        [this](auto& msg)
    // *INDENT-OFF*
        { this->interfacesAdded(msg); });
    // *INDENT-ON*

    alreadyRan = true;
}

template <typename DBusInterfaceType>
void PathWatch<DBusInterfaceType>::callback(Context ctx)
{
    // Invoke callback if present.
    if (this->alreadyRan && this->cb)
    {
        (*this->cb)(ctx);
    }
}

template <typename DBusInterfaceType>
void PathWatch<DBusInterfaceType>::callback(Context ctx,
        sdbusplus::message::message& msg)
{
    // Invoke callback if present.
    if (this->alreadyRan && this->cb)
    {
        (*this->cb)(ctx, msg);
    }
}

template <typename DBusInterfaceType>
void PathWatch<DBusInterfaceType>::interfacesAdded(
    sdbusplus::message::message& msg)
{
    this->callback(Context::SIGNAL, msg);
}
} // namespace monitoring
} // namespace dbus
} // namespace phosphor