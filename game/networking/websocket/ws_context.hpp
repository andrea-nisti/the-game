#ifndef NETWORKING_WEBSOCKET_WS_CONTEXT
#define NETWORKING_WEBSOCKET_WS_CONTEXT

#include <memory>
#include <string>

#include <boost/asio.hpp>

namespace net = boost::asio;       // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>

namespace game::support {
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

#endif  // NETWORKING_WEBSOCKET_WS_CONTEXT
