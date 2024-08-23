load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@rules_pkg//pkg:tar.bzl", "pkg_tar")

cc_binary(
    name = "kawai",
    srcs = ["beast.cpp"],
    deps = [
        "//support/networking:tcp_listener_base",
    ],
)

cc_binary(
    name = "simple",
    srcs = ["main.cpp"],
    deps = [
        "@cpp_httplib",
    ],
)

# Packaging the binary into tar, which is needed by oci_image rule
pkg_tar(
    name = "tar",
    srcs = [
        ":kawai",
    ],
)
