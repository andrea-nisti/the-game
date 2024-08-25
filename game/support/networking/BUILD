load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "net_utils",
    srcs = [
        "net_utils.hpp",
    ],
    hdrs = ["net_utils.hpp"],
    visibility = ["//visibility:public"],
    deps = [
        "@boost//:asio",
    ],
)

cc_library(
    name = "tcp_listener_base",
    srcs = [
        "interfaces/i_network_server.h",
        "tcp_listener_base.cpp",
    ],
    hdrs = ["tcp_listener_base.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":net_utils",
        "@boost//:asio",
        "@boost//:beast",
    ],
)

cc_library(
    name = "session_base",
    srcs = [
        "session_base.h",
    ],
    hdrs = ["session_base.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":net_utils",
        "@boost//:asio",
        "@boost//:beast",
    ],
)

cc_library(
    name = "http_session",
    srcs = [
        "http_session.cpp",
    ],
    hdrs = ["http_session.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":net_utils",
        ":session_base"
    ],
)
