/**
 * @file interfacewatchimpl.hpp
 * @brief Add interfaces added watch for the specified paths
 *
 */
#pragma once

#include <sdbusplus/message.hpp>
#include <sdbusplus/bus/match.hpp>
#include <vector>
#include "callback.hpp"
#include "data_types.hpp"
#include "interfacewatch.hpp"

namespace phosphor
{
namespace dbus
{
namespace monitoring
{
template <typename DBusInterfaceType>
void InterfaceWatch<DBusInterfaceType>::start()
{
    if (alreadyRan)
    {
        return;
    }
    // Watch for new interfaces on this path.
    DBusInterfaceType::addMatch(
        sdbusplus::bus::match::rules::interfacesAdded(objPath),
        [this](auto& msg)
        // *INDENT-OFF*
        { this->interfacesAdded(msg); });
    // *INDENT-ON*
    alreadyRan = true;
}

template <typename DBusInterfaceType>
void InterfaceWatch<DBusInterfaceType>::callback(Context ctx)
{
    // Invoke callback if present.
    if (this->alreadyRan && this->cb)
    {
        (*this->cb)(ctx);
    }
}
template <typename DBusInterfaceType>
void InterfaceWatch<DBusInterfaceType>::interfacesAdded(
    sdbusplus::message::message& msg)
{
    sdbusplus::message::object_path path;
    msg.read(path);
    InterfacesAdded intfs;
    msg.read(intfs);
}
} // namespace monitoring
} // namespace dbus
} // namespace phosphor
