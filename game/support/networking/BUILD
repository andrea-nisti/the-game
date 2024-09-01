load("@rules_cc//cc:defs.bzl", "cc_library")

_SERVER_VERSION = "game-server-v0"

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
    defines = ["SERVER_VERSION={}".format(_SERVER_VERSION)],
    visibility = ["//visibility:public"],
    deps = [
        ":net_utils",
        "@boost//:asio",
        "@boost//:beast",
    ],
)

cc_library(
    name = "route_manager_base",
    srcs = [
        "route_manager_base.hpp",
    ],
    hdrs = ["route_manager_base.hpp"],
    visibility = ["//visibility:public"],
    deps = [
        "//support/networking/http:http_common",
        "//support/networking/websocket:ws_handler",
    ],
)
