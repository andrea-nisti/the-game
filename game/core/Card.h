#ifndef GAME_CORE_CARD_H
#define GAME_CORE_CARD_H

#include <cstddef>

namespace game::core
{
class Card
{
  public:
    using value_t = std::size_t;
    Card(std::size_t val) : value_(val) {}

    bool operator<(const Card& other) const
    {
        return value_ < other.value_;
    }
    bool operator==(const Card& other) const
    {
        return value_ == other.value_;
    }

    std::size_t GetValue()
    {
        return value_;
    }

  private:
    std::size_t value_{};
};
}  // namespace game::core

#endif  // GAME_CORE_CARD_H
