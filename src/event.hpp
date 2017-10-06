#pragma once

#include <phosphor-logging/log.hpp>
#include "callback.hpp"
#include "event_manager.hpp"

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

/** @class Event
 *  @brief Event callbck implementation
 *
 *  The event callback creates the event dbus object
 *  having event message and metadata key value pairs
 *  as specified by the client supplied property index.
 *
 *  @tparam T - The C++ type of the property values being traced.
 */

template <typename T>
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
        void operator()() override
        {
            for (const auto& n : index)
            {
                const auto& path = std::get<pathIndex>(n.first);
                const auto& propertyMeta = std::get<metaIndex>(n.first);
                const auto& value = std::get<valueIndex>(n.second);

                if (!value.get().empty())
                {
                    createEvent(
                            path,
                            propertyMeta,
                            value);
                }
            }

        }

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
            const any_ns::any& value) const
        {
            auto val = convertToStr(any_ns::any_cast<T>(value));
            phosphor::events::getManager().create(
                name, message, path, property, val);
        }

        /** @brief helper function converts the given value in string.
         *  @param[in] value
         *  @return converted string.
         */
        std::string convertToStr(std::string& value) const
        {
            return value;
        }

        /** @brief helper function converts the given value in string.
         *  @param[in] value
         *  @return converted string.
         */
        std::string convertToStr(bool value) const
        {
            using namespace std::string_literals;
            auto str = (any_ns::any_cast<T>(value)) ? "true"s : "false"s;
            return str;
        }

        /** @brief Event Name */
        std::string name;

        /** @brief Event Message */
        std::string message;
};

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
