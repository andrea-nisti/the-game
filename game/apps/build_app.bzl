load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@rules_pkg//pkg:tar.bzl", "pkg_tar")

def build_app(name, srcs, deps, copts = [], linkopts = [], runfiles = []):
    cc_binary(
        name = name,
        srcs = srcs,
        deps = deps,
        copts = copts,
        linkopts = linkopts,
    )

    # Packaging the binary into tar, which is needed by oci_image rule
    pkg_tar(
        name = "{}_tar".format(name),
        srcs = [
            ":{}".format(name),
        ] + runfiles,
    )
