#pragma once
#include <phosphor-logging/elog-errors.hpp>
#include <phosphor-logging/elog.hpp>
#include <phosphor-logging/log.hpp>
#include "callback.hpp"
#include <sdbusplus/exception.hpp>

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
            Callback(){}

        /** @brief Callback interface implementation. */
        void operator()() override;

    private:
        /** @brief Delegate type specific calls to subclasses. */
        virtual void log() const = 0;
};

/**
 * @brief Structure used by callers to indicate they want to use the last value
 *        put in the journal for input parameter.
*/
template <typename T>
struct prev_entry
{
    using type = T;
};


namespace details
{
/**
 * @brief Used to return the generated tuple for the error code meta data
 *
 * The prev_entry (above) and deduce_entry_type structures below are used
 * to verify at compile time the required parameters have been passed to
 * the elog interface and then to forward on the appropriate tuple to the
 * log interface.
 */
template <typename T>
struct deduce_entry_type
{

    using type = T;
    auto get() { return value._entry; }

    T value;
};

/**
 * @brief Used to return an empty tuple for prev_entry parameters
 *
 * This is done so we can still call the log() interface with the variable
 * arg parameters to elog.  The log() interface will simply ignore the empty
 * tuples which is what we want for prev_entry parameters.
 */
template <typename T>
struct deduce_entry_type<prev_entry<T>>
{
    using type = T;
    auto get() { return std::make_tuple(); }

    prev_entry<T> value;
};

/**
 * @brief Typedef for above structure usage
 */
template <typename T> using deduce_entry_type_t =
        typename deduce_entry_type<T>::type;

/**
 * @brief Used to map an sdbusplus error to a phosphor-logging error type
 *
 * Users log errors via the sdbusplus error name, and the execption that's
 * thrown is the corresponding sdbusplus exception. However, there's a need
 * to map the sdbusplus error name to the phosphor-logging error name, in order
 * to verify the error metadata at compile-time.
 */
template <typename T>
struct map_exception_type
{
    using type = T;
};

/**
 * @brief Typedef for above structure usage
 */
template <typename T> using map_exception_type_t =
    typename map_exception_type<T>::type;

} // namespace details


/** @class Elog
 *  @brief C++ type specific logic for the elog callback.
 *
 *  @tparam T - Error log type
 *  @param[in] i_args - Metadata fields to be added to the journal entry
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
        Elog(Args... i_args) :
            ElogBase(), args(std::forward<Args>(i_args)...){}

    private:
        /** @brief elog interface implementation. */
        void log() const override
        {
            using namespace phosphor::logging;
            report<T>(details::deduce_entry_type<Args>{args}.get()...);
        }
        std::tuple<details::deduce_entry_type_t<Args>...> args;
};

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
