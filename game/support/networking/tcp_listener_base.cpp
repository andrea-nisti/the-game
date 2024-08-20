#include "tcp_listener_base.h"

#include <boost/asio/dispatch.hpp>

namespace game::support
{

void TcpListenerBase::Start()
{
    net::dispatch(acceptor_.get_executor(), [this]() { this->DoAccept(); });
}

void TcpListenerBase::Stop()
{
    acceptor_.close();
    io_context_ref_.stop();
    OnClose();
}

void TcpListenerBase::DoAccept()
{  // The new connection gets its own strand
    acceptor_.async_accept(
        io_context_ref_,
        [this](auto ec, auto socket) -> void
        { this->OnAcceptContinue(ec, std::move(socket)); });
}

void TcpListenerBase::OnAcceptContinue(boost::system::error_code ec, tcp::socket socket)
{
    OnAccept(ec, std::move(socket));

    // Accept another connection
    DoAccept();
}

}  // namespace game::support