#ifndef GAME_CORE_PLAYER_MANAGER_IMPL
#define GAME_CORE_PLAYER_MANAGER_IMPL

namespace game::core {

template <typename Command>
PlayerManager<Command>::PlayerManager(boost::asio::io_context& io_ctx) : strand_(boost::asio::make_strand(io_ctx))
{
}

template <typename Command>
void PlayerManager<Command>::Add(PlayerControllerPtr player, std::function<void(PlayerControllerPtr)> on_added)
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

template <typename Command>
void PlayerManager<Command>::Remove(PlayerControllerPtr player, std::function<void(PlayerControllerPtr)> on_removed)
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

template <typename Command>
void PlayerManager<Command>::Broadcast(const std::string& message, PlayerControllerPtr except)
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

template <typename Command>
void PlayerManager<Command>::Clear(std::function<void()> on_cleared)
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

template <typename Command>
void PlayerManager<Command>::QueryCount(std::function<void(std::size_t)> callback) const
{
    boost::asio::post(strand_, [this, callback = std::move(callback)]() { callback(players_.size()); });
}

template <typename Command>
void PlayerManager<Command>::QuerySnapshot(std::function<void(PlayerList)> callback) const
{
    boost::asio::post(strand_, [this, callback = std::move(callback)]() { callback(players_); });
}

}  // namespace game::core

#endif  // GAME_CORE_PLAYER_MANAGER_IMPL
