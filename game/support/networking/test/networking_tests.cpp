#include <iostream>

#include <boost/beast/http.hpp>
#include <gtest/gtest.h>

#include "support/networking/route_manager_base.hpp"
using namespace boost::beast;

namespace game::test
{

template <typename V>
constexpr auto ConvertVerbBeast(const V& value)
{
    static_assert(
        std::is_same_v<V, http::verb> or std::is_same_v<V, support::HttpMethod>,
        "Must be http::verb or support::HttpMethod");

    constexpr auto map = std::array<std::pair<http::verb, support::HttpMethod>, 6> {
        std::make_pair(http::verb::get, support::HttpMethod::GET),
        std::make_pair(http::verb::post, support::HttpMethod::POST),
        std::make_pair(http::verb::put, support::HttpMethod::PUT),
        std::make_pair(http::verb::delete_, support::HttpMethod::DELETE),
        std::make_pair(http::verb::patch, support::HttpMethod::PATCH),
        std::make_pair(http::verb::unknown, support::HttpMethod::UNKNOWN)};

    constexpr auto find = [](const V& value, const auto& map)
    {
        if constexpr (std::is_same_v<V, http::verb>)
        {
            for (const auto& pair : map)
            {
                if (pair.first == value)
                {
                    return pair.second;
                }
            }
        }
        else
        {
            for (const auto& pair : map)
            {
                if (pair.second == value)
                {
                    return pair.first;
                }
            }
        }

        if constexpr (std::is_same_v<V, support::HttpMethod>)
        {
            return http::verb::unknown;
        }
        else
        {
            return support::HttpMethod::UNKNOWN;
        }
    };

    return find(value, map);
}

}  // namespace game::test

using ReqT = http::request<http::string_body>;
using ResT = bool;
TEST(RouteManager, WhenHandleRequest_CheckCallbackIsCalled)
{
    auto route_manager_ptr = game::support::Add(
                                 game::test::ConvertVerbBeast(http::verb::get),
                                 "/test",
                                 [](const ReqT& req, ResT& res) { res = true; })
                                 .Build();
    ReqT req {};
    req.body() = "test";
    ResT res {false};

    route_manager_ptr->HandleRequest(
        game::test::ConvertVerbBeast(http::verb::get), "/test", req, res);
    EXPECT_TRUE(res);

    ResT res2 {false};

    route_manager_ptr->HandleRequest(
        game::test::ConvertVerbBeast(http::verb::trace), "/test", req, res);
    EXPECT_FALSE(res2);

    ResT res3 {false};

    route_manager_ptr->HandleRequest(
        game::test::ConvertVerbBeast(http::verb::get), "/", req, res);
    EXPECT_FALSE(res3);
}