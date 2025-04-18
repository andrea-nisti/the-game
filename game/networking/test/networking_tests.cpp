#include <boost/beast/http.hpp>
#include <gtest/gtest.h>

#include "networking/http/beast_utils.hpp"
#include "networking/route_manager_base.hpp"

using namespace boost::beast;

using ReqT = http::request<http::string_body>;
using ResT = bool;

struct RouteManagerWithHandleRequest : public game::support::RouteManagerBase<ReqT, ResT>
{
    void HandleRequest(
        const game::support::HttpMethod method,
        const std::string& path,
        const ReqT& req,
        ResT& res)
    {
        auto cb = RouteManagerBase<ReqT, ResT>::GetCallback(method, path);
        if (cb.has_value())
        {
            std::invoke(cb.value(), req, std::nullopt, res);
        }
    }
};

TEST(RouteManager, WhenHandleRequest_CheckCallbackIsCalled)
{
    using namespace game::support;
    std::function<void(const ReqT& req, ResT&)> callback =
        [](const ReqT& req, ResT& res) -> void { res = true; };

    auto route_manager_ptr =
        game::support::RouteManagerBuilder<ReqT, ResT> {}
            .WithRouteManagerType<RouteManagerWithHandleRequest>()
            .Add(
                ConvertVerbBeast(http::verb::get),
                "/test",
                [](const ReqT& req, std::optional<Params> params, ResT& res) -> void
                { res = true; })
            .Add(
                ConvertVerbBeast(http::verb::post),
                "/test_post",
                [](const ReqT& req, std::optional<Params> params, ResT& res) -> void
                { res = true; })
            .Build();

    auto rm = dynamic_cast<RouteManagerWithHandleRequest*>(route_manager_ptr.get());
    ReqT req {};
    req.body() = "test";
    ResT res {false};
    rm->HandleRequest(game::support::HttpMethod::GET, "/test", req, res);
    EXPECT_TRUE(res);

    ResT res2 {false};
    rm->HandleRequest(game::support::HttpMethod::UNKNOWN, "/test", req, res2);
    EXPECT_FALSE(res2);

    ResT res3 {false};
    rm->HandleRequest(game::support::HttpMethod::GET, "/", req, res3);
    EXPECT_FALSE(res3);

    ResT res4 {false};
    rm->HandleRequest(game::support::HttpMethod::POST, "/test_post", req, res4);
    EXPECT_TRUE(res4);
}