load("//apps/deploy:build_app.bzl", "build_app")

build_app(
    name = "app",
    srcs = ["beast.cpp"],
    deps = [
        "//support/networking:tcp_listener_base",
        "//support/networking/http:http_session",
        "//support/networking:route_manager_base",
        "//support/networking:net_utils",
    ],
)
