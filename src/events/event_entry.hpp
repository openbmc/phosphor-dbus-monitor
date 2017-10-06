#pragma once

#include <sdbusplus/bus.hpp>
#include <sdbusplus/server/object.hpp>
#include "xyz/openbmc_project/Event/Entry/server.hpp"

namespace phosphor
{
namespace events
{

using EntryIface = sdbusplus::server::object::object <
                        sdbusplus::xyz::openbmc_project::Event::server::Entry >;

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

        /** @brief Constructor to put object onto bus at a dbus path.
         *         Defer signal registration (pass true for deferSignal to the
         *         base class) until after the properties are set.
         *  @param[in] bus - Bus to attach to.
         *  @param[in] path - Path to attach at.
         *  @param[in] id - The event entry id.
         *  @param[in] timestamp - timestamp when the event created.
         *  @param[in] msg - The message of the error.
         *  @param[in] additionalData - The error metadata.
         */
        Entry(sdbusplus::bus::bus& bus,
              const std::string& path,
              uint32_t eventID,
              uint64_t eventTimestamp,
              std::string&& msg,
              std::vector<std::string>&& addData) :
            EntryIface(bus, path.c_str(), true)
        {
            id(eventID);
            timestamp(eventTimestamp);
            message(std::move(msg));
            additionalData(std::move(addData));
            // Emit deferred signal.
            this->emit_object_added();
        };
};

} // namespace events
} // namespace phosphor
