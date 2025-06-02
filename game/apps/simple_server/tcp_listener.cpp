#include "tcp_listener.hpp"

#include "networking/http/http_session.h"
#include "networking/websocket/websocket_session.h"

namespace game::simple_server {

using namespace networking;

void TcpListener::OnAccept(tcp::socket socket)
{
    auto http = std::make_shared<HttpSession>(std::move(socket), route_manager_.get());
    http->Run();
    return;
}

auto TcpListener::BuildRouteManager() -> RouteManagerPtr
{
    return RouteManagerBuilder<Request, Response> {}
        .AddWebsocket(
            "/",
            {.on_connect =
                 [this](const WSContext& ctx)
             {
                 std::cout << ctx.uuid << " connected" << std::endl;
                 this->ctxs_[ctx.uuid] = &ctx;
             },
             .on_disconnect =
                 [this](const WSContext& ctx)
             {
                 std::cout << ctx.uuid << " disconnected" << std::endl;
                 if (ctxs_.count(ctx.uuid))
                 {
                     ctxs_.erase(ctx.uuid);
                 }
             },
             .on_receive =
                 [this](
                     const WSContext& ctx,
                     std::string_view buf,
                     std::size_t size,
                     bool is_binary)
             {
                 if (auto s = ctx.ws_session.lock(); s)
                 {
                     s->Send(std::string {buf} + " UwU Kawaiiiiiiii!");
                 } else
                 {
                     // remove this session from the list
                     if (ctxs_.count(ctx.uuid))
                     {
                         ctxs_.erase(ctx.uuid);
                     }
                 }
             }})
        .Build();
}

}  // namespace game::simple_server
