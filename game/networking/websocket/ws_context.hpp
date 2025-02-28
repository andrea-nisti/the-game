#ifndef SUPPORT_NETWORKING_WEBSOCKET_WS_CONTEXT_H
#define SUPPORT_NETWORKING_WEBSOCKET_WS_CONTEXT_H

#include <memory>
#include <string>

#include <boost/asio.hpp>

namespace net = boost::asio;       // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>

namespace game::support
{
class WebSocketSession;

struct WSContext
{
    tcp::endpoint remote_endpoint;
    tcp::endpoint local_endpoint;
    std::string uuid;
    std::weak_ptr<WebSocketSession> ws_session;
    std::string target;
};
}  // namespace game::support

#endif  // SUPPORT_NETWORKING_WEBSOCKET_WS_CONTEXT_H
