#ifndef APPS_SIMPLE_SERVER_WEBSOCKET_PLAYER_CONTROLLER
#define APPS_SIMPLE_SERVER_WEBSOCKET_PLAYER_CONTROLLER

#include "networking/websocket/websocket_session.h"
#include "player_controller_base.hpp"
#include <memory>
#include <string>

namespace game::core {

template <typename Command>
class WebSocketPlayerController : public PlayerControllerBase<Command>
{
  public:
    WebSocketPlayerController(boost::asio::io_context& io_ctx, networking::WSContext ctx);

    void OnCommand(const Command& command) override;
    void SendMessage(const std::string& message) override;
    void Disconnect() override;

  private:
    networking::WSContext ws_context_;
};

}  // namespace game::core

#include "websocket_player_controller.cpp"

#endif  // APPS_SIMPLE_SERVER_WEBSOCKET_PLAYER_CONTROLLER
