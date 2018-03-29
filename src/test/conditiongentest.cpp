#include <array>
#include <string>
#include <gtest/gtest.h>
#include "data_types.hpp"

using namespace phosphor::dbus::monitoring;

#include "conditiongentest.hpp"

const std::array<std::vector<size_t>, 2> expectedGroups = {{
    {0},
    {1},
}};

const std::array<size_t, 4> expectedCallbacks = {
    0,
    0,
    1,
    1,
};

TEST(ConditionGenTest, GroupsSameSize)
{
    ASSERT_EQ(sizeof(expectedGroups), sizeof(groups));
}

TEST(ConditionGenTest, CallbacksSameSize)
{
    ASSERT_EQ(sizeof(expectedGroups), sizeof(groups));
}

TEST(ConditionGenTest, GroupsSameContent)
{
    size_t i;
    for (i = 0; i < expectedGroups.size(); ++i)
    {
        ASSERT_EQ(groups[i], expectedGroups[i]);
    }
}

TEST(ConditionGenTest, CallbacksSameContent)
{
    size_t i;
    for (i = 0; i < expectedCallbacks.size(); ++i)
    {
        ASSERT_EQ(callbacks[i], expectedCallbacks[i]);
    }
}
