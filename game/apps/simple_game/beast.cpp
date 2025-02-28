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

namespace game::test {

using namespace support;
using ReqT = http::request<http::string_body>;
using ResT = http::response<http::string_body>;

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
}  // namespace

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
                        std::string res_body = "UwU Kawaiiiiiiii!\n";

                        res_body += "Params: {";
                        ForEachParam(
                            params,
                            [&](const std::string& key, const std::string& value)
                            { res_body += "\n  \"" + key + "\" : \"" + value + "\","; });
                        res_body += "\n}";

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
                        res.body() = "Setting state to: \n" + state_;

                        // broadcast state
                        for (auto& ctx : ctxs_)
                        {
                            if (auto s = ctx.second->ws_session.lock(); s)
                            {
                                if (params.has_value() and params.value().count("delay"))
                                {
                                    int delay_secs {0};
                                    try
                                    {
                                        delay_secs =
                                            std::stoi(params.value().at("delay"));
                                    } catch (const std::exception& e)
                                    {
                                        std::cerr << "Failed to parse delay: " << e.what()
                                                  << std::endl;
                                    }

                                    auto timer =
                                        std::make_unique<boost::asio::steady_timer>(
                                            GetIOContextRef());
                                    timer->expires_after(
                                        std::chrono::seconds(delay_secs));

                                    timer->async_wait(
                                        [s, this, timer = std::move(timer)](
                                            const boost::system::error_code ec)
                                        { s->Send(state_); });
                                } else
                                {
                                    s->Send(state_);
                                }
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

int main(int argc, char* argv[])
{
    using namespace game::test;
    int port {8888};
    if (argc > 1)
    {
        try
        {
            port = std::stoi(argv[1]);
        } catch (const std::exception& ia)
        {
            std::cerr << "Invalid port: " << argv[1] << " : " << ia.what() << std::endl;
            return 1;
        }
    }
    std::cout << "Running server on port: " << port << std::endl;

    boost::asio::io_context io_context;
    TestListener listener {
        io_context, tcp::endpoint {tcp::v4(), net::ip::port_type(port)}};
    listener.Start();
    io_context.run();

    return 0;
}