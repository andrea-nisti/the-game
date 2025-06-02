#ifndef SUPPORT_GAMES_PLAYER
#define SUPPORT_GAMES_PLAYER

#include <string>

#include "Deck.h"

namespace game::core {
class Player
{
  public:
    explicit Player(std::string name)
        : uid_(++Player::n_of_players_), name_(std::move(name))
    {}
    ~Player() = default;

    // Just delete everything and enable when needed
    Player(const Player&) = delete;
    Player(Player&&) = delete;
    auto operator=(const Player&) -> Player& = delete;
    auto operator=(Player&&) -> Player& = delete;

  private:
    inline static int n_of_players_ {0};
    const int uid_ {};
    const std::string name_ {};
    Hand hand_ {};
};
}  // namespace game::core
#endif  // SUPPORT_GAMES_PLAYER
