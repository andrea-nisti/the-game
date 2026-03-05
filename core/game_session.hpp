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
class GameSession
{
  public:
    using PlayerControllerPtr = std::shared_ptr<PlayerControllerBase<Command>>;

    explicit GameSession(boost::asio::io_context& io_ctx)
        : command_strand_(boost::asio::make_strand(io_ctx)), player_strand_(boost::asio::make_strand(io_ctx))
    {
    }
    GameSession(const GameSession&) = delete;
    GameSession(GameSession&&) = delete;
    auto operator=(const GameSession&) -> GameSession& = delete;
    auto operator=(GameSession&&) -> GameSession& = delete;

    virtual ~GameSession() = default;

    void OnCommand(const Command& command, PlayerControllerPtr sender)
    {
        boost::asio::post(command_strand_, [this, command, sender]() { ProcessCommand(command, sender); });
    }

    void AddPlayer(PlayerControllerPtr player)
    {
        boost::asio::post(player_strand_, [this, player]() {
            // Avoid duplicate players
            auto it = std::find(players_.begin(), players_.end(), player);
            if (it != players_.end())
            {
                return;
            }
            players_.push_back(player);
            boost::asio::post(command_strand_, [this, player]() { OnPlayerJoined(player); });
        });
    }

    void RemovePlayer(PlayerControllerPtr player)
    {
        boost::asio::post(player_strand_, [this, player]() {
            auto it = std::find(players_.begin(), players_.end(), player);
            if (it != players_.end())
            {
                players_.erase(it);
                // Notify after erase so the player is no longer in the list
                // when OnPlayerLeft runs on the command strand
                boost::asio::post(command_strand_, [this, player]() { OnPlayerLeft(player); });
            }
        });
    }

    void BroadcastMessage(const std::string& message, PlayerControllerPtr except = nullptr)
    {
        boost::asio::post(player_strand_, [this, message, except]() {
            for (auto& player : players_)
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
        boost::asio::post(player_strand_, [this]() {
            for (auto& player : players_)
            {
                player->Disconnect();
            }
            players_.clear();
            boost::asio::post(command_strand_, [this]() { OnShutdown(); });
        });
    }

    /// @brief Returns the number of connected players. Safe to call only from player_strand_.
    [[nodiscard]] std::size_t GetPlayerCount() const { return players_.size(); }

    /// @brief Check if a player is in the session. Safe to call only from player_strand_.
    [[nodiscard]] bool HasPlayer(const PlayerControllerPtr& player) const
    {
        return std::find(players_.begin(), players_.end(), player) != players_.end();
    }

  protected:
    virtual void ProcessCommand(const Command& command, PlayerControllerPtr sender) {}
    virtual void OnPlayerJoined(PlayerControllerPtr player) {}
    virtual void OnPlayerLeft(PlayerControllerPtr player) {}
    virtual void OnShutdown() {}

  private:
    boost::asio::strand<boost::asio::io_context::executor_type> command_strand_;
    boost::asio::strand<boost::asio::io_context::executor_type> player_strand_;
    std::vector<PlayerControllerPtr> players_;
};

}  // namespace game::core

#endif  // APPS_SIMPLE_SERVER_GAME_SESSION