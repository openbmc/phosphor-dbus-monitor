#pragma once

#include <phosphor-logging/log.hpp>
#include "callback.hpp"

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

/** @class Event
 *  @brief Event callbck implementation
 */
class Event : public IndexedCallback
{
    public:
        Event() = delete;
        Event(const Event&) = delete;
        Event(Event&&) = default;
        Event& operator=(const Event&) = delete;
        Event& operator=(Event&&) = default;
        ~Event() = default;

        /** @brief Callback interface implementation. */
        void operator()() override;


        /** @brief Constructor.
         *  @param[in] eventName - Name of the event.
         *  @param[in] eventMessage- Event Message.
         *  @param[in] index - look up index for the properties.
         */
        Event(std::string eventName,
              std::string eventMessage,
              const PropertyIndex& index) :
            IndexedCallback(index),
            name(eventName),
            message(eventMessage) {}

    private:

        /** @brief Create the event Dbus Object.
         *  @param[in] path - Dbus Object Path for which the
         *                    property has changed.
         *  @param[in] property - Name of the property whose value
         *                        has been changed.
         *  @param[in] value - Changed property value.
         */
        void createEvent(
            const std::string& path,
            const std::string& property,
            const any_ns::any& value) const;

        /** @brief Event Name */
        std::string name;

        /** @brief Event Message */
        std::string message;
};

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
