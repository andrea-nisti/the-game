#include <memory>

#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/address_v4.hpp>

#include "support/networking/beast_utils.hpp"
#include "support/networking/http_session.h"
#include "support/networking/route_manager_base.hpp"
#include "support/networking/tcp_listener_base.h"

namespace game::test
{

using namespace support;
using ReqT = http::request<http::string_body>;
using ResT = http::response<http::string_body>;

class TestListener : public support::TcpListenerBase
{
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
                    [](const ReqT& req, ResT& res) -> void
                    {
                        std::string res_body = "UwU Kawaiiiiiiii!";
                        res.set(http::field::content_type, "text/plain; charset=utf-8");
                        res.body() = res_body;
                    })
                .Build();
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