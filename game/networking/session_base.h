#ifndef NETWORKING_SESSION_BASE
#define NETWORKING_SESSION_BASE

#include <cstdlib>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/optional.hpp>
#include <boost/throw_exception.hpp>

#define VALUE(string) #string
#define TO_STRING(s) VALUE(s)

namespace game::support {

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

template <typename Stream, typename Buffer>
class SessionBase
{
  public:
    // Take ownership of the socket
    SessionBase(tcp::socket&& socket) : stream_(std::move(socket)) {}

    /**
     * Start the session
     */
    virtual void Run() = 0;

  private:
    /// Called when a read is complete
    virtual void OnRead(boost::system::error_code ec, std::size_t bytes_transferred) = 0;

    /// Called when a write is complete
    virtual void OnWrite(boost::system::error_code ec, std::size_t bytes_transferred) = 0;

    /**
     * Start the read operation
     */
    virtual void Read() = 0;
    /**
     * Start the write operation
     */
    virtual void Write() = 0;
    /**
     * Start the close operation
     */
    virtual void Close() = 0;

  protected:
    Stream stream_;
    Buffer buffer_;
};

}  // namespace game::support

#endif  // NETWORKING_SESSION_BASE
