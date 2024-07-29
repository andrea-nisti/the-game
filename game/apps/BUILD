load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@rules_pkg//pkg:tar.bzl", "pkg_tar")

cc_binary(
    name = "game",
    srcs = ["main.cpp"],
    data = ["ciao.md"],
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

# oci_image(
#     name = "image",
#     base = "@debian_base",
#     tars = [":tar"],
#     entrypoint = ["/game"],
# )

# oci_load(
#     name = "image_load",
#     image = ":image",
#     repo_tags = ["example:latest"],
# )
