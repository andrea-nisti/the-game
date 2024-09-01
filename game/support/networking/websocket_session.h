#ifndef SUPPORT_NETWORKING_WEBSOCKET_SESSION_H
#define SUPPORT_NETWORKING_WEBSOCKET_SESSION_H

#include <cstdlib>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/optional.hpp>
#include <boost/throw_exception.hpp>

#include "session_base.h"

namespace beast = boost::beast;          // from <boost/beast.hpp>
namespace websocket = beast::websocket;  // from <boost/beast/websocket.hpp>
namespace net = boost::asio;             // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;        // from <boost/asio/ip/tcp.hpp>

namespace game::support
{

/**
 * \addtogroup support Support
 * \brief
 * @{
 */

/**
 * \addtogroup networking Networking Utilities
 * \brief Utility classes and functions for network operations
 * \ingroup support
 * @{
 */

class WebSocketSession final
    : public SessionBase<websocket::stream<beast::tcp_stream>, beast::flat_buffer>,
      public std::enable_shared_from_this<WebSocketSession>
{
  public:
    WebSocketSession(tcp::socket&& socket)
        : SessionBase<websocket::stream<beast::tcp_stream>, beast::flat_buffer>(
              std::move(socket))
    {
    }

    virtual ~WebSocketSession() { Close(); }

    /**
     * @brief Starts the session and begins processing requests.
     */
    void Run() override;

    void Close() override;

    void Read() override;

    void Write() override;

    void OnRead(boost::system::error_code ec, std::size_t bytes_transferred) override;

    void OnWrite(boost::system::error_code ec, std::size_t bytes_transferred) override;

  private:
};

/// @}
/// @}

}  // namespace game::support

#endif  // SUPPORT_NETWORKING_WEBSOCKET_SESSION_H
