load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "http_session",
    srcs = [
        "beast_utils.hpp",
        "http_session.cpp",
    ],
    hdrs = ["http_session.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//networking:net_utils",
        "//networking:route_manager_base",
        "//networking/websocket:websocket_session",
        "@boost//:url",
    ],
)

cc_library(
    name = "http_common",
    srcs = [
        "common.hpp",
    ],
    hdrs = ["common.hpp"],
    visibility = ["//visibility:public"],
    deps = [
    ],
)

cc_library(
    name = "beast_utils",
    srcs = [
        "beast_utils.hpp",
    ],
    hdrs = ["beast_utils.hpp"],
    visibility = ["//visibility:public"],
    deps = [
        "@boost//:beast",
    ],
)
