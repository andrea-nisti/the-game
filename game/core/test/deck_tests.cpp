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