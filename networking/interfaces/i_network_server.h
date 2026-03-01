#ifndef NETWORKING_INTERFACES_I_NETWORK_SERVER
#define NETWORKING_INTERFACES_I_NETWORK_SERVER

namespace game::networking {
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
};

}  // namespace game::networking

#endif  // NETWORKING_INTERFACES_I_NETWORK_SERVER
