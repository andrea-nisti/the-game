#include <boost/beast/http.hpp>
#include <gtest/gtest.h>

#include "support/networking/beast_utils.hpp"
#include "support/networking/route_manager_base.hpp"

using namespace boost::beast;

using ReqT = http::request<http::string_body>;
using ResT = bool;
TEST(RouteManager, WhenHandleRequest_CheckCallbackIsCalled)
{
    using namespace game::support;
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