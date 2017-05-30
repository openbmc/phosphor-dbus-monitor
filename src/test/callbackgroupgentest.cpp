#include <array>
#include <string>
#include <gtest/gtest.h>
#include "data_types.hpp"

using namespace phosphor::dbus::monitoring;

#include "callbackgroupgentest.hpp"

const std::array<std::vector<size_t>, 4> expectedGroups = {{
        {0, 1, 2},
        {0, 1},
        {2, 4, 5},
        {5},
    }
};

TEST(CallbackGroupGenTest, GroupsSameSize)
{
    ASSERT_EQ(sizeof(expectedGroups), sizeof(groups));
}

TEST(CallbackGroupGenTest, GroupsSameContent)
{
    size_t i;
    for (i = 0; i < expectedGroups.size(); ++i)
    {
        ASSERT_EQ(groups[i],
                  expectedGroups[i]);
    }
}