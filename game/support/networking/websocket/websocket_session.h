#ifndef SUPPORT_NETWORKING_WEBSOCKET_SESSION_H
#define SUPPORT_NETWORKING_WEBSOCKET_SESSION_H

#include <cstdlib>
#include <memory>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/optional.hpp>
#include <boost/throw_exception.hpp>

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

    void Run() override;

  private:
    void OnAccept(boost::system::error_code ec);
    void Read() override;
    void OnRead(boost::system::error_code ec, std::size_t bytes_transferred) override;
    void Write() override {}
    void OnWrite(boost::system::error_code ec, std::size_t bytes_transferred) override;
    void Close() override {}

    WSContext ctx_;
    http::request<http::string_body> req_;
    WSHandler handler_;
};

/// @}
/// @}

}  // namespace game::support

#endif  // SUPPORT_NETWORKING_WEBSOCKET_SESSION_H
