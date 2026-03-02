load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "app",
    srcs = ["beast.cpp"],
    visibility = ["//visibility:public"],
    deps = [
        "//networking:net_utils",
        "//networking:route_manager_base",
        "//networking:tcp_listener_base",
        "//networking/http:http_session",
    ],
)
