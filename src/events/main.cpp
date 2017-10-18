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
#include "event_manager.hpp"
#include "generated.hpp"
#include "sdbusplus.hpp"

#include <sdbusplus/bus.hpp>
#include <sdbusplus/server/manager.hpp>
#include <experimental/filesystem>

using namespace phosphor::dbus::monitoring;
std::unique_ptr<phosphor::events::Manager> manager = nullptr;

struct Loop
{
    /** @brief indefinitely process dbus traffic. */
    static void run(sdbusplus::bus::bus& bus)
    {
        auto& event = SDEvent::getEvent();
        event.attach(bus);
        event.loop();
    }

    static auto& getBus()
    {
        return SDBusPlus::getBus();
    }
};

int main(void)
{
    auto& bus = Loop::getBus();

    // Add sdbusplus Object Manager for the 'root' path of the network manager.
    sdbusplus::server::manager::manager objManager(bus, OBJ_EVENT);
    bus.request_name(BUSNAME_EVENT);

    manager = std::make_unique<phosphor::events::Manager>(bus);

    // Create a directory to persist events.
    std::experimental::filesystem::create_directories(EVENTS_PERSIST_PATH);
    manager->restore();

    for (auto& watch : ConfigPropertyWatches::get())
    {
        watch->start();
    }

    Loop::run(bus);

    return -1;
}
