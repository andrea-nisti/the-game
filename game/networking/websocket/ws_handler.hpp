#ifndef SUPPORT_NETWORKING_WS_HANDLER_H
#define SUPPORT_NETWORKING_WS_HANDLER_H

#include <functional>

#include <boost/system/error_code.hpp>

#include "ws_context.hpp"

namespace game::support {

using OnConnect = std::function<void(const WSContext&)>;
using OnReceive =
    std::function<void(const WSContext&, std::string_view, std::size_t, bool)>;
using OnDisconnect = std::function<void(const WSContext& ctx)>;
using OnError = std::function<void(boost::system::error_code, std::string_view what)>;

struct WSHandler
{
    OnConnect on_connect = [](const WSContext&) {};
    OnDisconnect on_disconnect = [](const WSContext& ctx) {};
    OnReceive on_receive = [](const WSContext&, std::string_view, std::size_t, bool) {};
    OnError on_error = [](boost::system::error_code ec, std::string_view what) {};
};

}  // namespace game::support

#endif  // SUPPORT_NETWORKING_WS_HANDLER_H
