#ifndef GAME_SUPPORT_VISIT_PROPERTY_H
#define GAME_SUPPORT_VISIT_PROPERTY_H

namespace game::support
{

template <typename Class, typename MemberType>
struct Property
{
    using Type = MemberType;

    constexpr Property(const char* name, MemberType Class::*member)
        : name_ {name}, member_ {member}
    {
    }

    Type& Get(Class& instance) const
    {
        return instance.*member_;
    }

    const char* name_;
    MemberType Class::*member_;
};

template <typename Class, typename MemberType>
constexpr inline auto property(const char* name, MemberType Class::*member)
{
    return Property<Class, MemberType> {name, member};
}

}  // namespace game::support
#endif  // GAME_SUPPORT_VISIT_PROPERTY_H
