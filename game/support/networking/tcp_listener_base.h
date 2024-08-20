#ifndef SUPPORT_NETWORKING_TCP_LISTENER_BASE_H
#define SUPPORT_NETWORKING_TCP_LISTENER_BASE_H

#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include "interfaces/i_network_server.h"
#include "support/networking/net_utils.hpp"

namespace game::support
{

/**
 * \addtogroup support Support
 * \brief Supporting classes and functions
 * @{
 */

/**
 * \addtogroup networking Networking Utilities
 * \brief Utility classes and functions for network operations
 * \ingroup support
 * @{
 */

namespace net = boost::asio;
using tcp = net::ip::tcp;

/**
 * @brief A TCP server implementation.
 *
 * This class is a TCP server which uses boost::asio for network operations.
 * It implements the @see INetowrkServer interface and provides a default
 * implementation of the pure virtual functions defined in the interface.
 *
 * This class implements the tcp connection by initializing the related asio objects. When
 * a connection is established, it calls the OnAccept function to handle the connection.
 *
 * @note This class is not copyable or moveable.
 */
class TcpListenerBase : public INetworkServer
{
  public:
    TcpListenerBase(net::io_context& ioc, tcp::endpoint endpoint)
        : io_context_ref_(ioc), acceptor_(ioc)
    {
        boost::system::error_code ec;

        if (not InitializeAcceptor(acceptor_, endpoint, ec))
        {
            Fail(ec, "InitializeAcceptor");
            return;
        }
    }

    TcpListenerBase(const TcpListenerBase&) = delete;
    TcpListenerBase(TcpListenerBase&&) = delete;
    TcpListenerBase& operator=(const TcpListenerBase&) = delete;
    TcpListenerBase& operator=(TcpListenerBase&&) = delete;
    virtual ~TcpListenerBase() { Stop(); }

    void Start() override;

    void Stop() override;

  protected:
    /**
     * @brief Handle incoming connections.
     *
     * This function is called when a new connection is accepted. It will
     * create a new socket object and start reading from it.
     *
     * @param ec The error code from the accept operation.
     * @param socket The socket object for the new connection.
     */
    virtual void OnAccept(
        boost::system::error_code ec, boost::asio::ip::tcp::socket socket) = 0;

    /**
     * @brief Handle incoming data.
     *
     * This function is called when data is received from the client. It will
     * write the data back to the client.
     *
     * @param ec The error code from the read operation.
     * @param bytes_transferred The number of bytes transferred.
     */
    virtual void OnRead(boost::system::error_code ec, std::size_t bytes_transferred) {}

    /**
     * @brief Handle write completion.
     *
     * This function is called when a write operation completes. It will
     * start a new read operation.
     *
     * @param ec The error code from the write operation.
     * @param bytes_transferred The number of bytes transferred.
     */
    virtual void OnWrite(boost::system::error_code ec, std::size_t bytes_transferred) {}

    /**
     * @brief Handle close notifications.
     *
     * This function is called when a close notification is received from the
     * client. It will close the socket.
     *
     * @param ec The error code from the close operation.
     */
    virtual void OnClose() {}

    /**
     * @brief Handle errors.
     *
     * This function is called when an error occurs. It will log the error and
     * stop the server.
     *
     * @param ec The error code from the error.
     */
    void OnError(boost::system::error_code ec);

  private:
    void DoAccept();
    void OnAcceptContinue(boost::system::error_code ec, tcp::socket socket);

    /**
     * @brief The IO context used for all operations.
     */
    boost::asio::io_context& io_context_ref_;

    /**
     * @brief The acceptor used for accepting new connections.
     */
    boost::asio::ip::tcp::acceptor acceptor_;
};

/// @}
/// @}

}  // namespace game::support

#endif  // SUPPORT_NETWORKING_TCP_LISTENER_BASE_H
