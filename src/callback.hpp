#pragma once

#include "data_types.hpp"

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

/** @class Callback
 *  @brief Callback interface.
 *
 *  Callbacks of any type can be run.
 */
class Callback
{
    public:
        Callback() = default;
        Callback(const Callback&) = delete;
        Callback(Callback&&) = default;
        Callback& operator=(const Callback&) = delete;
        Callback& operator=(Callback&&) = default;
        virtual ~Callback() = default;

        /** @brief Run the callback. */
        virtual void operator()() = 0;
};

/** @class Conditional
 *  @brief Condition interface.
 *
 *  Conditions of any type can be tested for true or false.
 */
class Conditional
{
    public:
        Conditional() = default;
        Conditional(const Conditional&) = delete;
        Conditional(Conditional&&) = default;
        Conditional& operator=(const Conditional&) = delete;
        Conditional& operator=(Conditional&&) = default;
        virtual ~Conditional() = default;

        /** @brief Test the condition. */
        virtual bool operator()() = 0;
};

/** @class IndexedConditional
 *  @brief Condition with an index.
 */
class IndexedConditional : public Conditional
{
    public:
        IndexedConditional() = delete;
        IndexedConditional(const IndexedConditional&) = delete;
        IndexedConditional(IndexedConditional&&) = default;
        IndexedConditional& operator=(const IndexedConditional&) = delete;
        IndexedConditional& operator=(IndexedConditional&&) = default;
        virtual ~IndexedConditional() = default;

        explicit IndexedConditional(const PropertyIndex& conditionIndex)
            : Conditional(), index(conditionIndex) {}

        /** @brief Test the condition. */
        virtual bool operator()() override = 0;

    protected:

        /** @brief Property names and their associated storage. */
        const PropertyIndex& index;
};

/** @class IndexedCallback
 *  @brief Callback with an index.
 */
class IndexedCallback : public Callback
{
    public:
        IndexedCallback() = delete;
        IndexedCallback(const IndexedCallback&) = delete;
        IndexedCallback(IndexedCallback&&) = default;
        IndexedCallback& operator=(const IndexedCallback&) = delete;
        IndexedCallback& operator=(IndexedCallback&&) = default;
        virtual ~IndexedCallback() = default;
        explicit IndexedCallback(const PropertyIndex& callbackIndex)
            : Callback(), index(callbackIndex) {}

        /** @brief Run the callback. */
        virtual void operator()() override = 0;

    protected:

        /** @brief Property names and their associated storage. */
        const PropertyIndex& index;
};

/** @class GroupOfCallbacks
 *  @brief Invoke multiple callbacks.
 *
 *  A group of callbacks is implemented as a vector of array indicies
 *  into an external array  of callbacks.  The group function call
 *  operator traverses the vector of indicies, invoking each
 *  callback.
 *
 *  @tparam CallbackAccess - Access to the array of callbacks.
 */
template <typename CallbackAccess>
class GroupOfCallbacks : public Callback
{
    public:
        GroupOfCallbacks() = delete;
        GroupOfCallbacks(const GroupOfCallbacks&) = delete;
        GroupOfCallbacks(GroupOfCallbacks&&) = default;
        GroupOfCallbacks& operator=(const GroupOfCallbacks&) = delete;
        GroupOfCallbacks& operator=(GroupOfCallbacks&&) = default;
        ~GroupOfCallbacks() = default;
        explicit GroupOfCallbacks(
            const std::vector<size_t>& graphEntry)
            : graph(graphEntry) {}

        /** @brief Run the callbacks. */
        void operator()() override
        {
            for (auto e : graph)
            {
                (*CallbackAccess::get()[e])();
            }
        }

    private:
        /** @brief The offsets of the callbacks in the group. */
        const std::vector<size_t>& graph;
};

/** @class ConditionalCallback
 *  @brief Callback adaptor that asssociates a condition with a callback.
 */
template <typename CallbackAccess>
class ConditionalCallback: public Callback
{
    public:
        ConditionalCallback() = delete;
        ConditionalCallback(const ConditionalCallback&) = delete;
        ConditionalCallback(ConditionalCallback&&) = default;
        ConditionalCallback& operator=(const ConditionalCallback&) = delete;
        ConditionalCallback& operator=(ConditionalCallback&&) = default;
        ~ConditionalCallback() = default;
        ConditionalCallback(
            const std::vector<size_t>& graphEntry,
            Conditional& cond)
            : graph(graphEntry), condition(cond) {}

        /** @brief Run the callback if the condition is satisfied. */
        void operator()() override
        {
            if (condition())
            {
                (*CallbackAccess::get()[graph[0]])();
            }
        }

    private:
        /** @brief The index of the callback to conditionally invoke. */
        const std::vector<size_t>& graph;

        /** @brief The condition to test. */
        Conditional& condition;
};

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
