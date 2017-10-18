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
#include "generated.hpp"
#include "sdbusplus.hpp"

#include <sdbusplus/bus.hpp>
#include <sdbusplus/server/manager.hpp>

using namespace phosphor::dbus::monitoring;

struct Loop
{
    /** @brief indefinitely process dbus traffic. */
    static void run()
    {
        auto& bus = SDBusPlus::getBus();
        auto& event = SDEvent::getEvent();
        event.attach(bus);
        event.loop();
    }
};

int main(void)
{
    auto& bus = SDBusPlus::getBus();

    // Add sdbusplus Object Manager for the 'root' path of events.
    sdbusplus::server::manager::manager objManager(bus, OBJ_EVENT);
    bus.request_name(BUSNAME_EVENT);

    phosphor::events::getManager().restore();

    for (auto& watch : ConfigPropertyWatches::get())
    {
        watch->start();
    }

    for (auto& watch : ConfigPropertyWatches::get())
    {
        watch->callback(Context::START);
    }

    Loop::run();

    return -1;
}
