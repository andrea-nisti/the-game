load("//apps/deploy:build_app.bzl", "build_app")

build_app(
    name = "app",
    srcs = ["main.cpp"],
    deps = [
        "//networking:net_utils",
        "//networking:route_manager_base",
        "//networking:tcp_listener_base",
        "//networking/http:http_session",
    ],
)
