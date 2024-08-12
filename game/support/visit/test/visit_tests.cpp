#include <type_traits>

#include <gtest/gtest.h>

#include "support/visit/property.h"
#include "support/visit/visitable.h"

struct Mock
{
    int c{9};
    double d{6.5};
    // std::string s{"cane"};
};

struct SimpleVisitor : public game::support::VisitorBase<SimpleVisitor>
{

    using game::support::VisitorBase<SimpleVisitor>::as;

    template <typename Visitable>
    auto as(Visitable&& visitable)
        -> std::enable_if_t<
            std::is_base_of_v<game::support::VisitableTag, std::decay_t<Visitable>>>
    {
        as_visitable(std::forward<Visitable>(visitable));
    }

    template <typename Visitable>
    auto as(Visitable&& visitable
    ) -> std::enable_if_t<std::is_arithmetic_v<std::decay_t<Visitable>>>
    {
        std::cout << "number" << std::endl;
    }

    auto as(const std::string& visitable)
    {
        std::cout << "string" << std::endl;
    }
};

TEST(Visitable, SimpleTest)
{
    Mock m{};
    auto v = game::support::visitable(
        game::support::property<Mock, int>("c"),
        game::support::property<Mock, double>("d"),
        game::support::property<Mock, std::string>("s")
    );

    auto r = std::is_base_of_v<game::support::VisitableTag, decltype(v)>;
    ASSERT_TRUE(r);

    SimpleVisitor{}.as(v);
}
