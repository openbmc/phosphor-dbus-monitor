#pragma once

#include "event_entry.hpp"
#include "data_types.hpp"
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
     *  @param[in] eventMessage - Message for the event.
     *  @param[in] objectPath - Path of the D-Bus object.
     *  @param[in] propertyName - Name of the property.
     *  @param[in] propertyValue - Value of the property.
     */
    void create(const std::string& eventName, const std::string& eventMessage,
                const std::string& objectPath, const std::string& propertyName,
                const std::string& propertyValue);

    /** @brief Create the D-Bus Event object.
     *  @detail Add the objectPath, interface added as additional data
     *          of the event object
     *  @param[in] eventName - Name of the event.
     *  @param[in] eventMessage - Message for the event.
     *  @param[in] objectPath - Path of the D-Bus object.
     *  @param[in] intrfName - Name of the interface added.
     */
    void create(const std::string& eventName, const std::string& eventMessage,
                const std::string& objectPath, const std::string& interface);

  private:
    using AdditionalData = std::vector<std::string>;

    /** @brief Helper method to create the D-Bus Event object.
     *  @param[in] eventName - Name of the event.
     *  @param[in] eventMessage - Message for the event.
     *  @param[in] objectPath - Path of the D-Bus object.
     *  @param[in] additionalData - Additional data
     */
    void createHelper(const std::string& eventName,
                      const std::string& eventMessage,
                      const std::string& objectPath,
                      AdditionalData&& additionalData);

    using EventName = std::string;
    /** @brief Queue of events */
    using EventQueue = std::queue<std::unique_ptr<Entry>>;

    using EventMap = std::map<EventName, EventQueue>;
    /** @brief Map of event name  and the list of events **/
    EventMap eventMap;
};

Manager& getManager();

} // namespace events
} // namespace phosphor
