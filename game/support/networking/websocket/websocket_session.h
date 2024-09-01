#ifndef SUPPORT_NETWORKING_WEBSOCKET_SESSION_H
#define SUPPORT_NETWORKING_WEBSOCKET_SESSION_H

#include <cstdlib>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/optional.hpp>
#include <boost/throw_exception.hpp>

#include "support/networking/net_utils.hpp"
#include "support/networking/session_base.h"

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
    WebSocketSession(tcp::socket&& socket, http::request<http::string_body> req)
        : SessionBase<websocket::stream<beast::tcp_stream>, beast::flat_buffer>(
              std::move(socket)),
          req_(std::move(req))
    {
    }

    virtual ~WebSocketSession() { Close(); }

    /**
     * @brief Starts the session and begins processing requests.
     */
    void Run() override
    {
        // Set suggested timeout settings for the websocket
        stream_.set_option(
            websocket::stream_base::timeout::suggested(beast::role_type::server));

        // Set a decorator to change the Server of the handshake
        stream_.set_option(websocket::stream_base::decorator(
            [](websocket::response_type& res)
            {
                res.set(
                    http::field::server,
                    std::string(BOOST_STRINGIZE(SERVER_VERSION)) + " advanced-server");
            }));

        std::cout << req_.body() << std::endl;
        // Accept the websocket handshake
        stream_.async_accept(
            req_,
            beast::bind_front_handler(&WebSocketSession::OnAccept, shared_from_this()));
    }

    void OnAccept(beast::error_code ec)
    {
        if (ec)
            Fail(ec, "accept");
        return;

        // Read a message
        // do_read();
    }

    void Close() override {}

    void Read() override {}

    void Write() override {}

    void OnRead(boost::system::error_code ec, std::size_t bytes_transferred) override {}

    void OnWrite(boost::system::error_code ec, std::size_t bytes_transferred) override {}

  private:
    http::request<http::string_body> req_;
};

/// @}
/// @}

}  // namespace game::support

#endif  // SUPPORT_NETWORKING_WEBSOCKET_SESSION_H
