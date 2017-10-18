#pragma once

#include <experimental/filesystem>
#include "event_entry.hpp"
#include "config.h"

namespace phosphor
{
namespace events
{

namespace fs = std::experimental::filesystem;

/** @brief Serialize and persist event d-bus object
 *  @param[in] event - const reference to event entry.
 *  @param[in] eventName - Name of the event.
 *  @param[in] dir - pathname of directory where the serialized event will
 *                   be placed.
 *  @return fs::path - pathname of persisted events file
 */
fs::path serialize(const Entry& event,
                   const std::string& eventName,
                   const fs::path& dir = fs::path(EVENTS_PERSIST_PATH));

/** @brief Deserialze a persisted event into a d-bus object
 *  @param[in] path - pathname of persisted event file
 *  @param[in] event - reference to event object which is the target of
 *             deserialization.
 *  @return bool - true if the deserialization was successful, false otherwise.
 */
bool deserialize(const fs::path& path, Entry& event);

} // namespace events
} // namespace phosphor
