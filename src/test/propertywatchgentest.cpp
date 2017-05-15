#include <array>
#include <string>
#include <gtest/gtest.h>
#include "data_types.hpp"

using namespace std::string_literals;
using namespace phosphor::dbus::monitoring;

using Index = std::map<std::tuple<size_t, size_t, size_t>, size_t>;

#include "propertywatchgentest.hpp"

auto expectedStorageCount = 16;

const std::array<Index, 4> expectedIndicies =
{
    {
        {
            {{0, 0, 0}, 0},
            {{0, 1, 0}, 1},
            {{1, 0, 0}, 2},
            {{1, 1, 0}, 3},
            {{2, 0, 0}, 4},
            {{2, 1, 0}, 5},
            {{3, 0, 0}, 6},
            {{3, 1, 0}, 7},
        },
        {
            {{2, 2, 1}, 8},
            {{2, 2, 2}, 9},
            {{3, 2, 1}, 10},
            {{3, 2, 2}, 11},
            {{4, 2, 1}, 12},
            {{4, 2, 2}, 13},
            {{5, 2, 1}, 14},
            {{5, 2, 2}, 15},
        },
        {
            {{3, 0, 0}, 6},
        },
        {
            {{3, 2, 2}, 11},
            {{5, 2, 2}, 15},
        },
    }
};

const std::array<std::tuple<std::string, size_t>, 4> expectedWatches =
{
    {
        {"std::string"s, 0},
        {"uint32_t"s, 1},
        {"int32_t"s, 2},
        {"std::string"s, 3},
    }
};

TEST(PropertyWatchGenTest, storageCount)
{
    ASSERT_EQ(expectedStorageCount, storageCount);
}

TEST(PropertyWatchGenTest, IndiciesSameSize)
{
    ASSERT_EQ(sizeof(expectedIndicies), sizeof(indicies));
}

TEST(PropertyWatchGenTest, WatchesSameSize)
{
    ASSERT_EQ(sizeof(expectedWatches), sizeof(watches));
}

TEST(PropertyWatchGenTest, WatchesSameContent)
{
    size_t i;
    for (i = 0; i < expectedWatches.size(); ++i)
    {
        ASSERT_EQ(watches[i],
                  expectedWatches[i]);
    }
}

TEST(PropertyWatchGenTest, IndiciesSameContent)
{
    size_t i;
    for (i = 0; i < expectedIndicies.size(); ++i)
    {
        ASSERT_EQ(indicies[i],
                  expectedIndicies[i]);
    }
}
