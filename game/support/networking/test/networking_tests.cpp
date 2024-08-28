#include <iostream>

#include <boost/beast/http.hpp>
#include <gtest/gtest.h>

#include "support/networking/route_manager_base.hpp"
using namespace boost::beast;

namespace game::test
{

template <typename V>
constexpr auto ConvertVerbBeast(const V value)
{
    auto find = [&value](const auto& map)
    {
        return std::find_if(
                   map.begin(),
                   map.end(),
                   [value](const auto& pair) { return pair.first == value; })
            ->second;
    };

    if constexpr (std::is_same_v<V, http::verb>)
    {
        constexpr auto map = std::array<std::pair<http::verb, support::HttpMethod>, 6> {
            std::make_pair(http::verb::get, support::HttpMethod::GET),
            std::make_pair(http::verb::post, support::HttpMethod::POST),
            std::make_pair(http::verb::put, support::HttpMethod::PUT),
            std::make_pair(http::verb::delete_, support::HttpMethod::DELETE),
            std::make_pair(http::verb::patch, support::HttpMethod::PATCH),
            std::make_pair(http::verb::unknown, support::HttpMethod::UNKNOWN)};

        return find(map);
    }
    else if constexpr (std::is_same_v<V, support::HttpMethod>)
    {
        constexpr auto map = std::array<std::pair<support::HttpMethod, http::verb>, 6> {
            std::make_pair(support::HttpMethod::GET, http::verb::get),
            std::make_pair(support::HttpMethod::POST, http::verb::post),
            std::make_pair(support::HttpMethod::PUT, http::verb::put),
            std::make_pair(support::HttpMethod::DELETE, http::verb::delete_),
            std::make_pair(support::HttpMethod::PATCH, http::verb::patch),
            std::make_pair(support::HttpMethod::UNKNOWN, http::verb::unknown)};

        return find(map);
    }
}

}  // namespace game::test

using ReqT = http::request<http::string_body>;
using ResT = bool;
TEST(RouteManager, WhenCallbackIsCalled_ThenCallbackIsCalled)
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
        game::test::ConvertVerbBeast(http::verb::get), "/", req, res);
    EXPECT_FALSE(res2);
}