#ifndef APPS_SIMPLE_SERVER_GAME_SESSION_IMPL
#define APPS_SIMPLE_SERVER_GAME_SESSION_IMPL

namespace game::core {

template <typename Command>
GameSession<Command>::GameSession(boost::asio::io_context& io_ctx) : strand_(boost::asio::make_strand(io_ctx))
{
}

template <typename Command>
void GameSession<Command>::OnCommand(const Command& command, PlayerControllerPtr sender)
{
    boost::asio::post(strand_, [this, command, sender]() { ProcessCommand(command, sender); });
}

template <typename Command>
void GameSession<Command>::AddPlayer(PlayerControllerPtr player)
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

template <typename Command>
void GameSession<Command>::RemovePlayer(PlayerControllerPtr player)
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

template <typename Command>
void GameSession<Command>::BroadcastMessage(const std::string& message, PlayerControllerPtr except)
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

template <typename Command>
void GameSession<Command>::Shutdown()
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

template <typename Command>
std::size_t GameSession<Command>::GetPlayerCount() const
{
    return player_controllers_.size();
}

template <typename Command>
bool GameSession<Command>::HasPlayer(const PlayerControllerPtr& player) const
{
    return std::find(player_controllers_.begin(), player_controllers_.end(), player) != player_controllers_.end();
}

template <typename Command>
void GameSession<Command>::ProcessCommand(const Command& /*command*/, PlayerControllerPtr /*sender*/)
{
}

template <typename Command>
void GameSession<Command>::OnPlayerJoined(PlayerControllerPtr /*player*/)
{
}

template <typename Command>
void GameSession<Command>::OnPlayerLeft(PlayerControllerPtr /*player*/)
{
}

template <typename Command>
void GameSession<Command>::OnShutdown()
{
}

}  // namespace game::core

#endif  // APPS_SIMPLE_SERVER_GAME_SESSION_IMPL
