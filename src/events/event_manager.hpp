#pragma once

#include "event_entry.hpp"

#include <map>
#include <memory>
#include <queue>
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
        Manager() = delete;
        Manager(const Manager&) = delete;
        Manager& operator=(const Manager&) = delete;
        Manager(Manager&&) = delete;
        Manager& operator=(Manager&&) = delete;
        virtual ~Manager() = default;

        /** @brief Constructor to put object onto bus at a D-Bus path.
         *  @param[in] bus - Bus to attach to.
         */
        Manager(sdbusplus::bus::bus& bus) :
            dBus(bus) {};

        /** @brief Create the D-Bus Event object.
         *  @detail Add the objectPath,propertyName, propertyValue
         *          as additional data of the event object.
         *  @param[in] eventName - Name of the event.
         *  @param[in] eventMessage - Message for the event.
         *  @param[in] objectPath - Path of the D-Bus object.
         *  @param[in] propertyName - Name of the property.
         *  @param[in] propertyValue - Value of the property.
         */
        void create(const std::string& eventName,
                    const std::string& eventMessage,
                    const std::string& objectPath,
                    const std::string& propertyName,
                    const std::string& propertyValue);

        /** @brief Construct error d-bus objects from their persisted
         *         representations.
         */
        void restore();


    private:
        /** sdbusplus dbus */
        sdbusplus::bus::bus& dBus;

        using EventName = std::string;
        /** @brief Queue of events */
        using EventQueue = std::queue<std::unique_ptr<Entry>>;

        using EventMap = std::map<EventName, EventQueue>;

        /** @brief Map of event name  and the list of events **/
        EventMap eventMap;
};

} // namespace events
} // namespace phosphor
