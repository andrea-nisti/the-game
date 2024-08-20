load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "tcp_listener_base",
    srcs = [
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
