// #include <gtest/gtest.h>
#include "support/networking/net_utils.hpp"
#include "support/networking/tcp_listener_base.h"

namespace game::test
{

namespace net = boost::asio;
using tcp = net::ip::tcp;

class TestListener : public support::TcpListenerBase
{
    using support::TcpListenerBase::TcpListenerBase;

  private:
    void OnAccept(boost::system::error_code ec, tcp::socket socket) override
    {
        if (not ec)
        {

            const std::string response =
                "HTTP/1.1 200 OK\r\nContent-Type: text/plain; "
                "charset=utf-8\r\n\r\n UwU Kawaiiiiiiii!";

            std::array<char, 1024> buffer {};
            std::size_t bytes_received = 0;

            bytes_received = socket.receive(net::buffer(buffer));

            std::cout << "Received " << bytes_received << " bytes: "
                      << std::string(buffer.begin(), buffer.begin() + bytes_received)
                      << std::endl;

            socket.send(net::buffer(response));
            return;
        }
        support::Fail(ec, "OnAccept");
    }
};

}  // namespace game::test

int main()
{
    using namespace game::test;

    boost::asio::io_context io_context;
    TestListener listener {io_context, tcp::endpoint {tcp::v4(), 8080}};
    listener.Start();
    io_context.run();

    return 0;
}