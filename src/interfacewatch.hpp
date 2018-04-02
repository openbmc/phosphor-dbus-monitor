/**
 * @file interfacewatch.hpp
 * @brief Add interfaces added watch for the specified path
 *
 * In general class users should include interfacewatchimpl.hpp instead to avoid
 * link failures.
 */
#pragma once

#include "data_types.hpp"
#include "watch.hpp"
#include <iostream>
namespace phosphor
{
namespace dbus
{
namespace monitoring
{

class Callback;

/** @class InterfaceWatch
 *  @brief Type agnostic, factored out logic for Interface watches.
 */
template <typename DBusInterfaceType> class InterfaceWatch : public Watch
{
  public:
    InterfaceWatch() = delete;
    InterfaceWatch(const InterfaceWatch&) = delete;
    InterfaceWatch(InterfaceWatch&&) = default;
    InterfaceWatch& operator=(const InterfaceWatch&) = delete;
    InterfaceWatch& operator=(InterfaceWatch&&) = default;
    virtual ~InterfaceWatch() = default;
    InterfaceWatch(const std::string& path, Callback* callback = nullptr) :
        Watch(),
        objPath(path), cb(callback), alreadyRan(false)
    {
    }

    /** @brief Start the watch.
     *
     *  Watch start interface implementation for InterfaceWatch.
     */
    void start() override;

    /** @brief Run the watch callback method.
     *
     *  Watch callback interface implementation for InterfaceWatch.
     */
    void callback(Context ctx) override;

    /** @brief Dbus signal callback for InterfacesAdded.
     *
     *  @param[in] msg - interfaces added message.
     */
    void interfacesAdded( sdbusplus::message::message& msg);

  protected:

    /** @brief Path to watch for. */
    const std::string objPath;

    /** @brief Optional callback method. */
    Callback* const cb;

    /** @brief The start method should only be invoked once. */
    bool alreadyRan;
};

} // namespace monitoring
} // namespace dbus
} // namespace phosphor
