#include <gtest/gtest.h>

#include "core/Card.h"
#include "core/Deck.h"

// Demonstrate some basic assertions.
TEST(DeckTest_IsSorted, AddNotSortedCards_ExpectIsSortedFalse)
{
    // This is a Deck
    game::core::Deck d{};
    d.AddCard(game::core::Card{2});
    d.AddCard(game::core::Card{1});

    ASSERT_FALSE(d.IsSorted());
}