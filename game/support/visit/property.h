#ifndef GAME_SUPPORT_VISIT_PROPERTY_H
#define GAME_SUPPORT_VISIT_PROPERTY_H

namespace game::support
{

template <typename Class, typename MemberType>
struct Property
{
    using Type = MemberType;

    constexpr Property(MemberType Class::*member, const char* name)
        : member_ {member}, name_ {name}
    {
    }
    constexpr Property(const char* name) : name_ {name} {}

    Type& Get(Class& instance) const
    {
        return instance.*member_;
    }

    MemberType Class::*member_;
    const char* name_;
};

template <typename Class, typename PropertyType>
constexpr inline auto property(const char* name)
{
    return Property<Class, PropertyType> {name};
}

}  // namespace game::support
#endif  // GAME_SUPPORT_VISIT_PROPERTY_H
