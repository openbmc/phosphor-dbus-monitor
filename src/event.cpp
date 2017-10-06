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

extern std::unique_ptr<phosphor::events::Manager> manager;

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

void Event::operator()()
{
    for (const auto& n : index)
    {
        const auto& path = std::get<pathIndex>(n.first);
        const auto& property = std::get<propertyIndex>(n.first);
        const auto& value = std::get<valueIndex>(n.second);

        if (!value.get().empty())
        {
            createEvent(
                path,
                property,
                value);
        }
    }
}

void Event::createEvent(
        const std::string& path,
        const std::string& property,
        const any_ns::any& value) const
{
    auto val = convertToStr(value);
    if (manager)
    {
        manager->create(name, message, path, property, val);
    }
}

std::string Event::convertToStr(const any_ns::any& value) const
{
    std::string str {};
    if (value.type() == typeid(std::string))
    {
        str = any_ns::any_cast<std::string>(value);
    }
    else if (value.type() == typeid(bool))
    {
        str = (any_ns::any_cast<bool>(value)) ? "true" : "false";
    }
    // Need to add the support for other type if needed.
     return str;
}

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
