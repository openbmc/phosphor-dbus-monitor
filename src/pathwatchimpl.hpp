/**
 * @file PathWatchimpl.hpp
 * @brief Add interfaces added watch for the specified path
 *
 */
#pragma once

#include "callback.hpp"
#include "data_types.hpp"
#include "pathwatch.hpp"

#include <sdbusplus/bus/match.hpp>
#include <sdbusplus/message.hpp>

#include <vector>

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

    for (const auto& pathRef : paths)
    {
        const auto& path = pathRef.get();

        auto match = event == ObjectManagerEvent::interfacesAdded
                         ? sdbusplus::match_rules::interfacesAdded()
                         : sdbusplus::match_rules::interfacesRemoved();

        if (match_object_path)
        {
            match.append(sdbusplus::match_rules::argNpath(0, path));
        }
        else
        {
            match.append(sdbusplus::match_rules::path(path));
        }

        // Watch for new interfaces added on this path.
        DBusInterfaceType::addMatch(match, [this](auto& msg) {
            (this->cb)(Context::SIGNAL, msg);
        });
    }

    alreadyRan = true;
}

template <typename DBusInterfaceType>
void PathWatch<DBusInterfaceType>::callback(Context ctx)
{
    (this->cb)(ctx);
}

template <typename DBusInterfaceType>
void PathWatch<DBusInterfaceType>::callback(Context ctx,
                                            sdbusplus::message_t& msg)
{
    (this->cb)(ctx, msg);
}
} // namespace monitoring
} // namespace dbus
} // namespace phosphor
