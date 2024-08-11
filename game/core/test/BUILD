load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "deck_tests",
    srcs = ["deck_tests.cpp"],
    deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "//core:deck",
    ],
    args = ["--gtest_color=yes"],
)