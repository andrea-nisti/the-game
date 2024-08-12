#ifndef GAME_CORE_DECK_H
#define GAME_CORE_DECK_H

#include <cstddef>
#include <iostream>
#include <vector>

#include "core/Card.h"

namespace game::core
{

using Hand = std::vector<Card>;

class Deck
{
  public:
    explicit Deck(Hand cards) : cards_(std::move(cards)) {}
    Deck() = default;
    ~Deck() = default;

    // Just delete everything and enable when needed
    Deck(const Deck&) = delete;
    Deck(Deck&&) = default;
    Deck& operator=(const Deck&) = delete;
    Deck& operator=(Deck&&) = delete;

    // Creational utilities
    static Deck Build(std::size_t n_of_cards)
    {
        Deck deck;

        for (std::size_t i{1}; i <= n_of_cards; ++i)
        {
            deck.AddCard(Card{i});
        }
        deck.Shuffle();
        return deck;
    }

    Hand::iterator begin()
    {
        return cards_.begin();
    }

    Hand::iterator end()
    {
        return cards_.end();
    }

    [[nodiscard]] bool IsSorted() const;
    [[nodiscard]] bool IsEmpty() const;
    [[nodiscard]] std::size_t GetSize() const;
    [[nodiscard]] const Hand& GetCards() const;

    void Sort();
    void Shuffle();

    // adds a card to the top of the deck
    void AddCard(Card&& card);

    // removes a card from the top of the deck
    [[nodiscard]] Card PickCard();

    [[nodiscard]] const Card& GetTopCard() const;

  private:
    Hand cards_{};
};
}  // namespace game::core
#endif  // GAME_CORE_DECK_H
