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
        route_manager_;

    std::unordered_map<std::string, WSContext const*> ctxs_;
    std::string state_ {};
};

}  // namespace game::test

auto main(int argc, char* argv[]) -> int
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