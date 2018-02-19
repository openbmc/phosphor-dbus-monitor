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

namespace phosphor
{
namespace events
{

void Manager::create(
    const std::string& eventName,
    const std::string& objectPath,
    const std::string& propertyName,
    const std::string& propertyvalue)
{
    // TODO Implement it in later commit.
}

Manager& getManager()
{
    static Manager mgr;
    return mgr;
}

} // namespace events
} // namespace phosphor
