#ifndef SUPPORT_NETWORKING_HTTP_HTTP_SESSION_H
#define SUPPORT_NETWORKING_HTTP_HTTP_SESSION_H

#include <cstdlib>
#include <memory>
#include <utility>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/version.hpp>
#include <boost/optional.hpp>
#include <boost/throw_exception.hpp>

#include "support/networking/route_manager_base.hpp"
#include "support/networking/session_base.h"

namespace beast = boost::beast;    // from <boost/beast.hpp>
namespace http = beast::http;      // from <boost/beast/http.hpp>
namespace net = boost::asio;       // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>

namespace game::support {

/**
 * \addtogroup support Support
 * \brief
 * @{
 */

/**
 * \addtogroup networking Networking Utilities
 * \brief Utility classes and functions for network operations
 * \ingroup support
 * @{
 */

/**
 * @class HttpSession
 * @brief Represents an HTTP session.
 *
 * This class provides a way to handle HTTP requests and responses.
 * It inherits from SessionBase and provides additional functionality
 * specific to HTTP sessions.
 *
 * @extends SessionBase<beast::tcp_stream, beast::flat_buffer>
 * @extends std::enable_shared_from_this<HttpSession>
 */
class HttpSession final : public SessionBase<beast::tcp_stream, beast::flat_buffer>,
                          public std::enable_shared_from_this<HttpSession>
{
  public:
    /**
     * @brief Constructor. Creates a new HttpSession object.
     *
     * @param socket The socket to use for the session.
     */
    HttpSession(
        tcp::socket&& socket,
        RouteManagerBase<
            http::request<http::string_body>,
            http::response<http::string_body>>* route_manager)
        : SessionBase<beast::tcp_stream, beast::flat_buffer>(std::move(socket)),
          route_manager_(route_manager)
    {}

    /**
     * @brief Destructor. Closes the session.
     */
    virtual ~HttpSession() { Close(); }

    /**
     * @brief Starts the session and begins processing requests.
     */
    void Run() override;

    /**
     * @brief Closes the session and releases any resources.
     */
    void Close() override;

    /**
     * @brief Reads data from the socket and processes any requests.
     */
    void Read() override;

    /**
     * @brief Writes data to the socket by sending responses.
     */
    void Write() override;

  private:
    /**
     * @brief This method is called when data is read from the session.
     *
     * @param ec The error code.
     * @param bytes_transferred The number of bytes transferred.
     */
    void OnRead(boost::system::error_code ec, std::size_t bytes_transferred) override;

    /**
     * @brief Called when data is written to the session.
     *
     * @param ec The error code.
     * @param bytes_transferred The number of bytes transferred.
     */
    void OnWrite(boost::system::error_code ec, std::size_t bytes_transferred) override;

    /**
     * @brief The parser for the session.
     */
    boost::optional<http::request_parser<http::string_body>> parser_;

    /**
     * @brief Whether to keep the session alive, set by the client. If false, the session
     * will be closed.
     */
    bool keep_alive_ {false};

    /**
     * @brief The response for the session.
     */
    boost::optional<http::response<http::string_body>> response_;

    /**
     * @brief The route manager for the session.
     */
    RouteManagerBase<http::request<http::string_body>, http::response<http::string_body>>*
        route_manager_;
};

/// @}
/// @}

}  // namespace game::support

#endif  // SUPPORT_NETWORKING_HTTP_HTTP_SESSION_H
