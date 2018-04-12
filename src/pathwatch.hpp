/**
 * @file PathWatch.hpp
 * @brief Add interfaces added watch for the specified paths
 *
 * In general class users should include pathwatchimpl.hpp instead to avoid
 * link failures.
 */
#pragma once

#include "data_types.hpp"
#include "watch.hpp"

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

class Callback;

/** @class PathWatch
 *  @brief Type agnostic, factored out logic for Interface watches.
 */
template <typename DBusInterfaceType> class PathWatch : public Watch
{
  public:
    PathWatch() = delete;
    PathWatch(const PathWatch&) = delete;
    PathWatch(PathWatch&&) = default;
    PathWatch& operator=(const PathWatch&) = delete;
    PathWatch& operator=(PathWatch&&) = default;
    virtual ~PathWatch() = default;
    PathWatch(const std::string& path, Callback& callback) :
        Watch(), objectPath(path), cb(&callback), alreadyRan(false)
    {
    }

    /** @brief Start the watch.
     *
     *  Watch start interface implementation for PathWatch.
     */
    void start() override;

    /** @brief Run the watch callback method.
     *
     *  Watch callback interface implementation for PathWatch.
     */
    void callback(Context ctx) override;

    /** @brief Run the watch callback method.
     *
     *  Watch callback interface implementation for PathWatch.
     */
    void callback(Context ctx, sdbusplus::message::message& msg) override;

    /** @brief Dbus signal callback for InterfacesAdded.
     *
     *  @param[in] msg - interfaces added message.
     */
    void interfacesAdded(sdbusplus::message::message& msg);

  protected:
    /** @brief Path of the D-Bus object to watch for. */
    const std::string& objectPath;

    /** @brief Optional callback method. */
    Callback* const cb;

    /** @brief The start method should only be invoked once. */
    bool alreadyRan;
};

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
