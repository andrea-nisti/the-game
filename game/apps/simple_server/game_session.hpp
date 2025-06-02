#ifndef APPS_SIMPLE_SERVER_GAME_SESSION
#define APPS_SIMPLE_SERVER_GAME_SESSION

#include <algorithm>
#include <memory>
#include <vector>

#include <boost/asio.hpp>

namespace game::core {

template <typename Command>
class PlayerControllerBase
{
  public:
    virtual ~PlayerControllerBase() = default;
    virtual void SendMessage(const std::string& message) = 0;
};

template <typename Command>
class GameSession
{
  public:
    using PlayerControllerPtr = std::shared_ptr<PlayerControllerBase<Command>>;

    explicit GameSession(boost::asio::io_context& io_ctx)
        : command_strand_(boost::asio::make_strand(io_ctx)),
          player_strand_(boost::asio::make_strand(io_ctx))
    {}
    GameSession(const GameSession&) = delete;
    GameSession(GameSession&&) = delete;
    auto operator=(const GameSession&) -> GameSession& = delete;
    auto operator=(GameSession&&) -> GameSession& = delete;

    ~GameSession() = default;

    void OnCommand(const Command& command, PlayerControllerPtr sender)
    {
        // Game commands go through the command strand
        boost::asio::post(
            command_strand_,
            [this, command, sender]() { ProcessCommand(command, sender); });
    }

    void AddPlayer(PlayerControllerPtr player)
    {
        // Player management goes through the player strand
        boost::asio::post(
            player_strand_,
            [this, player]()
            {
                players_.push_back(player);
                // Notify game logic about new player through command strand
                boost::asio::post(
                    command_strand_, [this, player]() { OnPlayerJoined(player); });
            });
    }

    void RemovePlayer(PlayerControllerPtr player)
    {
        // Player management goes through the player strand
        boost::asio::post(
            player_strand_,
            [this, player]()
            {
                auto it = std::find(players_.begin(), players_.end(), player);
                if (it != players_.end())
                {
                    // Notify game logic about player leaving through command strand
                    boost::asio::post(
                        command_strand_, [this, player]() { OnPlayerLeft(player); });
                    players_.erase(it);
                }
            });
    }

    void BroadcastMessage(
        const std::string& message, PlayerControllerPtr except = nullptr)
    {
        // Broadcasting goes through player strand since it accesses the player list
        boost::asio::post(
            player_strand_,
            [this, message, except]()
            {
                for (auto& player : players_)
                {
                    if (player != except)
                    {
                        player->SendMessage(message);
                    }
                }
            });
    }

  protected:
    virtual void ProcessCommand(const Command& command, PlayerControllerPtr sender) {}
    virtual void OnPlayerJoined(PlayerControllerPtr player) {}
    virtual void OnPlayerLeft(PlayerControllerPtr player) {}

  private:
    boost::asio::strand<boost::asio::io_context::executor_type> command_strand_;
    boost::asio::strand<boost::asio::io_context::executor_type> player_strand_;
    std::vector<PlayerControllerPtr> players_;
};

}  // namespace game::core

#endif  // APPS_SIMPLE_SERVER_GAME_SESSION
