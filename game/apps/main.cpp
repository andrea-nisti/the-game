#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>

#include "httplib.h"

std::string game_state_json = R"({
    "number_of_players": 2,
    "players": [
        {
            "name": "Alice",
            "hand": [5, 10, 15]
        },
        {
            "name": "Bob",
            "hand": [3, 8, 13]
        }
    ],
    "decks": {
        "ascending_deck": [1, 2, 3, 4, 5],
        "descending_deck": [100, 99, 98, 97, 96]
    },
    "current_turn": "Alice"
})";

int main()
{
    // HTTP
    httplib::Server svr;

    svr.Get(
        "/get(.*)",
        [](const httplib::Request& req, httplib::Response& res)
        {
            std::string_view sw{game_state_json};
            res.set_content(sw.data(), "text/plain");
            std::this_thread::sleep_for(std::chrono::seconds{1});
            std::cout << req.method << req.path << std::endl;
            std::for_each(
                req.params.begin(),
                req.params.end(),
                [](const auto& pair)
                { std::cout << pair.first << " " << pair.second << std::endl; }
            );
        }
    );

    svr.Get(
        "/get_state",
        [](const httplib::Request&, httplib::Response& res)
        {
            std::string_view sw{game_state_json};
            res.set_content(sw.data(), "text/plain");
            std::this_thread::sleep_for(std::chrono::seconds{2});
            std::cout << "The Game 2!! " << std::endl;
        }
    );

    svr.Post(
        "/set_state",
        [](const httplib::Request& req, httplib::Response& res)
        {
            std::string_view sw{req.body};
            game_state_json = sw;
            res.status = httplib::StatusCode::OK_200;
            std::cout << "The Game !! " << std::endl;
        }
    );

    svr.listen("0.0.0.0", 8080);

    std::cout << "The Game !! " << std::endl;
    return 0;
}