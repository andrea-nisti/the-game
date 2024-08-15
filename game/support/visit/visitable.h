/**
 * @file visit_visitable.h
 * @brief Defines classes and utilities for implementing the Visitor pattern.
 */

#ifndef GAME_SUPPORT_VISIT_VISITABLE_H
#define GAME_SUPPORT_VISIT_VISITABLE_H

#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

namespace game::support
{

/**
 * \addtogroup support Support
 * \brief Supporting classes and functions
 * \ingroup support
 * @{
 */

/**
 * @brief Interface (or simple tag) class for visitable objects.
 */
struct IVisitable
{
};

/**
 * @brief Type trait to check if a type is derived from IVisitable.
 * @tparam T The type to check.
 */
template <typename T>
inline constexpr bool is_i_visitable_v = std::is_base_of_v<IVisitable, std::decay_t<T>>;

/**
 * @brief Base class for visitors. This class uses CRTP pattern to forward function calls
 * to the Derived class
 * @tparam Derived The derived visitor class.
 */
template <typename Derived>
class VisitorBase
{
  public:
    /**
     * @brief Visit a named value. This is called by the visitable everytime an element is
     * visited.
     * @tparam T The type of the value.
     * @param name The name of the visited element.
     * @param value The visited value.
     */
    template <typename T>
    void visit(std::string_view name, T& value)
    {
        derived_->template visit(name, value);
    }

    /**
     * @brief Enter a nested visitable object. This is called by the visitable everytime a
     * visitable object (or nested) is visited.
     * @param name The name of the nested object.
     * @return Implementation-defined.
     */
    void visit_nested(std::string_view name) { derived_->visit_nested(name); }

    /**
     * @brief Exit a nested visitable object. This is called by the visitable everytime a
     * visiting a visitable object (or nested) is over.
     * @param name The name of the nested object.
     * @return Implementation-defined.
     */
    void exit_nested(std::string_view name) { derived_->exit_nested(name); }

  private:
    Derived* derived_ = static_cast<Derived*>(this);
};

/**
 * @brief Class to make a type visitable.
 * Look at tests for usage examples. This class
 * creates a visitable object that defines how the instance of an object should be
 * visited.
 * @tparam InstanceT The type of the instance being made visitable.
 * @tparam Properties The property types for the visitable object.
 */
template <typename InstanceT, typename... Properties>
class Visitable : public IVisitable
{
  public:
    constexpr Visitable(Properties&&... properties)
        : props_(std::forward<Properties>(properties)...)
    {
    }

    /**
     * @brief Accepts a visitor and triggers the visiting chain by calling the Visitor
     * callbacks when with the instance params.
     * @tparam Visitor The type of the visitor.
     * @param visitor The visitor object.
     * @param instance The instance being visited.
     */
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
        if constexpr (is_i_visitable_v<InstanceT>)
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

/**
 * @brief Helper function to create a Visitable object.
 * @tparam InstanceT The type of the instance being made visitable.
 * @tparam Properties The property types for the visitable object.
 * @param p The properties of the visitable object.
 * @return A Visitable object.
 */
template <typename InstanceT, typename... Properties>
constexpr inline auto visitable(Properties&&... p)
{
    return Visitable<InstanceT, Properties...> {std::forward<Properties>(p)...};
}

/**
 * @}
 */

}  // namespace game::support
#endif  // GAME_SUPPORT_VISIT_VISITABLE_H
