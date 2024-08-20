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
            std::cout << "Accepted connection" << std::endl;
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