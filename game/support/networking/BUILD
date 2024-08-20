load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "tcp_listener_base",
    srcs = [
        "interfaces/i_network_server.h",
        "net_utils.hpp",
        "tcp_listener_base.cpp",
    ],
    hdrs = ["tcp_listener_base.h"],
    visibility = ["//visibility:public"],
    deps = [
        "@boost//:asio",
        "@boost//:beast",
    ],
)
