// #include <gtest/gtest.h>
#include "support/networking/tcp_listener_base.h"

namespace game::test
{
using namespace support;

class TestListener : public support::TcpListenerBase
{
    using support::TcpListenerBase::TcpListenerBase;

  private:
    void OnAccept(tcp::socket socket) override
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