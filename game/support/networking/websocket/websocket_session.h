#ifndef SUPPORT_NETWORKING_WEBSOCKET_SESSION_H
#define SUPPORT_NETWORKING_WEBSOCKET_SESSION_H

#include <cstdlib>
#include <iostream>
#include <memory>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/optional.hpp>
#include <boost/throw_exception.hpp>

#include "support/networking/net_utils.hpp"
#include "support/networking/session_base.h"
#include "support/networking/websocket/ws_context.hpp"
#include "support/networking/websocket/ws_handler.hpp"

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
    WebSocketSession(const WebSocketSession &) = delete;
    WebSocketSession(WebSocketSession &&) = delete;
    WebSocketSession &operator=(const WebSocketSession &) = delete;
    WebSocketSession &operator=(WebSocketSession &&) = delete;
    WebSocketSession(
        tcp::socket &&socket, http::request<http::string_body> req, WSHandler handler)
        : SessionBase<websocket::stream<beast::tcp_stream>, beast::flat_buffer>(
              std::move(socket)),

          req_(std::move(req)),
          handler_(std::move(handler))
    {}

    virtual ~WebSocketSession() { Close(); }

    /**
     * @brief Starts the session and begins processing requests.
     */
    void Run() override;

    void OnAccept(boost::system::error_code ec)
    {
        if (ec)
        {
            Fail(ec, "accept");
            std::invoke(handler_.on_error, ec, "accept");
            return;
        }

        ctx_.ws_session = shared_from_this();
        ctx_.uuid = req_.at("Sec-WebSocket-Key");
        std::invoke(handler_.on_connect, ctx_);

        // Read a message
        Read();
    }
    void Write() override {}
    void Close() override {}

  private:
    void Read() override;

    void OnRead(boost::system::error_code ec, std::size_t bytes_transferred) override
    {
        boost::ignore_unused(bytes_transferred);

        // This indicates that the websocket_session was closed
        if (ec == websocket::error::closed)
        {
            std::invoke(handler_.on_disconnect, ctx_);
            return;
        }

        if (ec)
        {
            Fail(ec, "on read");
            std::invoke(handler_.on_error, ec, "on read");
            return;
        }

        // Echo the message
        // stream_.text(stream_.got_text());
        // std::string message = boost::beast::buffers_to_string(buffer_.data());
        // message += " UwU";

        // Append " UwU" to the buffer
        std::string message = " UwU";
        net::buffer_copy(buffer_.prepare(message.size()), net::buffer(message));
        buffer_.commit(message.size());

        stream_.async_write(
            buffer_.cdata(),
            [self = shared_from_this()](
                boost::system::error_code ec, std::size_t bytes_transferred)
            { self->OnWrite(ec, bytes_transferred); });
    }

    void OnWrite(boost::system::error_code ec, std::size_t bytes_transferred) override
    {
        buffer_.consume(bytes_transferred);
        Read();
    }

    WSContext ctx_;
    http::request<http::string_body> req_;
    WSHandler handler_;
};

/// @}
/// @}

}  // namespace game::support

#endif  // SUPPORT_NETWORKING_WEBSOCKET_SESSION_H
