load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "networking",
    # size = "small",
    srcs = ["networking_tests.cpp"],
    args = ["--gtest_color=yes"],
    deps = [
        "//networking:net_utils",
        "//networking:route_manager_base",
        "//networking/http:beast_utils",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
