#ifndef APPS_SIMPLE_SERVER_WEBSOCKET_PLAYER_CONTROLLER_IMPL
#define APPS_SIMPLE_SERVER_WEBSOCKET_PLAYER_CONTROLLER_IMPL

namespace game::core {

template <typename Command>
WebSocketPlayerController<Command>::WebSocketPlayerController(boost::asio::io_context& io_ctx,
                                                              networking::WSContext ctx)
    : PlayerControllerBase<Command>(io_ctx), ws_context_(std::move(ctx))
{
}

template <typename Command>
void WebSocketPlayerController<Command>::OnCommand(const Command& command)
{
    // Ensure commands are processed sequentially using the strand
    boost::asio::post(this->io_ctx_.get_executor(), [this, command]() {
        // TODO: call command dispatcher
    });
}

template <typename Command>
void WebSocketPlayerController<Command>::SendMessage(const std::string& message)
{
    if (auto session = ws_context_.ws_session.lock())
    {
        session->Send(message);
    }
}

template <typename Command>
void WebSocketPlayerController<Command>::Disconnect()
{
    if (auto session = ws_context_.ws_session.lock())
    {
        // session->Close();
    }
}

}  // namespace game::core

#endif  // APPS_SIMPLE_SERVER_WEBSOCKET_PLAYER_CONTROLLER_IMPL
