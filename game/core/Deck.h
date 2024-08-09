#ifndef GAME_CORE_DECK_H
#define GAME_CORE_DECK_H

#include <vector>

#include "core/Card.h"

namespace game::core
{

class Deck
{
  public:
    explicit Deck(std::vector<Card> cards) : cards_(std::move(cards)) {}
    Deck() = default;
    ~Deck() = default;

    // Just delete everything and enable when needed
    Deck(const Deck&) = delete;
    Deck(Deck&&) = delete;
    Deck& operator=(const Deck&) = delete;
    Deck& operator=(Deck&&) = delete;

    [[nodiscard]] bool IsSorted() const;

    void Sort();

    void Shuffle();

    // adds a card to the top of the deck
    void AddCard(Card&& card);

    // removes a card from the top of the deck
    [[__nodiscard__]] Card PickCard();

  private:
    std::vector<Card> cards_{};
};
}  // namespace game::core
#endif  // GAME_CORE_DECK_H
