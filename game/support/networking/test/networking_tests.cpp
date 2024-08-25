// #include <gtest/gtest.h>
#include <memory>

#include "support/networking/http_session.h"
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
        http = std::make_shared<HttpSession>(std::move(socket));
        http->Run();
        return;
    }
    std::shared_ptr<HttpSession> http;
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