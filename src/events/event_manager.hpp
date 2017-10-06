#pragma once

#include "xyz/openbmc_project/Events/Internal/Manager/server.hpp"

#include <sdbusplus/bus.hpp>

namespace phosphor
{
namespace events
{

namespace details
{

template <typename T>
using ServerObject = typename sdbusplus::server::object::object<T>;

using ManagerIface =
    sdbusplus::xyz::openbmc_project::Events::Internal::server::Manager;

} // namespace details

/** @class Manager
 *  @brief OpenBMC Event manager implementation.
 *  @details A concrete implementation for the
 *  xyz.openbmc_project.Event.Internal.Manager DBus API.
 */
class Manager : public details::ServerObject<details::ManagerIface>
{
    public:
        Manager() = delete;
        Manager(const Manager&) = delete;
        Manager& operator=(const Manager&) = delete;
        Manager(Manager&&) = delete;
        Manager& operator=(Manager&&) = delete;
        virtual ~Manager() = default;

        /** @brief Constructor to put object onto bus at a dbus path.
         *  @param[in] bus - Bus to attach to.
         *  @param[in] path - Path to attach at.
         */
        Manager(sdbusplus::bus::bus& bus, const char* objPath) :
            details::ServerObject<details::ManagerIface>(bus, objPath),
            busEvent(bus),
            objectPath(objPath) {};

        /** @brief Create the DbusEvent object.
         *  @detail Add the objectPath,propertyName, propertyValue
         *          as additional data of the event object.
         *  @param[in] eventType - Type of the event.
         *  @param[in] objectPath - Path of the dbus object.
         *  @param[in] propertyName - Name of the property.
         *  @param[in] propertyValue - Value of the property.
         */
        void create(std::string eventType,
                    std::string objectPath,
                    std::string propertyName,
                    std::string propertyValue) override;


    private:

        /** @brief Persistent sdbusplus DBus bus connection. */
        sdbusplus::bus::bus& busEvent;

        /** @brief Path of Object. */
        std::string objectPath;
};

} // namespace events
} // namespace phosphor
