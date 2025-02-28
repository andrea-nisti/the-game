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
}  // namespace

class TcpListener : public support::TcpListenerBase
{
  public:
    using support::TcpListenerBase::TcpListenerBase;

  private:
    void OnAccept(tcp::socket socket) override;
};

}  // namespace game::simple_server

#endif  // APPS_SIMPLE_SERVER_TCP_LISTENER_H
