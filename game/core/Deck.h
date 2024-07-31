#ifndef GAME_CORE_DECK_H
#define GAME_CORE_DECK_H

#include <algorithm>
#include <random>
#include <vector>

#include "core/Card.h"

class Deck
{
  public:
    explicit Deck(std::vector<Card> cards) : cards_(std::move(cards)) {}
    Deck() = default;

    // Just delete everything and enable when needed
    Deck(const Deck&) = delete;
    Deck(Deck&&) = delete;
    Deck& operator=(const Deck&) = delete;
    Deck& operator=(Deck&&) = delete;

    bool IsSorted() const { return std::is_sorted(cards_.begin(), cards_.end()); }
    void Sort() { std::sort(cards_.begin(), cards_.end()); }

    void Shuffle()
    {
        std::random_device rd;
        std::default_random_engine rng(rd());

        std::shuffle(
            cards_.begin(),
            cards_.end(),
            std::default_random_engine{std::random_device{}()}
        );
    }

  private:
    std::vector<Card> cards_{};
};

#endif  // GAME_CORE_DECK_H
