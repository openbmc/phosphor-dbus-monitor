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
#include <phosphor-logging/log.hpp>
#include "resolve_errors.hpp"
#include "sdbusplus.hpp"

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

constexpr auto PROPERTY_IFACE = "org.freedesktop.DBus.Properties";
constexpr auto ASSOCIATION_IFACE = "org.openbmc.Association";
constexpr auto ENDPOINTS_PROPERTY = "endpoints";

using namespace phosphor::logging;
using EndpointList = std::vector<std::string>;
using EndpointsProperty = sdbusplus::message::variant<EndpointList>;

void ResolveCallout::operator()()
{
    //Resolve all errors for this callout:
    //1) Read the 'endpoints' property for the callout/fault object
    //2) Follow each endpoint to its log entry
    //3) Set the Resolved property to true on the entry

    try
    {
        auto path = callout + "/fault";
        auto busName = SDBusPlus::getBusName(path, ASSOCIATION_IFACE);

        if (busName.empty())
        {
            //Just means we have no error logs with this callout
            return;
        }

        auto endpoints = SDBusPlus::callMethodAndRead<EndpointsProperty>(
                busName,
                path,
                PROPERTY_IFACE,
                "Get",
                ASSOCIATION_IFACE,
                ENDPOINTS_PROPERTY);

        const auto& logEntries = endpoints.get<EndpointList>();

        //Resolve each log entry
        for (const auto& logEntry : logEntries)
        {
            resolve(logEntry);
        }
    }
    catch (const std::exception& e)
    {
        log<level::ERR>(
                "Failed getting callout fault associations",
                entry("CALLOUT=%s", callout.c_str()),
                entry("MESSAGE=%s", e.what()));
    }
}

void ResolveCallout::resolve(const std::string& logEntry)
{
    //TODO: fill in
}

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
