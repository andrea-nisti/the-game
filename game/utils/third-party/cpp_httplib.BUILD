load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "cpp_httplib",
    srcs = glob([
        "cpp_httplib/*.cpp",
        "cpp_httplib/*.h",
    ]),
    hdrs = glob([
        "cpp_httplib/*.h",
    ]),
    includes = ["."],
    visibility = ["//visibility:public"],
)
