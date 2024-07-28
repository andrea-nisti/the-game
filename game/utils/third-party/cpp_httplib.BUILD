load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "cpp_httplib",
    srcs = glob([
        "*.cpp",
        "*.h",
    ]),
    hdrs = glob([
        "*.h",
    ]),
    includes = ["."],
    linkopts = ["-lpthread"],
    visibility = ["//visibility:public"],
)
