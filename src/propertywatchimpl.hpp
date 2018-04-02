#pragma once

#include <sdbusplus/message.hpp>
#include <sdbusplus/bus/match.hpp>
#include <vector>
#include "callback.hpp"
#include "data_types.hpp"
#include "propertywatch.hpp"

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

using MappedPropertyIndex =
    RefKeyMap<const std::string,
              RefKeyMap<const std::string, RefVector<const std::string>>>;

MappedPropertyIndex convert(const PropertyIndex& index);

template <typename DBusInterfaceType>
void PropertyWatch<DBusInterfaceType>::start()
{
    if (alreadyRan)
    {
        return;
    }

    // The index has a flat layout which is not optimal here.  Nest
    // properties in a map of interface names in a map of object paths.
    auto mapped = convert(index);

    for (const auto& m : mapped)
    {
        const auto& path = m.first.get();
        const auto& interfaces = m.second;

        // Do a query to populate the cache.  Start with a mapper query.
        // The specific services are queried below.
        const std::vector<std::string> queryInterfaces; // all interfaces
        auto mapperResp =
            DBusInterfaceType::template callMethodAndRead<GetObject>(
                MAPPER_BUSNAME, MAPPER_PATH, MAPPER_INTERFACE, "GetObject",
                path, queryInterfaces);

        for (const auto& i : interfaces)
        {
            const auto& interface = i.first.get();

            // Watch for property changes on this interface.
            DBusInterfaceType::addMatch(
                sdbusplus::bus::match::rules::propertiesChanged(path,
                                                                interface),
                [this](auto& msg)
                // *INDENT-OFF*
                {
                    std::string interface;
                    msg.read(interface);
                    auto path = msg.get_path();
                    this->propertiesChanged(msg, path, interface);
                });
            // *INDENT-ON*

            // The mapper response is a busname:[interfaces] map.  Look for
            // each interface in the index and if found, query the service and
            // populate the cache entries for the interface.
            for (const auto& mr : mapperResp)
            {
                const auto& busName = mr.first;
                const auto& mapperInterfaces = mr.second;
                if (mapperInterfaces.end() ==
                    std::find(mapperInterfaces.begin(), mapperInterfaces.end(),
                              interface))
                {
                    // This interface isn't being watched.
                    continue;
                }

                // Delegate type specific property updates to subclasses.
                updateProperties(busName, path, interface);
            }
        }
    }

    alreadyRan = true;
}

template <typename DBusInterfaceType>
void PropertyWatch<DBusInterfaceType>::callback(Context ctx)
{
    // Invoke callback if present.
    if (this->alreadyRan && this->cb)
    {
        (*this->cb)(ctx);
    }
}

template <typename T, typename DBusInterfaceType>
void PropertyWatchOfType<T, DBusInterfaceType>::updateProperties(
    const std::string& busName, const std::string& path,
    const std::string& interface)
{
    auto properties =
        DBusInterfaceType::template callMethodAndRead<PropertiesChanged<T>>(
            busName.c_str(), path.c_str(), "org.freedesktop.DBus.Properties",
            "GetAll", interface);
    propertiesChanged(path, interface, properties);
}

template <typename T, typename DBusInterfaceType>
void PropertyWatchOfType<T, DBusInterfaceType>::propertiesChanged(
    const std::string& path, const std::string& interface,
    const PropertiesChanged<T>& properties)
{
    // Update the cache for any watched properties.
    for (const auto& p : properties)
    {
        auto key = std::make_tuple(path, interface, p.first);
        auto item = this->index.find(key);
        if (item == this->index.end())
        {
            // This property isn't being watched.
            continue;
        }

        std::get<valueIndex>(std::get<storageIndex>(item->second).get()) =
            p.second.template get<T>();

        // Invoke callback if present.
        this->callback(Context::SIGNAL);
    }
}

template <typename T, typename DBusInterfaceType>
void PropertyWatchOfType<T, DBusInterfaceType>::propertiesChanged(
    sdbusplus::message::message& msg, const std::string& path,
    const std::string& interface)
{
    PropertiesChanged<T> properties;
    msg.read(properties);
    propertiesChanged(path, interface, properties);
}

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
