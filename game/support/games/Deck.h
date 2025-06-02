#ifndef SUPPORT_GAMES_DECK
#define SUPPORT_GAMES_DECK

#include <cstddef>
#include <vector>

#include "Card.h"

namespace game::core {

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
    auto operator=(const Deck&) -> Deck& = delete;
    auto operator=(Deck&&) -> Deck& = delete;

    // Creational utilities
    static auto Build(std::size_t n_of_cards) -> Deck
    {
        Deck deck;

        for (std::size_t i {1}; i <= n_of_cards; ++i)
        {
            deck.AddCard(Card {i});
        }
        deck.Shuffle();
        return deck;
    }

    auto begin() -> Hand::iterator { return cards_.begin(); }

    auto end() -> Hand::iterator { return cards_.end(); }

    [[nodiscard]] auto IsSorted() const -> bool;
    [[nodiscard]] auto IsEmpty() const -> bool;
    [[nodiscard]] std::size_t GetSize() const;
    [[nodiscard]] const Hand& GetCards() const;

    void Sort();
    void Shuffle();

    // adds a card to the top of the deck
    void AddCard(Card&& card);

    // removes a card from the top of the deck
    [[nodiscard]] auto PickCard() -> Card;

    [[nodiscard]] auto GetTopCard() const -> const Card&;

  private:
    Hand cards_ {};
};
}  // namespace game::core
#endif  // SUPPORT_GAMES_DECK
