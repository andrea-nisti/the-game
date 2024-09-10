#include "http_session.h"

#include <string>

#include <boost/beast/http/message.hpp>
#include <boost/beast/http/status.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/url.hpp>

#include "beast_utils.hpp"
#include "support/networking/net_utils.hpp"
#include "support/networking/websocket/websocket_session.h"

#define VERSION SERVER_VERSION

namespace game::support {

namespace {
using namespace boost::urls;
using namespace boost::asio::ip;
std::optional<url> ParseUri(const basic_endpoint<tcp>& endpoint, std::string target)
{
    std::string const local_address = endpoint.address().to_string();
    std::string const port = std::to_string(endpoint.port());
    std::string const protocol = "http";

    std::string full_path {protocol + "://" + local_address + ":" + port + target};
    std::cout << "uri " << full_path << std::endl;
    auto const parsed = parse_uri(full_path);

    if (parsed.has_value())
    {
        auto const& p = parsed.value();
        std::cout << "scheme: " << p.scheme() << std::endl;
        std::cout << "host: " << p.host() << std::endl;
        std::cout << "port: " << p.port() << std::endl;
        std::cout << "path: " << p.path() << std::endl;
        std::cout << "query: " << p.query() << std::endl;
        std::cout << "fragment: " << p.fragment() << std::endl;
        return parsed.value();
    }

    return {};
}
}  // namespace

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

    const auto request = parser_->release();
    const auto method = ConvertVerbBeast(request.method());
    const auto full_target = request.target();
    const bool is_upgrade = beast::websocket::is_upgrade(request);
    const auto uri = ParseUri(stream_.socket().local_endpoint(), full_target);

    bool target_found = false;
    if (uri.has_value())
    {
        const auto& target = uri.value().path();
        response_.emplace(http::status::ok, request.version());
        response_->keep_alive(request.keep_alive());
        response_->set(http::field::server, TO_STRING(VERSION));
        if (is_upgrade)
        {
            const auto cb = route_manager_->GetWSHandler(method, target);
            target_found = cb.has_value();
            if (target_found)
            {
                std::make_shared<WebSocketSession>(
                    stream_.release_socket(), request, std::move(cb.value()))
                    ->Run();
                return;
            }
        }
        else
        {
            const auto cb = route_manager_->GetCallback(method, target);
            target_found = cb.has_value();
            if (target_found)
            {
                std::invoke(cb.value(), request, response_.value());
            }
        }
    }

    if (not target_found)
    {
        // manage target not found
        response_->result(http::status::not_found);
        response_->body() = "Unknown HTTP-target: " + std::string(full_target);
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
