#ifndef SUPPORT_NETWORKING_INTERFACES_I_NETWORK_SERVER_H
#define SUPPORT_NETWORKING_INTERFACES_I_NETWORK_SERVER_H

#include <cstddef>

namespace game::support
{
/**
 * @brief The INetowrkServer interface.
 *
 * @details This interface is used by networking classes to define the interface
 * for network servers. The interface provides a way to start and stop the server
 * and to process messages.
 */
struct INetworkServer
{
    /**
     * @brief Starts the server.
     *
     * @details This function starts the server by calling the DoAccept() function.
     */
    virtual void Start() = 0;

    /**
     * @brief Stops the server.
     *
     * @details This function stops the server by cancelling all outstanding
     * operations on the acceptor.
     */
    virtual void Stop() = 0;

    /**
     * @brief The destructor.
     *
     * @details This is a pure virtual destructor to allow for derived classes to
     * have a proper destructor.
     */
    virtual ~INetworkServer() = 0;
};

}  // namespace game::support

#endif  // SUPPORT_NETWORKING_INTERFACES_I_NETWORK_SERVER_H
