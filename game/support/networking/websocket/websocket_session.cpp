#include "websocket_session.h"

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

}  // namespace game::support