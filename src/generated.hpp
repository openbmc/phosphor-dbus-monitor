/* This is an auto generated file. Do not edit. */
#pragma once

#include <array>
#include <chrono>
#include <string>
#include "count.hpp"
#include "data_types.hpp"
#include "journal.hpp"
#include "elog.hpp"
#include "errors.hpp"
#include "method.hpp"
#include "propertywatchimpl.hpp"
#include "pathwatchimpl.hpp"
#include "resolve_errors.hpp"
#include "sdbusplus.hpp"
#include "event.hpp"
#include "sdevent.hpp"

using namespace std::string_literals;
using namespace std::chrono_literals;

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

struct ConfigMeta
{
    using Meta = std::array<std::string, 2>;

    static auto& get()
    {
        static const Meta meta =
        {
            "PROPERTY"s,
            "PATH"s,
        };
        return meta;
    }
};

struct ConfigPaths
{
    using Paths = std::array<std::string, 7>;

    static auto& get()
    {
        static const Paths paths =
        {
            "/org/freedesktop/systemd1"s,
            "/xyz/openbmc_project/testing/inst1"s,
            "/xyz/openbmc_project/testing/inst2"s,
            "/xyz/openbmc_project/testing/inst3"s,
            "/xyz/openbmc_project/testing/inst4"s,
            "/xyz/openbmc_project/logging"s,
            "/test"s,
        };
        return paths;
    }
};

struct ConfigInterfaces
{
    using Interfaces = std::array<std::string, 13>;

    static auto& get()
    {
        static const Interfaces interfaces =
        {
            "xyz.openbmc_project.Sensor.Value"s,
            "org.freedesktop.systemd1"s,
            "org.freedesktop.systemd1.Manager"s,
            "org.test"s,
            "xyz.openbmc_project.byteIface"s,
            "xyz.openbmc_project.U16Iface"s,
            "xyz.openbmc_project.U32Iface"s,
            "xyz.openbmc_project.U64Iface"s,
            "xyz.openbmc_project.S16Iface"s,
            "xyz.openbmc_project.S32Iface"s,
            "xyz.openbmc_project.S64Iface"s,
            "xyz.openbmc_project.StringIface"s,
            "xyz.openbmc_project.BoolIface"s,
        };
        return interfaces;
    }
};

struct ConfigIntfAddPaths
{
    using Paths = std::array<std::string, 1>;

    static auto& get()
    {
        static const Paths paths =
        {
            "/xyz/openbmc_project/logging"s,
        };
        return paths;
    }
};

struct ConfigProperties
{
    using Properties = std::array<std::string, 13>;

    static auto& get()
    {
        static const Properties properties =
        {
            "ValueA"s,
            "ValueB"s,
            "StartUnit"s,
            "Test"s,
            "byteValue"s,
            "u16Value"s,
            "u32Value"s,
            "u64Value"s,
            "s16Value"s,
            "s32Value"s,
            "s64Value"s,
            "StringValue"s,
            "BoolValue"s,
        };
        return properties;
    }
};

struct ConfigPropertyStorage
{
    using Storage = std::array<std::tuple<any_ns::any, any_ns::any>, 44>;

    static auto& get()
    {
        static Storage storage;
        return storage;
    }
};

struct ConfigPropertyIndicies
{
    using PropertyIndicies = std::array<PropertyIndex, 10>;

    static auto& get()
    {
        static const PropertyIndicies propertyIndicies =
        {
            {
                {
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[1],
                            ConfigInterfaces::get()[0],
                            ConfigProperties::get()[0]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[0]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[1],
                            ConfigInterfaces::get()[0],
                            ConfigProperties::get()[1]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[1]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[2],
                            ConfigInterfaces::get()[0],
                            ConfigProperties::get()[0]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[2]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[2],
                            ConfigInterfaces::get()[0],
                            ConfigProperties::get()[1]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[3]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[3],
                            ConfigInterfaces::get()[0],
                            ConfigProperties::get()[0]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[4]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[3],
                            ConfigInterfaces::get()[0],
                            ConfigProperties::get()[1]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[5]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[4],
                            ConfigInterfaces::get()[0],
                            ConfigProperties::get()[0]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[6]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[4],
                            ConfigInterfaces::get()[0],
                            ConfigProperties::get()[1]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[7]
                        },
                    },
                },
                {
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[1],
                            ConfigInterfaces::get()[4],
                            ConfigProperties::get()[4]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[8]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[2],
                            ConfigInterfaces::get()[4],
                            ConfigProperties::get()[4]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[9]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[3],
                            ConfigInterfaces::get()[4],
                            ConfigProperties::get()[4]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[10]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[4],
                            ConfigInterfaces::get()[4],
                            ConfigProperties::get()[4]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[11]
                        },
                    },
                },
                {
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[1],
                            ConfigInterfaces::get()[5],
                            ConfigProperties::get()[5]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[12]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[2],
                            ConfigInterfaces::get()[5],
                            ConfigProperties::get()[5]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[13]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[3],
                            ConfigInterfaces::get()[5],
                            ConfigProperties::get()[5]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[14]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[4],
                            ConfigInterfaces::get()[5],
                            ConfigProperties::get()[5]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[15]
                        },
                    },
                },
                {
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[1],
                            ConfigInterfaces::get()[6],
                            ConfigProperties::get()[6]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[16]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[2],
                            ConfigInterfaces::get()[6],
                            ConfigProperties::get()[6]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[17]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[3],
                            ConfigInterfaces::get()[6],
                            ConfigProperties::get()[6]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[18]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[4],
                            ConfigInterfaces::get()[6],
                            ConfigProperties::get()[6]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[19]
                        },
                    },
                },
                {
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[1],
                            ConfigInterfaces::get()[7],
                            ConfigProperties::get()[7]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[20]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[2],
                            ConfigInterfaces::get()[7],
                            ConfigProperties::get()[7]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[21]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[3],
                            ConfigInterfaces::get()[7],
                            ConfigProperties::get()[7]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[22]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[4],
                            ConfigInterfaces::get()[7],
                            ConfigProperties::get()[7]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[23]
                        },
                    },
                },
                {
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[1],
                            ConfigInterfaces::get()[8],
                            ConfigProperties::get()[8]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[24]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[2],
                            ConfigInterfaces::get()[8],
                            ConfigProperties::get()[8]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[25]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[3],
                            ConfigInterfaces::get()[8],
                            ConfigProperties::get()[8]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[26]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[4],
                            ConfigInterfaces::get()[8],
                            ConfigProperties::get()[8]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[27]
                        },
                    },
                },
                {
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[1],
                            ConfigInterfaces::get()[9],
                            ConfigProperties::get()[9]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[28]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[2],
                            ConfigInterfaces::get()[9],
                            ConfigProperties::get()[9]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[29]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[3],
                            ConfigInterfaces::get()[9],
                            ConfigProperties::get()[9]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[30]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[4],
                            ConfigInterfaces::get()[9],
                            ConfigProperties::get()[9]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[31]
                        },
                    },
                },
                {
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[1],
                            ConfigInterfaces::get()[10],
                            ConfigProperties::get()[10]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[32]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[2],
                            ConfigInterfaces::get()[10],
                            ConfigProperties::get()[10]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[33]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[3],
                            ConfigInterfaces::get()[10],
                            ConfigProperties::get()[10]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[34]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[4],
                            ConfigInterfaces::get()[10],
                            ConfigProperties::get()[10]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[35]
                        },
                    },
                },
                {
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[1],
                            ConfigInterfaces::get()[12],
                            ConfigProperties::get()[12]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[36]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[2],
                            ConfigInterfaces::get()[12],
                            ConfigProperties::get()[12]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[37]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[3],
                            ConfigInterfaces::get()[12],
                            ConfigProperties::get()[12]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[38]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[4],
                            ConfigInterfaces::get()[12],
                            ConfigProperties::get()[12]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[39]
                        },
                    },
                },
                {
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[1],
                            ConfigInterfaces::get()[11],
                            ConfigProperties::get()[11]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[40]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[2],
                            ConfigInterfaces::get()[11],
                            ConfigProperties::get()[11]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[41]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[3],
                            ConfigInterfaces::get()[11],
                            ConfigProperties::get()[11]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[42]
                        },
                    },
                    {
                        PropertyIndex::key_type
                        {
                            ConfigPaths::get()[4],
                            ConfigInterfaces::get()[11],
                            ConfigProperties::get()[11]
                        },
                        PropertyIndex::mapped_type
                        {
                            ConfigMeta::get()[1],
                            ConfigMeta::get()[0],
                            ConfigPropertyStorage::get()[43]
                        },
                    },
                },
            }
        };
        return propertyIndicies;
    }
};

struct ConfigPropertyCallbackGroups
{
    using CallbackGroups = std::array<std::vector<size_t>, 3>;
    static auto& get()
    {
        static const CallbackGroups propertyCallbackGraph =
        {
            {
                {0, 8, 1},
                {6},
                {24},
            }
        };
        return propertyCallbackGraph;
    }
};

struct ConfigConditions
{
    using Conditions = std::array<std::unique_ptr<Conditional>, 15>;

    static auto& get()
    {
        static const Conditions propertyConditions =
        {
            std::make_unique<CountCondition<uint32_t>>(
                ConfigPropertyIndicies::get()[0],
                [](const auto& item){return item >= 3;},
                [](const auto& item){return item >= static_cast<uint32_t>(115ull);},
                true),
            std::make_unique<CountCondition<uint32_t>>(
                ConfigPropertyIndicies::get()[0],
                [](const auto& item){return item >= 3;},
                [](const auto& item){return item >= static_cast<uint32_t>(115ull);},
                false),
            std::make_unique<CountCondition<uint8_t>>(
                ConfigPropertyIndicies::get()[1],
                [](const auto& item){return item < 3;},
                [](const auto& item){return item < static_cast<uint8_t>(115);},
                true),
            std::make_unique<CountCondition<uint8_t>>(
                ConfigPropertyIndicies::get()[1],
                [](const auto& item){return item <= 3;},
                [](const auto& item){return item <= static_cast<uint8_t>(115);},
                false),
            std::make_unique<CountCondition<uint8_t>>(
                ConfigPropertyIndicies::get()[1],
                [](const auto& item){return item > 3;},
                [](const auto& item){return item > static_cast<uint8_t>(115);},
                false),
            std::make_unique<CountCondition<uint8_t>>(
                ConfigPropertyIndicies::get()[1],
                [](const auto& item){return item >= 3;},
                [](const auto& item){return item >= static_cast<uint8_t>(115);},
                false),
            std::make_unique<CountCondition<uint8_t>>(
                ConfigPropertyIndicies::get()[1],
                [](const auto& item){return item == 3;},
                [](const auto& item){return item == static_cast<uint8_t>(115);},
                false),
            std::make_unique<CountCondition<uint8_t>>(
                ConfigPropertyIndicies::get()[1],
                [](const auto& item){return item != 3;},
                [](const auto& item){return item != static_cast<uint8_t>(115);},
                false),
            std::make_unique<CountCondition<uint16_t>>(
                ConfigPropertyIndicies::get()[2],
                [](const auto& item){return item == 3;},
                [](const auto& item){return item == static_cast<uint16_t>(115ull);},
                false),
            std::make_unique<CountCondition<uint32_t>>(
                ConfigPropertyIndicies::get()[3],
                [](const auto& item){return item == 3;},
                [](const auto& item){return item == static_cast<uint32_t>(115ull);},
                false),
            std::make_unique<CountCondition<uint64_t>>(
                ConfigPropertyIndicies::get()[4],
                [](const auto& item){return item == 3;},
                [](const auto& item){return item == static_cast<uint64_t>(115ull);},
                false),
            std::make_unique<CountCondition<int16_t>>(
                ConfigPropertyIndicies::get()[5],
                [](const auto& item){return item == 3;},
                [](const auto& item){return item == static_cast<int16_t>(115ll);},
                false),
            std::make_unique<CountCondition<int32_t>>(
                ConfigPropertyIndicies::get()[6],
                [](const auto& item){return item == 3;},
                [](const auto& item){return item == static_cast<int32_t>(115ll);},
                false),
            std::make_unique<CountCondition<int64_t>>(
                ConfigPropertyIndicies::get()[7],
                [](const auto& item){return item == 3;},
                [](const auto& item){return item == static_cast<int64_t>(115ll);},
                false),
            std::make_unique<CountCondition<bool>>(
                ConfigPropertyIndicies::get()[8],
                [](const auto& item){return item == 3;},
                [](const auto& item){return item == false;},
                false),
        };
        return propertyConditions;
    }
};

struct ConfigPropertyCallbacks
{
    using Callbacks = std::array<std::unique_ptr<Callback>, 39>;

    static auto& get()
    {
        static const Callbacks propertyCallbacks =
        {
            std::make_unique<Journal<uint32_t, phosphor::logging::level::INFO>>(
                "Hello world from PDM!",
                ConfigPropertyIndicies::get()[0]),
            makeElog<sdbusplus::xyz::openbmc_project::Common::Error::InvalidArgument>(
                phosphor::logging::xyz::openbmc_project::Common::InvalidArgument::ARGUMENT_NAME("testing..."),
                phosphor::logging::xyz::openbmc_project::Common::InvalidArgument::ARGUMENT_VALUE("testing...")),
            std::make_unique<ElogWithMetadataCapture<
                sdbusplus::xyz::openbmc_project::Common::Callout::Error::Inventory,
                phosphor::logging::xyz::openbmc_project::Common::Callout::Inventory::CALLOUT_INVENTORY_PATH,
                uint32_t>>(
                ConfigPropertyIndicies::get()[0]),
            std::make_unique<Event<uint32_t>>("test"s, "Test configuration changed."s,
                ConfigPropertyIndicies::get()[0]),
            makeMethod<SDBusPlus>(
                ConfigInterfaces::get()[1],
                ConfigPaths::get()[0],
                ConfigInterfaces::get()[2],
                ConfigProperties::get()[2],
                "foo.unit"s,
                "replace"s),
            std::make_unique<ResolveCallout>("/xyz/openbmc_project/inventory/system/chassis/motherboard/fan0"),
            std::make_unique<GroupOfCallbacks<ConfigPropertyCallbacks>>(
                ConfigPropertyCallbackGroups::get()[0]),
            std::make_unique<ConditionalCallback<ConfigPropertyCallbacks>>(
                ConfigPropertyCallbackGroups::get()[1],
                *ConfigConditions::get()[0]),
            std::make_unique<DeferrableCallback<ConfigPropertyCallbacks, SDEventTimer>>(
                ConfigPropertyCallbackGroups::get()[1],
                *ConfigConditions::get()[1],
                1000us),
            std::make_unique<Journal<uint8_t, phosphor::logging::level::DEBUG>>(
                "Testing...",
                ConfigPropertyIndicies::get()[1]),
            std::make_unique<Journal<uint16_t, phosphor::logging::level::INFO>>(
                "Testing...",
                ConfigPropertyIndicies::get()[2]),
            std::make_unique<Journal<uint32_t, phosphor::logging::level::NOTICE>>(
                "Testing...",
                ConfigPropertyIndicies::get()[3]),
            std::make_unique<Journal<uint64_t, phosphor::logging::level::WARNING>>(
                "Testing...",
                ConfigPropertyIndicies::get()[4]),
            std::make_unique<Journal<int16_t, phosphor::logging::level::ERR>>(
                "Testing...",
                ConfigPropertyIndicies::get()[5]),
            std::make_unique<Journal<int32_t, phosphor::logging::level::CRIT>>(
                "Testing...",
                ConfigPropertyIndicies::get()[6]),
            std::make_unique<Journal<int64_t, phosphor::logging::level::ALERT>>(
                "Testing...",
                ConfigPropertyIndicies::get()[7]),
            std::make_unique<Journal<bool, phosphor::logging::level::EMERG>>(
                "Testing...",
                ConfigPropertyIndicies::get()[8]),
            std::make_unique<Journal<std::string, phosphor::logging::level::INFO>>(
                "Testing...",
                ConfigPropertyIndicies::get()[9]),
            makeElog<sdbusplus::xyz::openbmc_project::Common::Error::InvalidArgument>(
                phosphor::logging::xyz::openbmc_project::Common::InvalidArgument::ARGUMENT_NAME("testing..."),
                phosphor::logging::xyz::openbmc_project::Common::InvalidArgument::ARGUMENT_VALUE("testing...")),
            makeElog<sdbusplus::xyz::openbmc_project::Common::Callout::Error::Device>(
                phosphor::logging::xyz::openbmc_project::Common::Callout::Device::CALLOUT_ERRNO(123),
                phosphor::logging::xyz::openbmc_project::Common::Callout::Device::CALLOUT_DEVICE_PATH("testing...")),
            makeElog<sdbusplus::xyz::openbmc_project::Common::File::Error::Seek>(
                phosphor::logging::xyz::openbmc_project::Common::File::Seek::OFFSET(123),
                phosphor::logging::xyz::openbmc_project::Common::File::Seek::WHENCE(123),
                phosphor::logging::xyz::openbmc_project::Common::File::Seek::ERRNO(123),
                phosphor::logging::xyz::openbmc_project::Common::File::Seek::PATH("testing...")),
            makeElog<sdbusplus::xyz::openbmc_project::Common::Error::InternalFailure>(
                ),
            std::make_unique<ElogWithMetadataCapture<
                sdbusplus::xyz::openbmc_project::Common::Callout::Error::Inventory,
                phosphor::logging::xyz::openbmc_project::Common::Callout::Inventory::CALLOUT_INVENTORY_PATH,
                std::string>>(
                ConfigPropertyIndicies::get()[9]),
            std::make_unique<ElogWithMetadataCapture<
                sdbusplus::xyz::openbmc_project::Common::Callout::Error::Inventory,
                phosphor::logging::xyz::openbmc_project::Common::Callout::Inventory::CALLOUT_INVENTORY_PATH,
                int64_t>>(
                ConfigPropertyIndicies::get()[7]),
            makeMethod<SDBusPlus>(
                ConfigInterfaces::get()[3],
                ConfigPaths::get()[6],
                ConfigInterfaces::get()[3],
                ConfigProperties::get()[3],
                "123"s,
                false,
                true,
                static_cast<uint8_t>(123),
                static_cast<uint16_t>(123ull),
                static_cast<uint32_t>(123ull),
                static_cast<uint64_t>(123ull),
                static_cast<int16_t>(123ll),
                static_cast<int32_t>(123ll),
                static_cast<int64_t>(123ll)),
            std::make_unique<ResolveCallout>("/test/inventory/path"),
            std::make_unique<ConditionalCallback<ConfigPropertyCallbacks>>(
                ConfigPropertyCallbackGroups::get()[2],
                *ConfigConditions::get()[2]),
            std::make_unique<DeferrableCallback<ConfigPropertyCallbacks, SDEventTimer>>(
                ConfigPropertyCallbackGroups::get()[2],
                *ConfigConditions::get()[3],
                1000us),
            std::make_unique<DeferrableCallback<ConfigPropertyCallbacks, SDEventTimer>>(
                ConfigPropertyCallbackGroups::get()[2],
                *ConfigConditions::get()[4],
                1000us),
            std::make_unique<DeferrableCallback<ConfigPropertyCallbacks, SDEventTimer>>(
                ConfigPropertyCallbackGroups::get()[2],
                *ConfigConditions::get()[5],
                1000us),
            std::make_unique<DeferrableCallback<ConfigPropertyCallbacks, SDEventTimer>>(
                ConfigPropertyCallbackGroups::get()[2],
                *ConfigConditions::get()[6],
                1000us),
            std::make_unique<DeferrableCallback<ConfigPropertyCallbacks, SDEventTimer>>(
                ConfigPropertyCallbackGroups::get()[2],
                *ConfigConditions::get()[7],
                1000us),
            std::make_unique<DeferrableCallback<ConfigPropertyCallbacks, SDEventTimer>>(
                ConfigPropertyCallbackGroups::get()[2],
                *ConfigConditions::get()[8],
                1000us),
            std::make_unique<DeferrableCallback<ConfigPropertyCallbacks, SDEventTimer>>(
                ConfigPropertyCallbackGroups::get()[2],
                *ConfigConditions::get()[9],
                1000us),
            std::make_unique<DeferrableCallback<ConfigPropertyCallbacks, SDEventTimer>>(
                ConfigPropertyCallbackGroups::get()[2],
                *ConfigConditions::get()[10],
                1000us),
            std::make_unique<DeferrableCallback<ConfigPropertyCallbacks, SDEventTimer>>(
                ConfigPropertyCallbackGroups::get()[2],
                *ConfigConditions::get()[11],
                1000us),
            std::make_unique<DeferrableCallback<ConfigPropertyCallbacks, SDEventTimer>>(
                ConfigPropertyCallbackGroups::get()[2],
                *ConfigConditions::get()[12],
                1000us),
            std::make_unique<DeferrableCallback<ConfigPropertyCallbacks, SDEventTimer>>(
                ConfigPropertyCallbackGroups::get()[2],
                *ConfigConditions::get()[13],
                1000us),
            std::make_unique<DeferrableCallback<ConfigPropertyCallbacks, SDEventTimer>>(
                ConfigPropertyCallbackGroups::get()[2],
                *ConfigConditions::get()[14],
                1000us),
        };
        return propertyCallbacks;
    }
};

struct ConfigPathCallbacks
{
    using Callbacks = std::array<std::unique_ptr<Callback>, 1>;

    static auto& get()
    {
        static const Callbacks pathCallbacks =
        {
            std::make_unique<SNMPEvent>("error"s, "New error entry is created."s),
        };
        return pathCallbacks;
    }
};

struct ConfigPropertyWatches
{
    using PropertyWatches = std::array<std::unique_ptr<Watch>, 10>;

    static auto& get()
    {
        static const PropertyWatches propertyWatches =
        {
            std::make_unique<PropertyWatchOfType<uint32_t, SDBusPlus>>(
                ConfigPropertyIndicies::get()[0],
                *ConfigPropertyCallbacks::get()[7]),
            std::make_unique<PropertyWatchOfType<uint8_t, SDBusPlus>>(
                ConfigPropertyIndicies::get()[1]),
            std::make_unique<PropertyWatchOfType<uint16_t, SDBusPlus>>(
                ConfigPropertyIndicies::get()[2]),
            std::make_unique<PropertyWatchOfType<uint32_t, SDBusPlus>>(
                ConfigPropertyIndicies::get()[3]),
            std::make_unique<PropertyWatchOfType<uint64_t, SDBusPlus>>(
                ConfigPropertyIndicies::get()[4]),
            std::make_unique<PropertyWatchOfType<int16_t, SDBusPlus>>(
                ConfigPropertyIndicies::get()[5]),
            std::make_unique<PropertyWatchOfType<int32_t, SDBusPlus>>(
                ConfigPropertyIndicies::get()[6]),
            std::make_unique<PropertyWatchOfType<int64_t, SDBusPlus>>(
                ConfigPropertyIndicies::get()[7]),
            std::make_unique<PropertyWatchOfType<bool, SDBusPlus>>(
                ConfigPropertyIndicies::get()[8]),
            std::make_unique<PropertyWatchOfType<std::string, SDBusPlus>>(
                ConfigPropertyIndicies::get()[9]),
        };
        return propertyWatches;
    }
};

struct ConfigPathWatches
{
    using PathWatches = std::array<std::unique_ptr<Watch>, 1>;

    static auto& get()
    {
        static const PathWatches pathWatches =
        {
            std::make_unique<PathWatch<SDBusPlus>>(
                ConfigIntfAddPaths::get()[0],
                *ConfigPathCallbacks::get()[0]),
        };
        return pathWatches;
    }
};
} // namespace monitoring
} // namespace dbus
} // namespace phosphor
