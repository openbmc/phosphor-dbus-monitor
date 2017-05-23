## This file is a template, the comment below is emitted into the generated file
/* This is an auto generated file. Do not edit. */
#pragma once

#include <array>
#include <string>
#include "data_types.hpp"
#include "propertywatchimpl.hpp"
#include "sdbusplus.hpp"

using namespace std::string_literals;

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

struct ConfigMeta
{
    using Meta = std::array<std::string, ${len(meta)}>;

    static auto& get()
    {
        static const Meta meta =
        {
% for m in meta:
            "${m.name}"s,
% endfor
        };
        return meta;
    }
};

struct ConfigPaths
{
    using Paths = std::array<std::string, ${len(paths)}>;

    static auto& get()
    {
        static const Paths paths =
        {
% for p in paths:
            "${p.name}"s,
% endfor
        };
        return paths;
    }
};

struct ConfigInterfaces
{
    using Interfaces = std::array<std::string, ${len(interfaces)}>;

    static auto& get()
    {
        static const Interfaces interfaces =
        {
% for i in interfaces:
            "${i.name}"s,
% endfor
        };
        return interfaces;
    }
};

struct ConfigProperties
{
    using Properties = std::array<std::string, ${len(propertynames)}>;

    static auto& get()
    {
        static const Properties properties =
        {
% for p in propertynames:
            "${p.name}"s,
% endfor
        };
        return properties;
    }
};

struct ConfigPropertyStorage
{
    using Storage = std::array<any_ns::any, ${len(instances)}>;

    static auto& get()
    {
        static Storage storage;
        return storage;
    }
};

struct ConfigPropertyIndicies
{
    using PropertyIndicies = std::array<PropertyIndex, ${len(instancegroups)}>;

    static auto& get()
    {
        static const PropertyIndicies propertyIndicies =
        {
            {
% for g in instancegroups:
                {
    % for i in g.members:
                    {
                        {
                            ConfigPaths::get()[${i[0]}],
                            ConfigInterfaces::get()[${i[2]}],
                            ConfigProperties::get()[${i[3]}]
                        },
                        {
                            ConfigMeta::get()[${i[1]}],
                            ConfigMeta::get()[${i[4]}],
                            ConfigPropertyStorage::get()[${i[5]}]
                        },
                    },
    % endfor
                },
% endfor
            }
        };
        return propertyIndicies;
    }
};

struct ConfigPropertyWatches
{
    using PropertyWatches = std::array<std::unique_ptr<Watch>, ${len(watches)}>;

    static auto& get()
    {
        static const PropertyWatches propertyWatches =
        {
% for w in watches:
            std::make_unique<PropertyWatchOfType<${w.datatype}, SDBusPlus>>(
                ConfigPropertyIndicies::get()[${w.instances}]),
% endfor
        };
        return propertyWatches;
    }
};
} // namespace monitoring
} // namespace dbus
} // namespace phosphor
