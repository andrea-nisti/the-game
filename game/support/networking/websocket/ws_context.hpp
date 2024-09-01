#ifndef SUPPORT_NETWORKING_WEBSOCKET_WS_CONTEXT_H
#define SUPPORT_NETWORKING_WEBSOCKET_WS_CONTEXT_H

#include <memory>
#include <string>

#include <boost/asio.hpp>

// #include "support/networking/websocket/websocket_session.h"

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
    // attributes attributes;

    // std::string route_path;
};
}  // namespace game::support

#endif  // SUPPORT_NETWORKING_WEBSOCKET_WS_CONTEXT_H
