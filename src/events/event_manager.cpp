/**
 * Copyright © 2017 IBM Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "event.hpp"
#include "event_manager.hpp"
#include <chrono>
#include "config.h"

namespace phosphor
{
namespace events
{

void Manager::create(std::string eventType,
                     std::string objectPath,
                     std::string propertyName,
                     std::string propertyvalue)
{
    using namespace std::string_literals;
    EventQueue events;
    std::vector<std::string> additionalData;

    std::string message = propertyName + " changed.";

    std::string propVal = propertyName + "=" + propertyvalue;
    std::string path = "path="s + objectPath;

    additionalData.push_back(std::move(path));
    additionalData.push_back(std::move(propVal));

    auto it = eventTypeMap.find(eventType);
    if (it != eventTypeMap.end())
    {
        events = std::move(it->second);
    }

    // get the last event entry for this event type
    // to generate the id.
    auto id = (events.size() > 0) ? (events.back()->id() + 1) : 0;

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::system_clock::now().time_since_epoch()).count();

    auto objPath =  std::string(OBJ_EVENT) + '/' + eventType + '/' +
                    std::to_string(id);

    events.emplace(std::make_unique<Entry>(
                       busEvent,
                       objPath,
                       id,
                       ms, // Milliseconds since 1970
                       std::move(message),
                       std::move(additionalData)));

    if (it != eventTypeMap.end())
    {
        it->second = std::move(events);
    }
    else
    {
        eventTypeMap.insert(std::make_pair(eventType, std::move(events)));
    }
}

} // namespace events
} // namespace phosphor
