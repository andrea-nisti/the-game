#include "websocket_session.h"

#include <string_view>

#include "networking/net_utils.hpp"

namespace game::networking {

void WebSocketSession::Run()
{
    ctx_.remote_endpoint = stream_.next_layer().socket().remote_endpoint();
    ctx_.local_endpoint = stream_.next_layer().socket().local_endpoint();
    ctx_.target = std::string {req_.target()};

    // Set suggested timeout settings for the websocket
    stream_.set_option(
        websocket::stream_base::timeout::suggested(beast::role_type::server));

    // Set a decorator to change the Server of the handshake
    stream_.set_option(websocket::stream_base::decorator(
        [](websocket::response_type& res)
        {
            res.set(
                http::field::server,
                std::string(BOOST_STRINGIZE(SERVER_VERSION)) + " advanced-server");
        }));

    // Accept the websocket handshake
    stream_.async_accept(
        req_,
        [self = shared_from_this()](boost::system::error_code ec)
        { self->OnAccept(ec); });
}

void WebSocketSession::Read()
{
    // Read a message into our buffer
    stream_.async_read(
        buffer_,
        [self = shared_from_this()](
            boost::system::error_code ec, std::size_t bytes_transferred)
        { self->OnRead(ec, bytes_transferred); });
}

void WebSocketSession::OnRead(boost::system::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    // This indicates that the websocket_session was closed
    if (ec == websocket::error::closed)
    {
        std::invoke(handler_.on_disconnect, ctx_);
        return;
    }

    if (ec)
    {
        const auto where = "on read";
        support::Fail(ec, where);
        std::invoke(handler_.on_error, ec, where);
        return;
    }

    const auto& p = buffer_.data();
    stream_.text(stream_.got_text());
    std::string_view data_view {boost::asio::buffer_cast<const char*>(p), p.size()};
    std::invoke(handler_.on_receive, ctx_, data_view, p.size(), stream_.binary());
    buffer_.consume(p.size());

    Read();
}

void WebSocketSession::Write()
{
    stream_.async_write(
        write_buf_.cdata(),
        [self = shared_from_this()](
            boost::system::error_code ec, std::size_t bytes_transferred)
        { self->OnWrite(ec, bytes_transferred); });
}

void WebSocketSession::OnWrite(
    boost::system::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);
    if (ec)
    {
        const auto where = "on write";
        support::Fail(ec, where);
        std::invoke(handler_.on_error, ec, where);
        return;
    }

    write_buf_.consume(bytes_transferred);
}

void WebSocketSession::OnAccept(boost::system::error_code ec)
{
    if (ec)
    {
        const auto where = "on accept";
        support::Fail(ec, where);
        std::invoke(handler_.on_error, ec, where);
        return;
    }

    ctx_.ws_session = shared_from_this();
    ctx_.uuid = req_.at("Sec-WebSocket-Key");
    std::invoke(handler_.on_connect, ctx_);

    // Read a message
    Read();
}
}  // namespace game::networking