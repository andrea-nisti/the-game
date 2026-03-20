load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "game_session",
    hdrs = ["game_session.hpp"],
    textual_hdrs = ["game_session.cpp"],
    visibility = ["//visibility:public"],
    deps = ["@boost.asio"],
)

cc_library(
    name = "player_controller_base",
    hdrs = ["player_controller_base.hpp"],
    textual_hdrs = ["player_controller_base.cpp"],
    visibility = ["//visibility:public"],
    deps = ["@boost.asio"],
)

cc_library(
    name = "players_manager",
    hdrs = ["players_manager.hpp"],
    textual_hdrs = ["players_manager.cpp"],
    visibility = ["//visibility:public"],
    deps = ["@boost.asio"],
)

cc_library(
    name = "websocket_player_controller",
    hdrs = ["websocket_player_controller.hpp"],
    textual_hdrs = ["websocket_player_controller.cpp"],
    visibility = ["//visibility:public"],
    deps = [
        ":player_controller_base",
        "//networking/websocket:websocket_session",
    ],
)
