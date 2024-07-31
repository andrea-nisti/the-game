#ifndef GAME_CORE_PLAYER_H
#define GAME_CORE_PLAYER_H

#include <string>

#include "core/Deck.h"

class Player
{

  public:
    explicit Player(std::string name)
        : uid_(++Player::n_of_players_), name_(std::move(name))
    {
    }
    
    // Just delete everything and enable when needed
    Player(const Player&) = delete;
    Player(Player&&) = delete;
    Player& operator=(const Player&) = delete;
    Player& operator=(Player&&) = delete;

  private:
    inline static int n_of_players_{};
    const int uid_{};
    const std::string name_{};
    Deck hand_{};
};

#endif  // GAME_CORE_PLAYER_H
