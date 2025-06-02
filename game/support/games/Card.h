#ifndef SUPPORT_GAMES_CARD
#define SUPPORT_GAMES_CARD

#include <cstddef>

namespace game::core {
class Card
{
  public:
    using value_t = std::size_t;
    Card(std::size_t val) : value_(val) {}

    auto operator<(const Card& other) const -> bool { return value_ < other.value_; }
    auto operator==(const Card& other) const -> bool { return value_ == other.value_; }

    auto GetValue() -> std::size_t { return value_; }

  private:
    std::size_t value_ {};
};
}  // namespace game::core

#endif  // SUPPORT_GAMES_CARD
