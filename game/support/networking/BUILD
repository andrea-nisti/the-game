load("@rules_cc//cc:defs.bzl", "cc_library")


_SERVER_VERSION = "game-server-v0"

cc_library(
    name = "net_utils",
    srcs = [
        "net_utils.hpp",
        "beast_utils.hpp",
        "common.hpp"
        
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
    defines = ["SERVER_VERSION={}".format(_SERVER_VERSION)],
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
        ":session_base",
        ":route_manager_base",
    ],
)

cc_library(
    name = "route_manager_base",
    srcs = [
        "route_manager_base.hpp",
        "common.hpp"
    ],
    hdrs = ["route_manager_base.hpp"],
    visibility = ["//visibility:public"],
    deps = [],
)
