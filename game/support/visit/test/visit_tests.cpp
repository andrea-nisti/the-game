#include <type_traits>

#include <gtest/gtest.h>

#include "support/visit/property.h"
#include "support/visit/visitable.h"

struct Mock
{
    int c{9};
    double d{6.5};
    std::string s{"cane"};
};

struct SimpleVisitor : game::support::VisitorBase
{
    template <typename Visitor, typename Visitable>
    auto as(Visitable&& visitable) -> std::enable_if_t<std::is_arithmetic_v<Visitable>>
    {
        std::cout << "number" << std::endl;
    }

    auto as(std::string&& visitable)
    {
        std::cout << "string" << std::endl;
    }
};

TEST(Visitable, SimpleTest)
{
    auto v = game::support::visitable(
        game::support::property<Mock, int>("c"),
        game::support::property<Mock, double>("d"),
        game::support::property<Mock, std::string>("s")
    );

    v.visit_impl(std::make_index_sequence<3>{});
}
