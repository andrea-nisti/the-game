#ifndef APPS_SIMPLE_SERVER_TCP_LISTENER_H
#define APPS_SIMPLE_SERVER_TCP_LISTENER_H

#include <exception>
#include <iostream>
#include <memory>
#include <optional>

#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <sys/types.h>

#include "networking/http/beast_utils.hpp"
#include "networking/http/common.hpp"
#include "networking/http/http_session.h"
#include "networking/route_manager_base.hpp"
#include "networking/tcp_listener_base.h"
#include "networking/websocket/websocket_session.h"

namespace game::simple_server {

using namespace support;
using Request = http::request<http::string_body>;
using Response = http::response<http::string_body>;

namespace {
template <typename F>
void ForEachParam(const std::optional<Params>& params, F&& f)
{
    if (params.has_value())
    {
        for (auto& [key, value] : params.value())
        {
            std::invoke(std::forward<F>(f), key, value);
        }
    }
}

using UUID = std::string;

}  // namespace

class TcpListener : public support::TcpListenerBase
{
  public:
    using ReqT = http::request<http::string_body>;
    using ResT = http::response<http::string_body>;
    using support::TcpListenerBase::TcpListenerBase;
    using RouteManagerPtr = std::unique_ptr<RouteManagerBase<ReqT, ResT>>;

  private:
    void OnAccept(tcp::socket socket) override;

    std::unordered_map<UUID, WSContext const*> ctxs_;
    RouteManagerPtr route_manager_ =
        game::support::RouteManagerBuilder<ReqT, ResT> {}
            .AddWS(
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
};

}  // namespace game::simple_server

#endif  // APPS_SIMPLE_SERVER_TCP_LISTENER_H
