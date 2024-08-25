
#include "support/networking/http_session.h"

#include <boost/beast/http/message.hpp>

#include "support/networking/net_utils.hpp"

namespace game::support
{

namespace
{
http::response<http::string_body> handle_request(
    http::request<http::string_body>&& request)
{
    std::string res_body = "UwU Kawaiiiiiiii!";

    http::response<http::string_body> response {http::status::ok, request.version()};

    response.set(http::field::server, "TheGameServer-v0");
    response.set(http::field::content_type, "text/plain; charset=utf-8");
    response.body() = res_body;
    response.keep_alive(request.keep_alive());
    std::cout << "is keep alive: " << response.keep_alive() << std::endl;

    return response;
}

}  // namespace
void HttpSession::Run()
{
    std::cout << "run" << std::endl;
    net::dispatch(
        stream_.get_executor(), [self = this->shared_from_this()]() { self->Read(); });
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
void HttpSession::Close()
{
    // Send a TCP shutdown
    boost::system::error_code ec;
    stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
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

    response_ = handle_request(parser_->release());
    Write();
    Read();
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
