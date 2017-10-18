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

#include "config.h"
#include "event.hpp"
#include "event_manager.hpp"
#include "event_serialize.hpp"

#include <experimental/filesystem>

namespace phosphor
{
namespace events
{

void Manager::create(
    const std::string& eventName,
    const std::string& eventMessage,
    const std::string& objectPath,
    const std::string& propertyName,
    const std::string& propertyValue)
{
    using namespace std::string_literals;
    namespace fs = std::experimental::filesystem;

    auto msg = eventMessage;
    std::vector<std::string> additionalData;

    auto propVal = propertyName + "=" + propertyValue;
    auto path = "path="s + objectPath;

    additionalData.push_back(std::move(path));
    additionalData.push_back(std::move(propVal));

    auto& eventQueue = eventMap[eventName];

    // get the last event entry for this event
    // to generate the id.
    auto id = 0;
    if (eventQueue.size() > 0)
    {
        fs::path path(eventQueue.back()->objectPath);
        id = std::stoi(std::string(path.filename().c_str()));
        id ++;
    }

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::system_clock::now().time_since_epoch()).count();

    auto objPath =  std::string(OBJ_EVENT) + '/' + eventName + '/' +
                        std::to_string(id);

    // check for capping of the events,if cap reached then erase the oldest
    // event.
    if (eventQueue.size() == MAX_EVENTS)
    {
        fs::path eventPath(EVENTS_PERSIST_PATH);
        eventPath /= eventName;
        eventPath /= std::to_string(eventQueue.front()->timestamp());
        eventQueue.pop();
        fs::remove(eventPath);
    }

    auto event = std::make_unique<Entry>(
                       objPath,
                       ms, // Milliseconds since 1970
                       std::move(msg),
                       std::move(additionalData));
    serialize(*event, eventName);
    eventQueue.push(std::move(event));
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
        auto validEvent = false;

        for (auto& eventFile: fs::directory_iterator(eventDir))
        {
            auto timestamp = eventFile.path().filename().c_str();
            auto tsNum = std::stol(timestamp);
            auto eventPath = std::string(eventName) + '/' +
                             std::string(timestamp);
            auto objPath = std::string(OBJ_EVENT) + '/' + eventPath;

            auto event = std::make_unique<Entry>(
                       objPath,
                       tsNum);
            if (deserialize(eventFile.path(), *event))
            {
                event->emit_object_added();
                events.push(std::move(event));
                validEvent = true;
            }
        }

        if (validEvent)
        {
            eventMap[eventName] = std::move(events);
        }
    }
}


Manager& getManager()
{
    static Manager mgr;
    return mgr;
}

} // namespace events
} // namespace phosphor
