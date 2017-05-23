/**
 * @file propertywatch.hpp
 * @brief PropertyWatch class declarations.
 *
 * In general class users should include propertywatchimpl.hpp instead to avoid
 * link failures.
 */
#pragma once

#include "data_types.hpp"
#include "watch.hpp"

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

class Callback;

/** @class PropertyWatch
 *  @brief Type agnostic, factored out logic for property watches.
 *
 *  A property watch maintains the state of one or more DBus properties
 *  as specified by the supplied index.
 */
template <typename DBusInterfaceType>
class PropertyWatch : public Watch
{
    public:
        PropertyWatch() = delete;
        PropertyWatch(const PropertyWatch&) = delete;
        PropertyWatch(PropertyWatch&&) = default;
        PropertyWatch& operator=(const PropertyWatch&) = delete;
        PropertyWatch& operator=(PropertyWatch&&) = default;
        virtual ~PropertyWatch() = default;
        PropertyWatch(
            const PropertyIndex& watchIndex,
            Callback* cb = nullptr)
            : Watch(), index(watchIndex), callback(cb), ranOnce(false) {}

        /** @brief Start the watch.
         *
         *  Watch start interface implementation for PropertyWatch.
         */
        void start() override;

        /** @brief Update properties.
         *
         *  Subclasses to query the properties specified by the index
         *  and update the cache.
         */
        virtual void updateProperties(
            const std::string& busName,
            const std::string& path,
            const std::string& interface) = 0;

        /** @brief Dbus signal callback for PropertiesChanged.
         *
         *  Subclasses to update the cache.
         */
        virtual void propertiesChanged(
            sdbusplus::message::message&,
            const std::string& path,
            const std::string& interface) = 0;

        /** @brief Dbus signal callback for InterfacesAdded.
         *
         *  Subclasses to update the cache.
         */
        virtual void interfacesAdded(sdbusplus::message::message&) = 0;

    protected:

        /** @brief Property names and their associated storage. */
        const PropertyIndex& index;

        /** @brief Optional callback method. */
        Callback* const callback;

    private:

        /** @brief The start method should only be invoked once. */
        bool ranOnce;
};

/** @class PropertyWatchOfType
 *  @brief Type specific logic for PropertyWatch.
 *
 *  @tparam DBusInterfaceType - DBus access delegate.
 *  @tparam T - The type of the properties being watched.
 */
template <typename T, typename DBusInterfaceType>
class PropertyWatchOfType : public PropertyWatch<DBusInterfaceType>
{
    public:
        PropertyWatchOfType() = default;
        PropertyWatchOfType(const PropertyWatchOfType&) = delete;
        PropertyWatchOfType(PropertyWatchOfType&&) = default;
        PropertyWatchOfType& operator=(const PropertyWatchOfType&) = delete;
        PropertyWatchOfType& operator=(PropertyWatchOfType&&) = default;
        ~PropertyWatchOfType() = default;
        PropertyWatchOfType(
            const PropertyIndex& watchIndex, Callback& callback) :
            PropertyWatch<DBusInterfaceType>(watchIndex, &callback) {}
        PropertyWatchOfType(
            const PropertyIndex& watchIndex) :
            PropertyWatch<DBusInterfaceType>(watchIndex, nullptr) {}


        /** @brief PropertyMatch implementation for PropertyWatchOfType. */
        void updateProperties(
            const std::string& busName,
            const std::string& path,
            const std::string& interface) override;

        /** @brief PropertyMatch implementation for PropertyWatchOfType. */
        void propertiesChanged(
            sdbusplus::message::message&,
            const std::string& path,
            const std::string& interface) override;

        /** @brief DBus agnostic implementation of interfacesAdded. */
        void propertiesChanged(
            const std::string& path,
            const std::string& interface,
            const PropertiesChanged<T>&);

        /** @brief PropertyMatch implementation for PropertyWatchOfType. */
        void interfacesAdded(sdbusplus::message::message&) override;

        /** @brief DBus agnostic implementation of interfacesAdded. */
        void interfacesAdded(
            const std::string& path,
            const InterfacesAdded<T>&);
};

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
