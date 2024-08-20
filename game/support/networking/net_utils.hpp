#ifndef SUPPORT_NETWORKING_NET_UTILS_H
#define SUPPORT_NETWORKING_NET_UTILS_H

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>

namespace game::support
{

namespace net = boost::asio;
inline void Fail(boost::system::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

inline bool InitializeAcceptor(
    boost::asio::ip::tcp::acceptor& acceptor,
    boost::asio::ip::tcp::endpoint const& endpoint,
    boost::system::error_code& ec)
{
    // Open the acceptor
    acceptor.open(endpoint.protocol(), ec);
    if (ec)
    {
        Fail(ec, "open");
        return false;
    }

    // Allow address reuse
    acceptor.set_option(net::socket_base::reuse_address(true), ec);
    if (ec)
    {
        Fail(ec, "set_option");
        return false;
    }

    // Bind to the server address
    acceptor.bind(endpoint, ec);
    if (ec)
    {
        Fail(ec, "bind");
        return false;
    }

    // Start listening for connections
    acceptor.listen(net::socket_base::max_listen_connections, ec);
    if (ec)
    {
        Fail(ec, "listen");
        return false;
    }
    return true;
}

}  // namespace game::support

#endif  // SUPPORT_NETWORKING_NET_UTILS_H
