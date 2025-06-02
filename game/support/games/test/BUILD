load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "deck_tests",
    size = "small",
    srcs = ["deck_tests.cpp"],
    args = ["--gtest_color=yes"],
    deps = [
        "//support/games:deck",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
