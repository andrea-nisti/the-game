#include "Deck.h"

#include <algorithm>
#include <random>

using namespace game::core;

bool Deck::IsSorted() const
{
    return std::is_sorted(cards_.begin(), cards_.end());
}

bool Deck::IsEmpty() const
{
    return cards_.empty();
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
        cards_.begin(), cards_.end(), std::default_random_engine{std::random_device{}()});
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

const Card& Deck::GetTopCard() const
{
    return cards_.back();
}

std::size_t Deck::GetSize() const
{
    return cards_.size();
}

const Hand& Deck::GetCards() const
{
    return cards_;
}
