#include "http_session.h"

#include <boost/beast/http/message.hpp>

#include "net_utils.hpp"
#include "support/networking/beast_utils.hpp"

#define VERSION SERVER_VERSION

#define VALUE(string) #string
#define TO_STRING(s) VALUE(s)

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
    std::cout << "on read" << std::endl;
    boost::ignore_unused(bytes_transferred);

    // This means they closed the connection
    if (ec == http::error::end_of_stream)
        return Close();

    if (ec)
    {
        Fail(ec, "on read");
        return;
    }

    auto request = parser_->release();

    response_.emplace(http::status::ok, request.version());
    response_->keep_alive(request.keep_alive());
    response_->set(http::field::server, TO_STRING(VERSION));

    route_manager_->HandleRequest(
        ConvertVerbBeast(request.method()),
        request.target(),
        parser_->release(),
        response_.value());

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
