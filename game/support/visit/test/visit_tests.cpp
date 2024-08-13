#include <gtest/gtest.h>

#include <string_view>
#include <tuple>
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
    void visit(std::string_view name, const T& value)
    {
        std::cout << name << " " << value << std::endl;
    }

    template <>
    void visit(std::string_view name, const int& value)
    {
        c_result.first = name;
        c_result.second = value;
    }

    template <>
    void visit(std::string_view name, const double& value)
    {
        d_result.first = name;
        d_result.second = value;
    }

    template <>
    void visit(std::string_view name, const std::string& value)
    {
        s_result.first = name;
        s_result.second = value;
    }

    std::pair<std::string_view, int> c_result;
    std::pair<std::string_view, double> d_result;
    std::pair<std::string_view, std::string> s_result;
};

}  // namespace game::support

TEST(Visitable, GivenMockStructure_MembersAreVisited)
{
    Mock m {};
    auto v = game::support::visitable<Mock>(
        game::support::property("c", &Mock::c),
        game::support::property("d", &Mock::d),
        game::support::property("s", &Mock::s));

    auto r = std::is_base_of_v<game::support::VisitableTag, decltype(v)>;
    ASSERT_TRUE(r);
    game::support::SimpleVisitor visitor {};
    v.accept(visitor, m);

    EXPECT_TRUE(visitor.c_result.first == "c" and visitor.c_result.second == m.c);
    EXPECT_TRUE(visitor.d_result.first == "d" and visitor.d_result.second == m.d);
    EXPECT_TRUE(visitor.s_result.first == "s" and visitor.s_result.second == m.s);
}
