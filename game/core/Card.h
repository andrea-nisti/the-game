#ifndef GAME_CORE_CARD_H
#define GAME_CORE_CARD_H

#include <cstdint>

class Card
{
  public:
    Card(std::int8_t val) : value_(val) {}

    bool operator<(const Card& other) const { return value_ < other.value_; }
    bool operator==(const Card& other) const { return value_ == other.value_; }

    std::int8_t GetValue() { return value_; }

  private:
    std::int8_t value_{};
};

#endif  // GAME_CORE_CARD_H
