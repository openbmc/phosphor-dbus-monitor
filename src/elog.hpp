#pragma once
#include <phosphor-logging/elog-errors.hpp>
#include <phosphor-logging/elog.hpp>
#include "callback.hpp"
#include <sdbusplus/exception.hpp>
#include <experimental/tuple>

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
        ElogBase(const ElogBase&) = delete;
        ElogBase(ElogBase&&) = default;
        ElogBase& operator=(const ElogBase&) = delete;
        ElogBase& operator=(ElogBase&&) = default;
        virtual ~ElogBase() = default;
        ElogBase() :
            Callback() {}

        /** @brief Callback interface implementation. */
        void operator()() override;

    private:
        /** @brief Delegate type specific calls to subclasses. */
        virtual void log() const = 0;
};

namespace detail
{

/** @class CallElog
 *  @brief Provide explicit call forwarding to phosphor::logging::report.
 *
 *  @tparam T - Error log type
 *  @tparam Args - Metadata fields types.
 */
template <typename T, typename ...Args>
struct CallElog
{
    static void op(Args&& ...args)
    {
        phosphor::logging::report<T>(std::forward<Args>(args)...);
    }
};

} // namespace detail

/** @class Elog
 *  @brief C++ type specific logic for the elog callback.
 *         The elog callback logs the elog and elog metadata.
 *
 *  @tparam T - Error log type
 *  @tparam Args - Metadata fields types.
 *  @param[in] arguments - Metadata fields to be added to the error log
 */
template <typename T, typename ...Args>
class Elog : public ElogBase
{
    public:
        Elog(const Elog&) = delete;
        Elog(Elog&&) = default;
        Elog& operator=(const Elog&) = delete;
        Elog& operator=(Elog&&) = default;
        ~Elog() = default;
        Elog(Args&& ... arguments) :
            ElogBase(), args(std::forward<Args>(arguments)...) {}

    private:
        /** @brief elog interface implementation. */
        void log() const override
        {
            std::experimental::apply(
                detail::CallElog<T, Args...>::op,
                std::tuple_cat(args));
        }
        std::tuple<Args...> args;

};

/** @brief Argument type deduction for constructing Elog instances.
 *
 *  @tparam T - Error log type
 *  @tparam Args - Metadata fields types.
 *  @param[in] arguments - Metadata fields to be added to the error log
 */
template <typename T, typename ...Args>
auto makeElog(Args&& ... arguments)
{
    return std::make_unique<Elog<T, Args...>>(
               std::forward<Args>(arguments)...);
}

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
