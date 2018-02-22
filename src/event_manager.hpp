#pragma once

#include <sdbusplus/bus.hpp>

namespace phosphor
{
namespace events
{

/** @class Manager
 *  @brief OpenBMC Event manager implementation.
 */
class Manager
{
    public:
        Manager() = default;
        Manager(const Manager&) = delete;
        Manager& operator=(const Manager&) = delete;
        Manager(Manager&&) = default;
        Manager& operator=(Manager&&) = default;
        virtual ~Manager() = default;

        /** @brief Create the D-Bus Event object.
         *  @detail Add the objectPath,propertyName, propertyValue
         *          as additional data of the event object.
         *  @param[in] eventName - Name of the event.
         *  @param[in] objectPath - Path of the D-Bus object.
         *  @param[in] propertyName - Name of the property.
         *  @param[in] propertyValue - Value of the property.
         */
        void create(
            const std::string& eventName,
            const std::string& objectPath,
            const std::string& propertyName,
            const std::string& propertyValue);

};

Manager& getManager();

} // namespace events
} // namespace phosphor
