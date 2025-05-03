cc_library(
    name = "tcp_listener",
    srcs = [
        "tcp_listener.cpp",
    ],
    hdrs = ["tcp_listener.hpp"],
    visibility = ["//visibility:public"],
    deps = [
        "//networking:route_manager_base",
        "//networking:tcp_listener_base",
        "//networking/http:http_common",
        "//networking/http:http_session",
    ],
)

cc_library(
    name = "game_session",
    srcs = [
        # "game_session.cpp",
    ],
    hdrs = ["game_session.hpp"],
    visibility = ["//visibility:public"],
    deps = ["@boost//:asio"],
)
