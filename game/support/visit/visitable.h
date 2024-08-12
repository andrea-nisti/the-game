#ifndef GAME_SUPPORT_VISIT_VISITABLE_H
#define GAME_SUPPORT_VISIT_VISITABLE_H

#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

namespace game::support
{

namespace
{

struct VisitableTag
{
};

}  // namespace

struct VisitorBase
{
    template <typename Visitor, typename Visitable>
    auto as(Visitable&& visitable
    ) -> std::enable_if_t<std::is_base_of_v<Visitable, VisitableTag>>
    {
        as_visitable(std::forward<Visitable>(visitable));
    }

  private:
    template <typename Visitable>
    auto as_visitable(Visitable&& visitable)
    {
        constexpr auto prop_size = std::tuple_size<decltype(visitable.props_)>::value;
        visitable.visit_impl(this, std::make_index_sequence<prop_size>{});
    }
};

// template <typename T, T... S, typename F>
// constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f)
// {
//     (static_cast<void>(f(std::integral_constant<T, S>{})), ...);
// }

// Visitable mixin
template <typename... Properties>
class Visitable : private VisitableTag
{
  public:
    constexpr Visitable(Properties&&... properties)
        : props_(std::forward<Properties>(properties)...)
    {
    }

  private:
    template <typename Visitor, std::size_t... Is>
    void visit_impl(Visitor&& v, std::index_sequence<Is...>) const
    {
        std::string_view field_name{};
        // ((field_name = std::get<Is>(props_).name_, std::cout << field_name <<
        // std::endl),
        //  ...);

        (void)std::initializer_list<int>{
            (field_name = std::get<Is>(props_).name_,
             std::cout << field_name << std::endl,
             v.as(std::get<Is>(props_)::Type),
             0)...
        };
    }

    using visitable_t = Visitable<Properties...>;
    friend VisitorBase;

    std::tuple<Properties...> props_;
};

template <typename... Properties>
constexpr inline auto visitable(Properties&&... p)
{
    return Visitable{std::forward<Properties>(p)...};
}

}  // namespace game::support
#endif  // GAME_SUPPORT_VISIT_VISITABLE_H
