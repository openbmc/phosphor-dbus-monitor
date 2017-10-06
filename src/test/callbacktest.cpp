#include <gtest/gtest.h>

#include "generated.hpp"
#include "event_manager.hpp"

using namespace phosphor::dbus::monitoring;

std::unique_ptr<phosphor::events::Manager> manager = nullptr;

TEST(JournalTest, Test)
{
    // No assertions here, but the least we can do
    // make sure the program runs without crashing...
    for (auto& c : ConfigPropertyCallbacks::get())
    {
        (*c)();
    }
}
