#include "Deck.h"

#include <algorithm>
#include <random>

using namespace game::core;

bool Deck::IsSorted() const
{
    return std::is_sorted(cards_.begin(), cards_.end());
}

void Deck::Sort()
{
    std::sort(cards_.begin(), cards_.end());
}

void Deck::Shuffle()
{
    std::random_device rd;
    std::default_random_engine rng(rd());

    std::shuffle(
        cards_.begin(), cards_.end(), std::default_random_engine{std::random_device{}()}
    );
}

void Deck::AddCard(Card&& card)
{
    cards_.push_back(std::move(card));
}

Card Deck::PickCard()
{
    auto card = std::move(cards_.back());
    cards_.pop_back();
    return card;
}
