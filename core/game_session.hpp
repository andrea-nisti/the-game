#ifndef APPS_SIMPLE_SERVER_GAME_SESSION
#define APPS_SIMPLE_SERVER_GAME_SESSION

#include <boost/asio.hpp>
#include <algorithm>
#include <memory>
#include <vector>

namespace game::core {

template <typename Command>
class PlayerControllerBase
{
  public:
    virtual ~PlayerControllerBase() = default;
    virtual void SendMessage(const std::string& message) = 0;
    virtual void Disconnect() = 0;
    [[nodiscard]] virtual std::string GetId() const = 0;
};

template <typename Command>
class GameSession : public std::enable_shared_from_this<GameSession<Command>>
{
  public:
    using PlayerControllerPtr = std::shared_ptr<PlayerControllerBase<Command>>;

    explicit GameSession(boost::asio::io_context& io_ctx);
    GameSession(const GameSession&) = delete;
    GameSession(GameSession&&) = delete;
    auto operator=(const GameSession&) -> GameSession& = delete;
    auto operator=(GameSession&&) -> GameSession& = delete;

    virtual ~GameSession() = default;

    void OnCommand(const Command& command, PlayerControllerPtr sender);
    void AddPlayer(PlayerControllerPtr player);
    void RemovePlayer(PlayerControllerPtr player);
    void BroadcastMessage(const std::string& message, PlayerControllerPtr except = nullptr);

    /// @brief Disconnects and removes all players, then calls OnShutdown.
    void Shutdown();

    /// @brief Returns the number of connected players. Safe to call only from strand_.
    [[nodiscard]] std::size_t GetPlayerCount() const;

    /// @brief Check if a player is in the session. Safe to call only from strand_.
    [[nodiscard]] bool HasPlayer(const PlayerControllerPtr& player) const;

  protected:
    virtual void ProcessCommand(const Command& command, PlayerControllerPtr sender);
    virtual void OnPlayerJoined(PlayerControllerPtr player);
    virtual void OnPlayerLeft(PlayerControllerPtr player);
    virtual void OnShutdown();

  private:
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    std::vector<PlayerControllerPtr> player_controllers_;
};

}  // namespace game::core

#include "game_session.cpp"

#endif  // APPS_SIMPLE_SERVER_GAME_SESSION