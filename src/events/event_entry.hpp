#pragma once

#include <sdbusplus/bus.hpp>
#include <sdbusplus/server/object.hpp>
#include "xyz/openbmc_project/Logging/Event/server.hpp"

namespace phosphor
{
namespace events
{

using EntryIface = sdbusplus::server::object::object <
                        sdbusplus::xyz::openbmc_project::Logging::server::Event >;

/** @class Entry
 *  @brief OpenBMC Event entry implementation.
 *  @details A concrete implementation for the
 *  xyz.openbmc_project.Event.Entry.
 */
class Entry : public EntryIface
{
    public:
        Entry() = delete;
        Entry(const Entry&) = delete;
        Entry& operator=(const Entry&) = delete;
        Entry(Entry&&) = delete;
        Entry& operator=(Entry&&) = delete;
        virtual ~Entry() = default;

        /** @brief Constructor to create an empty event object with only id,
         *  caller should make a call to emit added signal.
         *  @param[in] bus - Bus to attach to.
         *  @param[in] path - Path to attach at.
         *  @param[in] eventId - The event entry id.
         */
        Entry(sdbusplus::bus::bus& bus,
              const std::string& path,
              uint32_t eventId):
                    EntryIface(bus, path.c_str(), true)
        {
            id(eventId);
        };

        /** @brief Constructor to put object onto bus at a dbus path.
         *  @param[in] bus - Bus to attach to.
         *  @param[in] path - Path to attach at.
         *  @param[in] eventId - The event entry id.
         *  @param[in] timestamp - timestamp when the event created.
         *  @param[in] msg - The message of the event.
         *  @param[in] metaData - The event metadata.
         */
        Entry(sdbusplus::bus::bus& bus,
              const std::string& path,
              uint32_t eventId,
              uint64_t eventTimestamp,
              std::string&& msg,
              std::vector<std::string>&& metaData) :
                    EntryIface(bus, path.c_str(), true)
        {
            id(eventId);
            timestamp(eventTimestamp);
            message(std::move(msg));
            additionalData(std::move(metaData));
            // Emit deferred signal.
            this->emit_object_added();
        };
};

} // namespace events
} // namespace phosphor
