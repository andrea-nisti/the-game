#ifndef GAME_CORE_PLAYER_MANAGER_HPP
#define GAME_CORE_PLAYER_MANAGER_HPP

#include <boost/asio.hpp>
#include <algorithm>
#include <functional>
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

    explicit PlayerManager(boost::asio::io_context& io_ctx);

    /// @brief Adds a player if not already present. Calls on_added(player) on the strand after insertion.
    void Add(PlayerControllerPtr player, std::function<void(PlayerControllerPtr)> on_added = nullptr);

    /// @brief Removes a player if present. Calls on_removed(player) on the strand after removal.
    void Remove(PlayerControllerPtr player, std::function<void(PlayerControllerPtr)> on_removed = nullptr);

    /// @brief Broadcasts a message to all players except the optional excluded one.
    void Broadcast(const std::string& message, PlayerControllerPtr except = nullptr);

    /// @brief Disconnects all players and clears the list. Calls on_cleared() when done.
    void Clear(std::function<void()> on_cleared = nullptr);

    /// @brief Posts a query — invokes callback with the current player count.
    void QueryCount(std::function<void(std::size_t)> callback) const;

    /// @brief Posts a query — invokes callback with a snapshot of the player list.
    void QuerySnapshot(std::function<void(PlayerList)> callback) const;

  private:
    mutable boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    PlayerList players_;
};

}  // namespace game::core

#include "players_manager.cpp"

#endif  // GAME_CORE_PLAYER_MANAGER_HPP
