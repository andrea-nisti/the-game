load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@rules_pkg//pkg:tar.bzl", "pkg_tar")

cc_binary(
    name = "game",
    srcs = ["main.cpp"],
    # linkstatic = True,
    deps = ["@cpp_httplib"],
)
# Packaging the binary into tar, which is needed by oci_image rule
pkg_tar(
    name = "tar",
    srcs = [
        ":game",
    ],
)

