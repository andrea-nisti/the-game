#ifndef APPS_SIMPLE_SERVER_PLAYER_CONTROLLER_BASE
#define APPS_SIMPLE_SERVER_PLAYER_CONTROLLER_BASE

#include <boost/asio.hpp>

namespace game::core {

template <typename Command>
class PlayerControllerBase
{
  public:
    explicit PlayerControllerBase(boost::asio::io_context& io_ctx) : io_ctx_(io_ctx) {}
    PlayerControllerBase(const PlayerControllerBase&) = delete;
    PlayerControllerBase(PlayerControllerBase&&) = delete;
    auto operator=(const PlayerControllerBase&) -> PlayerControllerBase& = delete;
    auto operator=(PlayerControllerBase&&) -> PlayerControllerBase& = delete;

    virtual ~PlayerControllerBase() = default;

    virtual void OnCommand(const Command& command) = 0;
    virtual void SendMessage(const std::string& message) = 0;
    virtual void Disconnect() = 0;

  protected:
    boost::asio::io_context& io_ctx_;
};

}  // namespace game::core

#endif  // APPS_SIMPLE_SERVER_PLAYER_CONTROLLER_BASE
