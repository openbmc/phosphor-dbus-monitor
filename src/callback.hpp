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

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
