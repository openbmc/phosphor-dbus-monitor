/**
 * @file PathWatch.hpp
 * @brief Add watch for the object path for interfaces added/removed signal
 *
 * In general class users should include pathwatchimpl.hpp instead to avoid
 * link failures.
 */
#pragma once

#include "data_types.hpp"
#include "watch.hpp"

#include <string>

namespace phosphor
{
namespace dbus
{
namespace monitoring
{

enum class ObjectManagerEvent
{
    interfacesAdded,
    interfacesRemoved,
};

class Callback;

/** @class PathWatch
 *  @brief Watch on object path for interfaceadded/interfaceremoved signals
 */
template <typename DBusInterfaceType>
class PathWatch : public Watch
{
  public:
    PathWatch() = delete;
    PathWatch(const PathWatch&) = delete;
    PathWatch(PathWatch&&) = default;
    PathWatch& operator=(const PathWatch&) = delete;
    PathWatch& operator=(PathWatch&&) = default;
    virtual ~PathWatch() = default;
    PathWatch(const RefVector<const std::string>& paths, Callback& callback,
              ObjectManagerEvent event = ObjectManagerEvent::interfacesAdded,
              bool match_object_path = false) :
        Watch(), paths(paths), cb(callback), alreadyRan(false), event(event),
        match_object_path(match_object_path)
    {}

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
    void callback(Context ctx, sdbusplus::message_t& msg) override;

  protected:
    /** @brief Path of the D-Bus object to watch for. */
    const RefVector<const std::string>& paths;

    /** @brief Optional callback method. */
    Callback& cb;

    /** @brief The start method should only be invoked once. */
    bool alreadyRan;

    /** @brief Monitor Event */
    const ObjectManagerEvent event;

    /** @brief match sender path or object path. */
    bool match_object_path;
};

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
