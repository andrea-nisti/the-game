load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "networking",
    # size = "small",
    srcs = ["networking_tests.cpp"],
    # args = ["--gtest_color=yes"],
    deps = [
        "//support/networking:tcp_listener_base",
        "//support/networking:http_session",
        # "@googletest//:gtest",
        # "@googletest//:gtest_main",
    ],
)
