#include "propertywatchtest.hpp"

#include "propertywatchimpl.hpp"

#include <array>

using namespace std::string_literals;
using namespace phosphor::dbus::monitoring;

const std::array<std::string, 4> paths = {
    "/xyz/openbmc_project/testing/inst1"s,
    "/xyz/openbmc_project/testing/inst2"s,
    "/xyz/openbmc_project/testing/inst3"s,
    "/xyz/openbmc_project/testing/inst4"s,
};

const std::array<std::string, 2> interfaces = {
    "xyz.openbmc_project.Iface1"s,
    "xyz.openbmc_project.Iface2"s,
};

const std::array<std::string, 2> properties = {
    "Value1"s,
    "Value2"s,
};

const std::string meta;

std::array<std::tuple<any_ns::any, any_ns::any>, 8> storage = {};

const PropertyIndex watchIndex = {
    {
        {PropertyIndex::key_type{paths[0], interfaces[0], properties[0]},
         PropertyIndex::mapped_type{meta, meta, storage[0]}},
        {PropertyIndex::key_type{paths[0], interfaces[1], properties[1]},
         PropertyIndex::mapped_type{meta, meta, storage[1]}},
        {PropertyIndex::key_type{paths[1], interfaces[0], properties[0]},
         PropertyIndex::mapped_type{meta, meta, storage[2]}},
        {PropertyIndex::key_type{paths[1], interfaces[1], properties[1]},
         PropertyIndex::mapped_type{meta, meta, storage[3]}},
        {PropertyIndex::key_type{paths[2], interfaces[0], properties[0]},
         PropertyIndex::mapped_type{meta, meta, storage[4]}},
        {PropertyIndex::key_type{paths[2], interfaces[1], properties[1]},
         PropertyIndex::mapped_type{meta, meta, storage[5]}},
        {PropertyIndex::key_type{paths[3], interfaces[0], properties[0]},
         PropertyIndex::mapped_type{meta, meta, storage[6]}},
        {PropertyIndex::key_type{paths[3], interfaces[1], properties[1]},
         PropertyIndex::mapped_type{meta, meta, storage[7]}},
    },
};

template <typename T>
struct ExpectedValues
{
};
template <>
struct ExpectedValues<uint8_t>
{
    static auto& get(size_t i)
    {
        static const std::array<uint8_t, 8> values = {
            {0, 1, 2, 3, 4, 5, 6, 7},
        };
        return values[i];
    }
};

template <>
struct ExpectedValues<uint16_t>
{
    static auto& get(size_t i)
    {
        static const std::array<uint16_t, 8> values = {
            {88, 77, 66, 55, 44, 33, 22, 11},
        };
        return values[i];
    }
};

template <>
struct ExpectedValues<uint32_t>
{
    static auto& get(size_t i)
    {
        static const std::array<uint32_t, 8> values = {
            {0xffffffff, 1, 3, 0, 5, 7, 9, 0xffffffff},
        };
        return values[i];
    }
};

template <>
struct ExpectedValues<uint64_t>
{
    static auto& get(size_t i)
    {
        static const std::array<uint64_t, 8> values = {
            {0xffffffffffffffff, 3, 7, 12234, 0, 3, 9, 0xffffffff},
        };
        return values[i];
    }
};

template <>
struct ExpectedValues<std::string>
{
    static auto& get(size_t i)
    {
        static const std::array<std::string, 8> values = {
            {""s, "foo"s, "bar"s, "baz"s, "hello"s, "string", "\x2\x3", "\\"},
        };
        return values[i];
    }
};

template <typename T>
void testStart()
{
    using ::testing::_;
    using ::testing::Return;

    MockDBusInterface dbus;
    MockDBusInterface::instance(dbus);

    const std::vector<std::string> expectedMapperInterfaces;
    PropertyWatchOfType<T, MockDBusInterface> watch(watchIndex);

    auto ndx = static_cast<size_t>(0);
    for (const auto& o : convert(watchIndex))
    {
        const auto& path = o.first.get();
        const auto& interfaces = o.second;
        std::vector<std::string> mapperResponse;
        std::transform(interfaces.begin(), interfaces.end(),
                       std::back_inserter(mapperResponse),
                       // *INDENT-OFF*
                       [](const auto& item) { return item.first; });
        // *INDENT-ON*
        EXPECT_CALL(dbus, mapperGetObject(MAPPER_BUSNAME, MAPPER_PATH,
                                          MAPPER_INTERFACE, "GetObject", path,
                                          expectedMapperInterfaces))
            .WillOnce(Return(GetObject({{"", mapperResponse}})));
        EXPECT_CALL(
            dbus, fwdAddMatch(
                      sdbusplus::bus::match::rules::interfacesAdded(path),
                      _));
        for (const auto& i : interfaces)
        {
            const auto& interface = i.first.get();
            const auto& properties = i.second;
            EXPECT_CALL(
                dbus,
                fwdAddMatch(
                    sdbusplus::bus::match::rules::propertiesChanged(
                        path, interface),
                    _));

            PropertiesChanged<T> serviceResponse;
            for (const auto& p : properties)
            {
                serviceResponse[p] = ExpectedValues<T>::get(ndx);
                ++ndx;
            }
            Expect<T>::getProperties(dbus, path, interface)
                .WillOnce(Return(serviceResponse));
        }
    }

    watch.start();

    ndx = 0;
    for (auto s : storage)
    {
        ASSERT_EQ(std::get<valueIndex>(s).empty(), false);
        ASSERT_EQ(
            any_ns::any_cast<T>(std::get<valueIndex>(s)),
            ExpectedValues<T>::get(ndx));
        ++ndx;
    }

    // Make sure start logic only runs the first time.
    watch.start();
}

TEST(PropertyWatchTest, TestStart)
{
    testStart<uint8_t>();
    testStart<uint16_t>();
    testStart<uint32_t>();
    testStart<uint64_t>();
    testStart<std::string>();
}

MockDBusInterface* MockDBusInterface::ptr = nullptr;
