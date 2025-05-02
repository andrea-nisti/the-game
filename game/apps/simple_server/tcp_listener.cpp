#include "tcp_listener.h"

namespace game::simple_server {

void TcpListener::OnAccept(tcp::socket socket)
{
    auto http = std::make_shared<HttpSession>(std::move(socket), route_manager_.get());
    http->Run();
    return;
}

}  // namespace game::simple_server
