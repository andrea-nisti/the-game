load("@rules_oci//oci:defs.bzl", "oci_image", "oci_load")
load("//apps/deploy:build_app.bzl", "build_app")

build_app(
    name = "app",
    srcs = ["beast.cpp"],
    deps = [
        "//networking:net_utils",
        "//networking:route_manager_base",
        "//networking:tcp_listener_base",
        "//networking/http:http_session",
    ],
)

# Making image
# C++ programs usually need some fundamental libraries such as glibc, libstdc++, etc.
# Correspondigly, use language-specific distroless images.
# Here we use docker.io/library/ubuntu image for this C++ program.
oci_image(
    name = "image",
    base = "@debian_slim",
    entrypoint = ["/app"],
    tars = [":app_tar"],
)

# Use with 'bazel run' to load the oci image into a container runtime.
# The image is designated using `repo_tags` attribute.
oci_load(
    name = "image_load",
    image = ":image",
)
