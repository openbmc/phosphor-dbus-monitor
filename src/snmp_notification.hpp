#include <string>
#include <iostream>
#pragma once
namespace phosphor
{
namespace snmp
{

/* @brief sends the trap to the snmp manager
 * T - Notification type
 * @param[in] tArgs - arguments for the trap.
 */
template <typename T, typename... ArgTypes> void sendTrap(ArgTypes&&... tArgs)
{
    T obj(tArgs...);
    obj.sendTrap();
}

/** @class Notification
 *  @brief Notification interface.
 *
 *  Each subclass has to get its own notification type
 *  as well as the list of objects which is a part of
 *  that notification type.
 *
 *  This class implements the sendTrap function which
 *  send the list of objects defined by the specific notification
 *  to the configured SNMP manager.
 */

class Notification
{

  public:
    Notification() = default;
    Notification(const Notification&) = delete;
    Notification(Notification&&) = default;
    Notification& operator=(const Notification&) = delete;
    Notification& operator=(Notification&&) = default;
    virtual ~Notification() = default;

    /** @brief Send the snmp trap to the configured
     *          manager.
     */
    virtual void sendTrap() = 0;
};

/** @class ErrorNotification
 *  @brief subclass of Notification
 *
 *  A Error Notification represents the objects needed by the
 *  Error Object.
 */
class ErrorNotification : public Notification
{

  private:
    uint32_t errorID;
    uint64_t errorTimestamp;
    int errorSeverity;
    std::string errorMessage;

  public:
    ErrorNotification() = default;
    ErrorNotification(const ErrorNotification&) = delete;
    ErrorNotification(ErrorNotification&&) = default;
    ErrorNotification& operator=(const ErrorNotification&) = delete;
    ErrorNotification& operator=(ErrorNotification&&) = default;
    ~ErrorNotification() = default;

    ErrorNotification(uint32_t id, uint64_t ts, uint8_t sev, std::string msg) :
        errorID(id),
        errorTimestamp(ts), errorSeverity(sev), errorMessage(msg)
    {
    }

    void sendTrap() override
    {
        std::cout << "enter:sendTrap " << std::endl;
        std::cout << "enter:errorId" << errorID << std::endl;
        std::cout << "enter:errorTimestamp" << errorTimestamp << std::endl;
        std::cout << "enter:errorSeverity" << errorSeverity << std::endl;
        std::cout << "enter:errorMessage" << errorMessage << std::endl;
    }
};


} // namespce snmp
} // namespce phosphor
