load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "http_session",
    srcs = [
        "beast_utils.hpp",
        "common.hpp",
        "http_session.cpp",
    ],
    hdrs = ["http_session.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//support/networking:net_utils",
        "//support/networking:route_manager_base",
        "//support/networking:session_base",
    ],
)

cc_library(
    name = "http_common",
    srcs = [
        "common.hpp",
    ],
    hdrs = ["common.hpp"],
    visibility = ["//visibility:public"],
    deps = [
    ],
)
