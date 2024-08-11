#include <cstddef>
#include <iostream>
#include <unordered_map>

#include <gtest/gtest.h>

#include "core/Card.h"
#include "core/Deck.h"

// Demonstrate some basic assertions.
TEST(DeckTest_IsSorted, AddNotSortedCards_ExpectIsSortedFalse)
{
    game::core::Deck d{};
    d.AddCard(game::core::Card{2});
    d.AddCard(game::core::Card{1});

    ASSERT_FALSE(d.IsSorted());
}

TEST(DeckTest_IsSorted, AddSortedCards_ExpectIsSortedTrue)
{
    game::core::Deck d{};
    d.AddCard(game::core::Card{1});
    d.AddCard(game::core::Card{2});
    d.AddCard(game::core::Card{3});

    ASSERT_TRUE(d.IsSorted());
}

TEST(DeckTest_AddPickCard, AddCards_ExpectSizeCorrectAndUniqueCards)
{
    constexpr auto n{100};
    auto deck = game::core::Deck::Build(n);

    std::unordered_map<game::core::Card::value_t, int> value_counts{};
    while (not deck.IsEmpty())
    {
        auto card = deck.PickCard();
        ++value_counts[card.GetValue()];
    }

    ASSERT_EQ(value_counts.size(), n);

    for (const auto p : value_counts)
    {
        ASSERT_EQ(p.second, 1);
    }
}