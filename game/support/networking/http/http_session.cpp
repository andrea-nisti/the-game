#include "http_session.h"

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/status.hpp>
#include <boost/beast/websocket.hpp>

#include "beast_utils.hpp"
#include "support/networking/net_utils.hpp"
#include "support/networking/websocket/websocket_session.h"

#define VERSION SERVER_VERSION

namespace game::support
{

void HttpSession::Run()
{
    std::cout << "run" << std::endl;
    net::dispatch(
        stream_.get_executor(), [self = shared_from_this()]() { self->Read(); });
}
void HttpSession::Close()
{
    // Send a TCP shutdown
    boost::system::error_code ec;
    stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
}
void HttpSession::Read()
{
    std::cout << "read" << std::endl;
    // Construct a new parser for each message
    parser_.emplace();

    // Apply a reasonable limit to the allowed size
    // of the body in bytes to prevent abuse.
    parser_->body_limit(10000);

    // Set the timeout.
    stream_.expires_after(std::chrono::seconds(30));

    // Read a request using the parser-oriented interface
    http::async_read(
        stream_,
        buffer_,
        *parser_,
        [self = shared_from_this()](
            boost::system::error_code ec, std::size_t bytes_transferred)
        { self->OnRead(ec, bytes_transferred); });
}
void HttpSession::OnRead(boost::system::error_code ec, std::size_t bytes_transferred)
{
    if (ec == http::error::end_of_stream)
    {
        return Close();
    }

    if (ec)
    {
        Fail(ec, "on read");
        return;
    }

    auto request = parser_->release();
    auto const method = ConvertVerbBeast(request.method());
    auto const target = request.target();
    bool const is_upgrade = beast::websocket::is_upgrade(request);

    response_.emplace(http::status::ok, request.version());
    response_->keep_alive(request.keep_alive());
    response_->set(http::field::server, TO_STRING(VERSION));
    bool target_found = false;
    if (is_upgrade)
    {
        auto const cb = route_manager_->GetWSHandler(method, target);
        target_found = cb.has_value();
        if (target_found)
        {
            std::make_shared<WebSocketSession>(
                stream_.release_socket(), std::move(request))
                ->Run();
            return;
        }
    }
    else
    {
        auto const cb = route_manager_->GetCallback(method, target);
        target_found = cb.has_value();
        if (target_found)
        {
            std::invoke(cb.value(), request, response_.value());
        }
    }

    if (not target_found)
    {
        // manage target not found
        response_->result(http::status::not_found);
        response_->body() = "Unknown HTTP-target: " + std::string(target);
    }

    Write();
    Read();
}

void HttpSession::Write()
{
    keep_alive_ = response_.value().keep_alive();
    response_.value().prepare_payload();

    beast::async_write(
        stream_,
        http::message_generator {std::move(response_.value())},
        [self = shared_from_this()](
            boost::system::error_code ec, std::size_t bytes_transferred)
        { self->OnWrite(ec, bytes_transferred); });
}
void HttpSession::OnWrite(boost::system::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);
    std::cout << "on write" << std::endl;
    if (ec)
    {
        Fail(ec, "on write");
        return;
    }

    if (not keep_alive_)
    {
        // This means we should close the connection, usually because
        // the response indicated the "Connection: close" semantic.
        return Close();
    }
}
}  // namespace game::support
