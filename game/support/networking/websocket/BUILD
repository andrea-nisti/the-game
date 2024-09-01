load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "ws_context",
    srcs = [
        "ws_context.hpp",
    ],
    hdrs = ["ws_context.hpp"],
    visibility = ["//visibility:public"],
    deps = ["@boost//:asio"],
)

cc_library(
    name = "ws_handler",
    srcs = [
        "ws_handler.hpp",
    ],
    hdrs = ["ws_handler.hpp"],
    visibility = ["//visibility:public"],
    deps = [":ws_context"],
)
