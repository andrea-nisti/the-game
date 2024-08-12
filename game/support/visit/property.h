#ifndef GAME_SUPPORT_VISIT_PROPERTY_H
#define GAME_SUPPORT_VISIT_PROPERTY_H

#include <functional>

namespace game::support
{

template <typename Class, typename PropertyType>
struct Property
{
    using Getter = std::function<PropertyType(const Class&)>;
    using Type = PropertyType;

    constexpr Property(Getter getter, const char* name)
        : getter_{std::move(getter)}, name_{name}
    {
    }

    constexpr Property(const char* name) : name_{name} {}

    Getter getter_{};
    const char* name_;
};

// template <typename Class, typename PropertyType>
// constexpr inline auto property(
//     std::function<PropertyType(const Class&)> get, const char* name
// )
// {
//     return Property<Class, PropertyType>{std::move(get), name};
// }

template <typename Class, typename PropertyType>
constexpr inline auto property(const char* name)
{
    return Property<Class, PropertyType>{name};
}

}  // namespace game::support
#endif  // GAME_SUPPORT_VISIT_PROPERTY_H
