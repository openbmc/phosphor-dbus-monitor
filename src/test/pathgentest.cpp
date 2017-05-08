#include <array>
#include <string>
#include <gtest/gtest.h>
#include "data_types.hpp"

using namespace std::string_literals;
using namespace phosphor::dbus::monitoring;
using PathMeta = TupleOfRefs<const std::string, const std::string>;

#include "pathgentest.hpp"

const std::array<std::string, 3> expectedMeta =
{
    "path1"s,
    "path3"s,
    "path2"s,
};

const std::array<std::string, 6> expectedPaths =
{
    "/xyz/openbmc_project/testing/inst1"s,
    "/xyz/openbmc_project/testing/inst2"s,
    "/xyz/openbmc_project/testing/inst3"s,
    "/xyz/openbmc_project/testing/inst4"s,
    "/xyz/openbmc_project/testing/inst5"s,
    "/xyz/openbmc_project/testing/inst6"s,
};

const std::array<PathMeta, 14> expectedPathMeta =
{
    {
        { paths[0], meta[0] },
        { paths[1], meta[0] },
        { paths[2], meta[0] },
        { paths[3], meta[0] },
        { paths[0], meta[1] },
        { paths[1], meta[1] },
        { paths[2], meta[1] },
        { paths[3], meta[1] },
        { paths[4], meta[0] },
        { paths[5], meta[0] },
        { paths[3], meta[2] },
        { paths[2], meta[2] },
        { paths[1], meta[2] },
        { paths[0], meta[2] },
    }
};

const std::array<RefVector<const std::string>, 4> expectedGroups =
{
    {
        {
            paths[0],
            paths[1],
            paths[2],
            paths[3],
        },
        {
            paths[0],
            paths[1],
            paths[2],
            paths[3],
        },
        {
            paths[0],
            paths[1],
            paths[4],
            paths[5],
        },
        {
            paths[3],
            paths[2],
            paths[1],
            paths[0],
        },
    }
};

TEST(PathGenTest, MetaSameSize)
{
    ASSERT_EQ(sizeof(expectedMeta), sizeof(meta));
}

TEST(PathGenTest, PathsSameSize)
{
    ASSERT_EQ(sizeof(expectedPaths), sizeof(paths));
}

TEST(PathGenTest, GroupsSameSize)
{
    ASSERT_EQ(sizeof(expectedGroups), sizeof(groups));
}

TEST(PathGenTest, MetaSameContent)
{
    size_t i;
    for (i = 0; i < expectedMeta.size(); ++i)
    {
        ASSERT_EQ(meta[i], expectedMeta[i]);
    }
}

TEST(PathGenTest, PathsSameContent)
{
    size_t i;
    for (i = 0; i < expectedPaths.size(); ++i)
    {
        ASSERT_EQ(paths[i], expectedPaths[i]);
    }
}

TEST(PathGenTest, GroupsSameContent)
{
    size_t i;
    for (i = 0; i < expectedGroups.size(); ++i)
    {
        size_t j;
        for (j = 0; j < groups[i].size(); ++j)
        {
            ASSERT_EQ(groups[i][j].get(), expectedGroups[i][j].get());

        }
    }
}
