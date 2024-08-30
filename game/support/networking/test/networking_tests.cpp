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

    constexpr std::array map {
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
                if (std::get<0>(pair) == value)
                {
                    return std::get<1>(pair);
                }
            }
            return support::HttpMethod::UNKNOWN;
        }
        else
        {
            for (const auto& pair : map)
            {
                if (std::get<1>(pair) == value)
                {
                    return std::get<0>(pair);
                }
            }
            return http::verb::unknown;
        }
    };

    return find(value, map);
}

}  // namespace game::test

using ReqT = http::request<http::string_body>;
using ResT = bool;
TEST(RouteManager, WhenHandleRequest_CheckCallbackIsCalled)
{
    using namespace game::test;
    std::function<void(const ReqT& req, ResT&)> callback =
        [](const ReqT& req, ResT& res) -> void { res = true; };

    auto route_manager_ptr =
        game::support::RouteManagerBuilder<ReqT, ResT> {}
            .Add(
                ConvertVerbBeast(http::verb::get),
                "/test",
                [](const ReqT& req, ResT& res) -> void { res = true; })
            .Add(
                ConvertVerbBeast(http::verb::post),
                "/test_post",
                [](const ReqT& req, ResT& res) -> void { res = true; })
            .Build();

    ReqT req {};
    req.body() = "test";
    ResT res {false};
    route_manager_ptr->HandleRequest(
        ConvertVerbBeast(http::verb::get), "/test", req, res);
    EXPECT_TRUE(res);

    ResT res2 {false};
    route_manager_ptr->HandleRequest(
        ConvertVerbBeast(http::verb::trace), "/test", req, res2);
    EXPECT_FALSE(res2);

    ResT res3 {false};
    route_manager_ptr->HandleRequest(ConvertVerbBeast(http::verb::get), "/", req, res3);
    EXPECT_FALSE(res3);

    ResT res4 {false};
    route_manager_ptr->HandleRequest(
        ConvertVerbBeast(http::verb::post), "/test_post", req, res4);
    EXPECT_TRUE(res4);
}