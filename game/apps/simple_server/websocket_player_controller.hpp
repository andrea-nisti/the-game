#ifndef APPS_SIMPLE_SERVER_WEBSOCKET_PLAYER_CONTROLLER
#define APPS_SIMPLE_SERVER_WEBSOCKET_PLAYER_CONTROLLER

#include <memory>
#include <string>

#include "apps/simple_server/player_controller_base.hpp"
#include "networking/websocket/websocket_session.h"

namespace game::core {

template <typename Command>
class WebSocketPlayerController : public PlayerControllerBase<Command>
{
  public:
    WebSocketPlayerController(boost::asio::io_context& io_ctx, networking::WSContext ctx)
        : PlayerControllerBase<Command>(io_ctx), ws_context_(std::move(ctx))
    {}

    void OnCommand(const Command& command) override
    {
        // Ensure commands are processed sequentially using the strand
        boost::asio::post(
            this->io_ctx_.get_executor(),
            [this, command]()
            {
                // TODO: call command dispatcher
            });
    }

    void SendMessage(const std::string& message) override
    {
        if (auto session = ws_context_.ws_session.lock())
        {
            session->Send(message);
        }
    }

    void Disconnect() override
    {
        if (auto session = ws_context_.ws_session.lock())
        {
            // session->Close();
        }
    }

  private:
    networking::WSContext ws_context_;
};

}  // namespace game::core

#endif  // APPS_SIMPLE_SERVER_WEBSOCKET_PLAYER_CONTROLLER
