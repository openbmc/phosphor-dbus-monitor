#pragma once

#include "callback.hpp"
#include "data_types.hpp"

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

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
            auto count = std::count_if(
                             index.cbegin(),
                             index.cend(),
                             [this](const auto & item)
            // *INDENT-OFF*
                             {
                                 const auto& storage = std::get<2>(
                                     item.second);
                                 if (storage.get().empty())
                                 {
                                     return false;
                                 }
                                 const auto& value = any_ns::any_cast<T>(
                                     storage);
                                 return propertyOp(value);
                             });
            // *INDENT-ON*
            return countOp(count);
        }

    private:
        std::function<bool(size_t)> countOp;
        std::function<bool(T)> propertyOp;
};
} // namespace monitoring
} // namespace dbus
} // namespace phosphor
