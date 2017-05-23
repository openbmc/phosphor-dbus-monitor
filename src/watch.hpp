#pragma once

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

/** @class Watch
 *  @brief Watch interface.
 *
 *  The start method is invoked by main() on all watches of any type
 *  at application startup, to allow watches to perform custom setup
 *  or initialization.  Typical implementations might register dbus
 *  callbacks or perform queries.
 *
 *  Watches of any type can be started.
 */
class Watch
{
    public:
        Watch() = default;
        Watch(const Watch&) = default;
        Watch(Watch&&) = default;
        Watch& operator=(const Watch&) = default;
        Watch& operator=(Watch&&) = default;
        virtual ~Watch() = default;

        /** @brief Start the watch. */
        virtual void start() = 0;
};

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
