#pragma once
#include "callback.hpp"

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

/**
 * @class ResolveErrorsBase
 * @brief Error log resolving callback implementation
 *
 * These classes will resolve errors (set Resolved property to true),
 * based on certain conditions.
 */
class ResolveErrorsBase : public Callback
{
    public:

        ResolveErrorsBase() = default;
        ~ResolveErrorsBase() = default;
        ResolveErrorsBase(const ResolveErrorsBase&) = delete;
        ResolveErrorsBase& operator=(const ResolveErrorsBase&) = delete;
        ResolveErrorsBase(ResolveErrorsBase&&) = default;
        ResolveErrorsBase& operator=(ResolveErrorsBase&&) = default;

        /**
         * @brief Callback interface implementation.
         */
        virtual void operator()() override = 0;
};

/**
 * @class ResolveCallout
 * @brief Resolves error logs with the associated callout
 */
class ResolveCallout : public ResolveErrorsBase
{
    public:

        ResolveCallout() = delete;
        ~ResolveCallout() = default;
        ResolveCallout(const ResolveCallout&) = delete;
        ResolveCallout& operator=(const ResolveCallout&) = delete;
        ResolveCallout(ResolveCallout&&) = default;
        ResolveCallout& operator=(ResolveCallout&&) = default;

        /**
         * @brief constructor
         *
         * @param[in] callout - The callout whose errors need to be resolved.
         *                      Normally an inventory path.
         */
        ResolveCallout(const std::string& callout) :
                ResolveErrorsBase(),
                callout(callout) {}

        /**
         * @brief Callback interface to resolve errors
         *
         * Resolves all error log entries that are associated
         * with the callout.
         */
        void operator()() override;

    private:

        /**
         * @brief Resolves a single error log entry
         *
         * param[in] entry - the object path of the error log entry
         */
        void resolve(const std::string& entry);

        /**
         * @brief The inventory path of the callout
         */
        std::string callout;
};


} // namespace monitoring
} // namespace dbus
} // namespace phosphor
