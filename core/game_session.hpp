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

    explicit GameSession(boost::asio::io_context& io_ctx) : strand_(boost::asio::make_strand(io_ctx)) {}
    GameSession(const GameSession&) = delete;
    GameSession(GameSession&&) = delete;
    auto operator=(const GameSession&) -> GameSession& = delete;
    auto operator=(GameSession&&) -> GameSession& = delete;

    virtual ~GameSession() = default;

    void OnCommand(const Command& command, PlayerControllerPtr sender)
    {
        boost::asio::post(strand_, [this, command, sender]() { ProcessCommand(command, sender); });
    }

    void AddPlayer(PlayerControllerPtr player)
    {
        boost::asio::post(strand_, [this, player]() {
            // Avoid duplicate players
            auto it = std::find(player_controllers_.begin(), player_controllers_.end(), player);
            if (it != player_controllers_.end())
            {
                return;
            }
            player_controllers_.push_back(player);
            boost::asio::post(strand_, [this, player]() { OnPlayerJoined(player); });
        });
    }

    void RemovePlayer(PlayerControllerPtr player)
    {
        boost::asio::post(strand_, [this, player]() {
            auto it = std::find(player_controllers_.begin(), player_controllers_.end(), player);
            if (it != player_controllers_.end())
            {
                player_controllers_.erase(it);
                // Notify after erase so the player is no longer in the list
                // when OnPlayerLeft runs on the command strand
                boost::asio::post(strand_, [this, player]() { OnPlayerLeft(player); });
            }
        });
    }

    void BroadcastMessage(const std::string& message, PlayerControllerPtr except = nullptr)
    {
        boost::asio::post(strand_, [this, message, except]() {
            for (auto& player : player_controllers_)
            {
                if (player != except)
                {
                    player->SendMessage(message);
                }
            }
        });
    }

    /// @brief Disconnects and removes all players, then calls OnShutdown.
    void Shutdown()
    {
        boost::asio::post(strand_, [this]() {
            for (auto& player : player_controllers_)
            {
                player->Disconnect();
            }
            player_controllers_.clear();
            boost::asio::post(strand_, [this]() { OnShutdown(); });
        });
    }

    /// @brief Returns the number of connected players. Safe to call only from strand_.
    [[nodiscard]] std::size_t GetPlayerCount() const { return player_controllers_.size(); }

    /// @brief Check if a player is in the session. Safe to call only from strand_.
    [[nodiscard]] bool HasPlayer(const PlayerControllerPtr& player) const
    {
        return std::find(player_controllers_.begin(), player_controllers_.end(), player) != player_controllers_.end();
    }

  protected:
    virtual void ProcessCommand(const Command& command, PlayerControllerPtr sender) {}
    virtual void OnPlayerJoined(PlayerControllerPtr player) {}
    virtual void OnPlayerLeft(PlayerControllerPtr player) {}
    virtual void OnShutdown() {}

  private:
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    std::vector<PlayerControllerPtr> player_controllers_;
};

}  // namespace game::core

#endif  // APPS_SIMPLE_SERVER_GAME_SESSION