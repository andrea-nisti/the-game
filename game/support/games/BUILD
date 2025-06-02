load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "deck",
    srcs = [
        "Card.h",
        "Deck.cpp",
    ],
    hdrs = ["Deck.h"],
    visibility = ["//visibility:public"],
    deps = [],
)

# The Game Session CC Library
#
# This library is responsible for managing player controller connections and disconnections
# within a game session. It ensures proper handling of player controllers as they join or
# leave the session. Additionally, it routes commands received from the connected player
# controllers to the appropriate game logic, facilitating seamless interaction between
# players and the game.
#
cc_library(
    name = "game_session",
    srcs = [
    ],
    hdrs = [],
    visibility = ["//visibility:public"],
    deps = [],
)
