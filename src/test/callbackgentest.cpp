#include <array>
#include <string>
#include <gtest/gtest.h>
#include "data_types.hpp"

using namespace phosphor::dbus::monitoring;

using Index = std::map<std::tuple<size_t, size_t, size_t>, size_t>;

#include "callbackgentest.hpp"

const std::array<std::tuple<std::string, size_t>, 4> expectedCallbacks =
{
    {
        {"int32_t", 0},
        {"int32_t", 0},
        {"std::string", 1},
        {"std::string", 2},
    }
};

TEST(CallbackGenTest, CallbacksSameSize)
{
    ASSERT_EQ(sizeof(expectedCallbacks), sizeof(callbacks));
}

TEST(CallbackGenTest, CallbacksSameContent)
{
    size_t i;
    for (i = 0; i < expectedCallbacks.size(); ++i)
    {
        ASSERT_EQ(callbacks[i],
                  expectedCallbacks[i]);
    }
}
