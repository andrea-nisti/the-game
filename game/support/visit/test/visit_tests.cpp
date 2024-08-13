#include <gtest/gtest.h>

#include <string_view>
#include <type_traits>

#include "support/visit/property.h"
#include "support/visit/visitable.h"

struct Mock
{
    int c {9};
    double d {6.5};
    std::string s {"cane"};
};

namespace game::support
{

struct SimpleVisitor : public VisitorBase<SimpleVisitor>
{
    template <typename T>
    void visit(std::string_view name, T& value)
    {
        std::cout << name << " " << value << std::endl;
    }
};

}  // namespace game::support

TEST(Visitable, SimpleTest)
{
    Mock m {};
    auto v = game::support::visitable<Mock>(
        game::support::property<Mock, int>("c"),
        game::support::property<Mock, double>("d"),
        game::support::property<Mock, std::string>("s"));

    auto r = std::is_base_of_v<game::support::VisitableTag, decltype(v)>;
    ASSERT_TRUE(r);

    v.accept(game::support::SimpleVisitor {}, m);
}
