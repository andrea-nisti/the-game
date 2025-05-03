#ifndef APPS_SIMPLE_SERVER_GAME_SESSION
#define APPS_SIMPLE_SERVER_GAME_SESSION

#include <boost/asio.hpp>

namespace game::core {

template <typename Command>
class GameSession
{
  public:
    explicit GameSession(boost::asio::io_context& io_ctx)
        : command_strand_(boost::asio::make_strand(io_ctx))
    {}
    GameSession(const GameSession&) = delete;
    GameSession(GameSession&&) = delete;
    auto operator=(const GameSession&) -> GameSession& = delete;
    auto operator=(GameSession&&) -> GameSession& = delete;

    ~GameSession() = default;

    void OnCommand(const Command& command) {}

  private:
    boost::asio::strand<boost::asio::io_context::executor_type> command_strand_;
};
}  // namespace game::core

#endif  // APPS_SIMPLE_SERVER_GAME_SESSION
