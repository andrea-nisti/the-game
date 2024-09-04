#include "websocket_session.h"

#include "support/networking/net_utils.hpp"

namespace game::support
{

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
        Fail(ec, "on read");
        std::invoke(handler_.on_error, ec, "on read");
        return;
    }

    // Echo the message
    // stream_.text(stream_.got_text());
    // std::string message = boost::beast::buffers_to_string(buffer_.data());
    // message += " UwU";

    // Append " UwU" to the buffer
    std::string message = " UwU";
    net::buffer_copy(buffer_.prepare(message.size()), net::buffer(message));
    buffer_.commit(message.size());

    stream_.async_write(
        buffer_.cdata(),
        [self = shared_from_this()](
            boost::system::error_code ec, std::size_t bytes_transferred)
        { self->OnWrite(ec, bytes_transferred); });
}

void WebSocketSession::OnWrite(
    boost::system::error_code ec, std::size_t bytes_transferred)
{
    buffer_.consume(bytes_transferred);
    Read();
}

void WebSocketSession::OnAccept(boost::system::error_code ec)
{
    if (ec)
    {
        Fail(ec, "accept");
        std::invoke(handler_.on_error, ec, "accept");
        return;
    }

    ctx_.ws_session = shared_from_this();
    ctx_.uuid = req_.at("Sec-WebSocket-Key");
    std::invoke(handler_.on_connect, ctx_);

    // Read a message
    Read();
}
}  // namespace game::support