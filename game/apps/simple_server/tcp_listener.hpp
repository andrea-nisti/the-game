#ifndef APPS_SIMPLE_SERVER_TCP_LISTENER
#define APPS_SIMPLE_SERVER_TCP_LISTENER

#include <memory>
#include <optional>

#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/beast/http.hpp>
#include <sys/types.h>

#include "networking/http/common.hpp"
#include "networking/route_manager_base.hpp"
#include "networking/tcp_listener_base.h"

namespace game::simple_server {

namespace http = boost::beast::http;  // from <boost/beast/http.hpp>

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
    using support::TcpListenerBase::TcpListenerBase;

    using RouteManagerPtr = std::unique_ptr<RouteManagerBase<Request, Response>>;

    TcpListener(boost::asio::io_context& ioc, tcp::endpoint&& endpoint)
        : TcpListenerBase(ioc, std::move(endpoint)), route_manager_(BuildRouteManager())
    {}

  private:
    void OnAccept(tcp::socket socket) override;
    auto BuildRouteManager() -> RouteManagerPtr;

    RouteManagerPtr route_manager_;
    std::unordered_map<UUID, WSContext const*> ctxs_;
};

}  // namespace game::simple_server

#endif  // APPS_SIMPLE_SERVER_TCP_LISTENER
