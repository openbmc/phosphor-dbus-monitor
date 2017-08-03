#pragma once
#include <phosphor-logging/elog-errors.hpp>
#include "callback.hpp"

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

/** @class ElogBase
 *  @brief Elog callback implementation.
 *
 *  The elog callback logs the elog and
 *  elog metadata.
 */
class ElogBase : public Callback
{
    public:
        ElogBase() = delete;
        ElogBase(const ElogBase&) = delete;
        ElogBase(ElogBase&&) = default;
        ElogBase& operator=(const ElogBase&) = delete;
        ElogBase& operator=(ElogBase&&) = default;
        virtual ~ElogBase() = default;
        ElogBase(const char* msg, const PropertyIndex& index) :
            Callback(){}

        /** @brief Callback interface implementation. */
        void operator()() override;

    private:
        /** @brief Delegate type specific calls to subclasses. */
        virtual void log() const = 0;
};


/** @class Elog
 *  @brief C++ type specific logic for the elog callback.
 *
 *  @tparam T - Error log type
 */
template <typename T>
class Elog : public ElogBase
{
    public:
        Elog() = delete;
        Elog(const Elog&) = delete;
        Elog(Elog&&) = default;
        Elog& operator=(const Elog&) = delete;
        Elog& operator=(Elog&&) = default;
        ~Elog() = default;
        Elog(const char* msg, const PropertyIndex& index) :
            ElogBase() {}

    private:
        /** @brief elog interface implementation. */
        void log() const override
        {
            using namespace phosphor::logging;
            commit<T>();
        }
};

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
