#ifndef GAME_SUPPORT_VISIT_PROPERTY_H
#define GAME_SUPPORT_VISIT_PROPERTY_H

namespace game::support
{

/**
 * \addtogroup support Support
 * \brief Supporting classes and functions
 * @{
 */

/**
 * @brief Represents a visitable property of a struct.
 *
 * This struct template allows for the creation of named ***public*** properties that are
 * members of a specific struct.
 *
 * @tparam Class The class that contains the member.
 * @tparam MemberType The type of the member.
 */
template <typename Class, typename MemberType>
struct Property
{
    /** @brief Alias for the member type. */
    using Type = MemberType;

    /**
     * @brief Constructs a Property object.
     *
     * @param name The name of the property.
     * @param member Pointer to the member of the class.
     */
    constexpr Property(const char* name, MemberType Class::*member)
        : name_ {name}, member_ {member}
    {
    }

    /**
     * @brief Gets the modifiable reference of the member described by a property for a
     * given instance.
     *
     * @param instance The instance of the class.
     * @return Type& Reference to the member value.
     */
    Type& Get(Class& instance) const { return instance.*member_; }

    /** The name of the property. This usually matches the member name */
    const char* name_;
    /** Pointer to the member of the class. */
    MemberType Class::*member_;
};

/**
 * @brief  Helper function to create a Property object.
 *
 * @tparam Class The class type that contains the member.
 * @tparam MemberType  The type of the member.
 * @param name The name of the property.
 * @param member Pointer to the member of the class.
 * @return constexpr Property<Class, MemberType> The created Property object.
 */
template <typename Class, typename MemberType>
constexpr inline auto property(const char* name, MemberType Class::*member)
{
    return Property<Class, MemberType> {name, member};
}

}  // namespace game::support

/**
 * @}
 */
#endif  // GAME_SUPPORT_VISIT_PROPERTY_H
