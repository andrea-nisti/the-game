#include <gtest/gtest.h>

#include <string_view>
#include <type_traits>

#include "support/visit/property.h"
#include "support/visit/visitable.h"

namespace game::test
{

struct SimpleVisitor : public game::support::VisitorBase<SimpleVisitor>
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

struct Mock
{
    int c {9};
    double d {6.5};
    std::string s {"cane"};

    static auto GetVisitable()
    {
        auto v = game::support::visitable<Mock>(
            game::support::property("c", &Mock::c),
            game::support::property("d", &Mock::d),
            game::support::property("s", &Mock::s));

        return v;
    }
};

}  // namespace game::test

TEST(Visitable, GivenMockStructure_MembersAreVisited)
{
    using namespace game::test;
    auto r = game::support::is_i_visitable_v<decltype(Mock::GetVisitable())>;
    ASSERT_TRUE(r);

    Mock m {};
    SimpleVisitor visitor {};

    Mock::GetVisitable().accept(visitor, m);
    EXPECT_TRUE(visitor.c_result.first == "c" and visitor.c_result.second == m.c);
    EXPECT_TRUE(visitor.d_result.first == "d" and visitor.d_result.second == m.d);
    EXPECT_TRUE(visitor.s_result.first == "s" and visitor.s_result.second == m.s);

    m.c = 33;
    m.d = 33.3;
    m.s = "maiale";

    Mock::GetVisitable().accept(visitor, m);
    EXPECT_TRUE(visitor.c_result.first == "c" and visitor.c_result.second == m.c);
    EXPECT_TRUE(visitor.d_result.first == "d" and visitor.d_result.second == m.d);
    EXPECT_TRUE(visitor.s_result.first == "s" and visitor.s_result.second == m.s);
}
