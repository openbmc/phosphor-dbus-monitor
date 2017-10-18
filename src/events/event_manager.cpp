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
#include "config.h"
#include "event_serialize.hpp"

namespace phosphor
{
namespace events
{

void Manager::create(const std::string& eventName,
                     const std::string& eventMessage,
                     const std::string& objectPath,
                     const std::string& propertyName,
                     const std::string& propertyValue)
{
    using namespace std::string_literals;
    std::string msg = eventMessage;
    EventQueue events;
    std::vector<std::string> additionalData;

    std::string propVal = propertyName + "=" + propertyValue;
    std::string path = "path="s + objectPath;

    additionalData.push_back(std::move(path));
    additionalData.push_back(std::move(propVal));

    auto it = eventMap.find(eventName);
    if (it != eventMap.end())
    {
        events = std::move(it->second);
    }

    // get the last event entry for this event
    // to generate the id.
    auto id = (events.size() > 0) ? (events.back()->id() + 1) : 0;

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::system_clock::now().time_since_epoch()).count();

    auto objPath =  std::string(OBJ_EVENT) + '/' + eventName + '/' +
                    std::to_string(id);

    // check for capping of the events,if cap reached then erase the oldest
    // event.
    if (events.size() == MAX_EVENTS)
    {
        auto popEvent = std::move(events.front());
        events.pop();
        fs::path eventPath(EVENTS_PERSIST_PATH);
        eventPath /= eventName;
        eventPath /= std::to_string(popEvent->id());
        fs::remove(eventPath);
    }

    auto event = std::make_unique<Entry>(
                       dBus,
                       objPath,
                       id,
                       ms, // Milliseconds since 1970
                       std::move(msg),
                       std::move(additionalData));

    serialize(*event, eventName);

    events.push(std::move(event));
    eventMap[eventName] = std::move(events);
}

void Manager::restore()
{
    fs::path dir(EVENTS_PERSIST_PATH);

    if (!fs::exists(dir) || fs::is_empty(dir))
    {
        return;
    }

    for (auto& eventDir: fs::directory_iterator(dir))
    {
        EventQueue events;

        auto eventName = eventDir.path().filename().c_str();
        auto it = eventMap.find(eventName);
        if (it != eventMap.end())
        {
            events = std::move(it->second);
        }

        for (auto& eventFile: fs::directory_iterator(eventDir))
        {
            auto id = eventFile.path().filename().c_str();
            auto idNum = std::stol(id);
            auto eventPath = std::string(eventName) + '/' + std::string(id);
            auto objPath = std::string(OBJ_EVENT) + '/' + eventPath;

            auto event = std::make_unique<Entry>(
                       dBus,
                       objPath,
                       idNum);
            if (deserialize(eventFile.path(), *event))
            {
                event->emit_object_added();
                events.push(std::move(event));
                eventMap[eventName] = std::move(events);
            }
        }
    }
}

} // namespace events
} // namespace phosphor
