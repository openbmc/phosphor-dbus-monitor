#pragma once

#include "callback.hpp"
#include "data_types.hpp"

#include <algorithm>

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

/** @class CountCondition
 *  @brief Count properties that satisfy a condition.
 *
 *  When invoked, a count class instance performs its condition
 *  test in two passes.
 *
 *  In pass one, apply a C++ relational operator to the value of
 *  each property in the index and a value provided by the
 *  configuration file.
 *
 *  Count the number of properties that pass the test in pass
 *  one.  In pass two, apply a second C++ relational operator
 *  to the number of properties that pass the test from pass one
 *  to a count provided by the configuration file.
 */
template <typename T>
class CountCondition : public IndexedConditional
{
    public:
        CountCondition() = delete;
        CountCondition(const CountCondition&) = default;
        CountCondition(CountCondition&&) = default;
        CountCondition& operator=(const CountCondition&) = default;
        CountCondition& operator=(CountCondition&&) = default;
        ~CountCondition() = default;

        CountCondition(
            const PropertyIndex& conditionIndex,
            const std::function<bool(size_t)>& _countOp,
            const std::function<bool(T)>& _propertyOp) :
            IndexedConditional(conditionIndex),
            countOp(_countOp),
            propertyOp(_propertyOp) {}

        bool operator()() override
        {
            // Count the number of properties in the index that
            // pass the condition specified in the config file.
            auto count = std::count_if(
                             index.cbegin(),
                             index.cend(),
                             [this](const auto & item)
            // *INDENT-OFF*
                             {
                                 const auto& storage = std::get<2>(
                                     item.second);
                                 // Don't count properties that don't exist.
                                 if (storage.get().empty())
                                 {
                                     return false;
                                 }
                                 const auto& value = any_ns::any_cast<T>(
                                     storage);
                                 return propertyOp(value);
                             });
            // *INDENT-ON*

            // Now apply the count condition to the count.
            return countOp(count);
        }

    private:
        /** @brief The comparison to perform on the count. */
        std::function<bool(size_t)> countOp;
        /** @brief The comparison to perform on each property. */
        std::function<bool(T)> propertyOp;
};
} // namespace monitoring
} // namespace dbus
} // namespace phosphor
