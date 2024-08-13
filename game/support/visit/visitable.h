#ifndef GAME_SUPPORT_VISIT_VISITABLE_H
#define GAME_SUPPORT_VISIT_VISITABLE_H

#include <string_view>
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

template <typename Derived>
class VisitorBase
{
  public:
    template <typename T>
    void visit(std::string_view name, T& value)
    {
        derived_->template visit(name, value);
    }

    auto visit_nested(std::string_view name)
    {
        derived_->visit_nested(name);
    }

    auto exit_nested(std::string_view name)
    {
        derived_->exit_nested(name);
    }

  private:
    Derived* derived_ = static_cast<Derived*>(this);
};

// Visitable mixin
template <typename InstanceT, typename... Properties>
class Visitable : public VisitableTag
{
  public:
    constexpr Visitable(Properties&&... properties)
        : props_(std::forward<Properties>(properties)...)
    {
    }

    template <typename Visitor>
    void accept(Visitor&& visitor, InstanceT& instance)
    {
        constexpr auto prop_size = std::tuple_size<decltype(props_)>::value;
        visit_foreach_property(
            std::forward<Visitor>(visitor),
            instance,
            std::make_index_sequence<prop_size> {});
    }

  private:
    template <typename Visitor, typename PropertyT>
    void visit_property(Visitor&& visitor, InstanceT& instance, const PropertyT& prop)
    {
        if constexpr (std::is_base_of_v<
                          game::support::VisitableTag,
                          std::decay_t<InstanceT>>)
        {
            std::string_view name {prop.name_};
            visitor.visit_nested(name);

            auto& nested_instance = prop.Get(instance);
            nested_instance.accept(visitor, nested_instance);

            visitor.exit_nested(name);
        }
        else
        {
            visitor.visit(prop.name_, prop.Get(instance));
        }
    }

    template <typename Visitor, std::size_t... Is>
    void visit_foreach_property(
        Visitor&& visitor, InstanceT& instance, std::index_sequence<Is...>)
    {
        (visit_property(visitor, instance, std::get<Is>(props_)), ...);
    }

    std::tuple<Properties...> props_;
};

template <typename InstanceT, typename... Properties>
constexpr inline auto visitable(Properties&&... p)
{
    return Visitable<InstanceT, Properties...> {std::forward<Properties>(p)...};
}

}  // namespace game::support
#endif  // GAME_SUPPORT_VISIT_VISITABLE_H
