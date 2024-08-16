#ifndef GAME_SUPPORT_VISIT_VISITOR_BASE_H
#define GAME_SUPPORT_VISIT_VISITOR_BASE_H

#include <string_view>

namespace game::support
{

/**
 * \addtogroup support Support
 * \brief Supporting classes and functions
 * \ingroup support
 * @{
 */

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
 * @}
 */

}  // namespace game::support
#endif  // GAME_SUPPORT_VISIT_VISITOR_BASE_H
