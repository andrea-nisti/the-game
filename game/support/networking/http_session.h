#ifndef SUPPORT_NETWORKING_HTTP_SESSION_H
#define SUPPORT_NETWORKING_HTTP_SESSION_H

#include <cstdlib>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/version.hpp>
#include <boost/optional.hpp>
#include <boost/throw_exception.hpp>

#include "net_utils.hpp"
#include "session_base.h"

namespace beast = boost::beast;    // from <boost/beast.hpp>
namespace http = beast::http;      // from <boost/beast/http.hpp>
namespace net = boost::asio;       // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>

namespace game::support
{

class HttpSession final : public SessionBase<beast::tcp_stream, beast::flat_buffer>,
                          public std::enable_shared_from_this<HttpSession>
{
  public:
    HttpSession(tcp::socket&& socket)
        : SessionBase<beast::tcp_stream, beast::flat_buffer>(std::move(socket))
    {
    }

    virtual ~HttpSession() { Close(); }

    void Run() override;

  private:
    void OnRead(boost::system::error_code ec, std::size_t bytes_transferred) override;
    void OnWrite(boost::system::error_code ec, std::size_t bytes_transferred) override;

    void Read() override;
    void Write() override;
    void Close() override;

    boost::optional<http::request_parser<http::string_body>> parser_;
    bool keep_alive_ {false};
    http::response<http::string_body> response_;
};

}  // namespace game::support

#endif  // SUPPORT_NETWORKING_HTTP_SESSION_H
