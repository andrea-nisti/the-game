#ifndef APPS_SIMPLE_SERVER_PLAYER_CONTROLLER_BASE_IMPL
#define APPS_SIMPLE_SERVER_PLAYER_CONTROLLER_BASE_IMPL

namespace game::core {

template <typename Command>
PlayerControllerBase<Command>::PlayerControllerBase(boost::asio::io_context& io_ctx) : io_ctx_(io_ctx)
{
}

}  // namespace game::core

#endif  // APPS_SIMPLE_SERVER_PLAYER_CONTROLLER_BASE_IMPL
