#include <iostream>
#include <string>
#include <string_view>

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

  svr.Get("/get_state",
          [](const httplib::Request&, httplib::Response& res)
          {
            std::string_view sw{game_state_json};
            res.set_content(sw.data(), "text/plain");
            std::cout << "The Game !! " << std::endl;
          });

  svr.listen("0.0.0.0", 8080);

  std::cout << "The Game !! " << std::endl;
  return 0;
}