#include <iostream>
#include <memory>
#include <optional>

#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <sys/types.h>

#include "support/networking/http/beast_utils.hpp"
#include "support/networking/http/common.hpp"
#include "support/networking/http/http_session.h"
#include "support/networking/route_manager_base.hpp"
#include "support/networking/tcp_listener_base.h"
#include "support/networking/websocket/websocket_session.h"

namespace game::test {

using namespace support;
using ReqT = http::request<http::string_body>;
using ResT = http::response<http::string_body>;

class TestListener : public support::TcpListenerBase
{
  public:
    using support::TcpListenerBase::TcpListenerBase;

  private:
    void OnAccept(tcp::socket socket) override
    {
        auto http =
            std::make_shared<HttpSession>(std::move(socket), route_manager_.get());
        http->Run();
        return;
    }

    std::unique_ptr<RouteManagerBase<
        http::request<http::string_body>,
        http::response<http::string_body>>>
        route_manager_ =
            game::support::RouteManagerBuilder<ReqT, ResT> {}
                .Add(
                    ConvertVerbBeast(http::verb::get),
                    "/test",
                    [](const ReqT& req, std::optional<Params> params, ResT& res) -> void
                    {
                        std::string res_body = "UwU Kawaiiiiiiii!";
                        if (params.has_value())
                        {
                            res_body += "\n";
                            res_body += "Params: {";
                            for (auto& [key, value] : params.value())
                            {
                                res_body += "\n  \"" + key + "\" : \"" + value + "\",";
                            }
                            res_body += "\n}";
                        }
                        res.set(http::field::content_type, "text/plain; charset=utf-8");
                        res.body() = res_body;
                    })
                .Add(
                    ConvertVerbBeast(http::verb::post),
                    "/test/set_state",
                    [this](
                        const ReqT& req, std::optional<Params> params, ResT& res) -> void
                    {
                        state_ = req.body();
                        res.set(http::field::content_type, "text/json; charset=utf-8");
                        res.body() = "Set state to: \n" + state_;

                        // broadcast state
                        for (auto& ctx : ctxs_)
                        {
                            if (auto s = ctx.second->ws_session.lock(); s)
                            {
                                // TODO: add customizable delay and jitter
                                s->Send(state_);
                            }
                        }
                    })
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

    std::unordered_map<std::string, WSContext const*> ctxs_;
    std::string state_ {};
};

}  // namespace game::test

// TODO: create an app that uses a post to set the state and the websocket to notify
// clients
int main()
{
    using namespace game::test;

    boost::asio::io_context io_context;
    TestListener listener {io_context, tcp::endpoint {tcp::v4(), 8888}};
    listener.Start();
    io_context.run();

    return 0;
}