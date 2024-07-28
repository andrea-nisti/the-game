load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "hello_test",
    size = "small",
    srcs = ["simple_tests.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
    args = ["--gtest_color=yes"],
)