#ifndef GAME_CORE_PLAYER_MANAGER_HPP
#define GAME_CORE_PLAYER_MANAGER_HPP

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

/// @brief Manages the player list on its own strand.
/// All public methods are safe to call from any thread.
template <typename Command>
class PlayerManager
{
  public:
    using PlayerControllerPtr = std::shared_ptr<PlayerControllerBase<Command>>;
    using PlayerList = std::vector<PlayerControllerPtr>;

    explicit PlayerManager(boost::asio::io_context& io_ctx) : strand_(boost::asio::make_strand(io_ctx)) {}

    /// @brief Adds a player if not already present. Calls on_added(player) on the strand after insertion.
    void Add(PlayerControllerPtr player, std::function<void(PlayerControllerPtr)> on_added = nullptr)
    {
        boost::asio::post(strand_, [this, player, on_added = std::move(on_added)]() {
            if (std::find(players_.begin(), players_.end(), player) != players_.end())
            {
                return;
            }
            players_.push_back(player);
            if (on_added)
            {
                on_added(player);
            }
        });
    }

    /// @brief Removes a player if present. Calls on_removed(player) on the strand after removal.
    void Remove(PlayerControllerPtr player, std::function<void(PlayerControllerPtr)> on_removed = nullptr)
    {
        boost::asio::post(strand_, [this, player, on_removed = std::move(on_removed)]() {
            auto it = std::find(players_.begin(), players_.end(), player);
            if (it == players_.end())
            {
                return;
            }
            players_.erase(it);
            if (on_removed)
            {
                on_removed(player);
            }
        });
    }

    /// @brief Broadcasts a message to all players except the optional excluded one.
    void Broadcast(const std::string& message, PlayerControllerPtr except = nullptr)
    {
        boost::asio::post(strand_, [this, message, except]() {
            for (auto& player : players_)
            {
                if (player != except)
                {
                    player->SendMessage(message);
                }
            }
        });
    }

    /// @brief Disconnects all players and clears the list. Calls on_cleared() when done.
    void Clear(std::function<void()> on_cleared = nullptr)
    {
        boost::asio::post(strand_, [this, on_cleared = std::move(on_cleared)]() {
            for (auto& player : players_)
            {
                player->Disconnect();
            }
            players_.clear();
            if (on_cleared)
            {
                on_cleared();
            }
        });
    }

    /// @brief Posts a query — invokes callback with the current player count.
    void QueryCount(std::function<void(std::size_t)> callback) const
    {
        boost::asio::post(strand_, [this, callback = std::move(callback)]() { callback(players_.size()); });
    }

    /// @brief Posts a query — invokes callback with a snapshot of the player list.
    void QuerySnapshot(std::function<void(PlayerList)> callback) const
    {
        boost::asio::post(strand_, [this, callback = std::move(callback)]() { callback(players_); });
    }

  private:
    mutable boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    PlayerList players_;
};

}  // namespace game::core

#endif  // GAME_CORE_PLAYER_MANAGER_HPP
