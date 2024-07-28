#include <iostream>

#include "httplib.h"

int main()
{
  // HTTP
  httplib::Server svr;

  svr.Get("/hi",
          [](const httplib::Request&, httplib::Response& res)
          {
            res.set_content("Hello World!", "text/plain");
            std::cout << "The Game !! " << std::endl;
          });

  svr.listen("0.0.0.0", 8080);

  std::cout << "The Game !! " << std::endl;
  return 0;
}