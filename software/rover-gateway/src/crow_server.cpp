#include "crow_server.hpp"

void configureServer(crow::SimpleApp &app)
{
    app.static_file("/", "web/index.html");
    app.static_file("/style.css", "web/style.css");
    app.static_file("/controller.js", "web/controller.js");

    std::cout << "Starting Rock5A rover server..." << std::endl;

    app.bindaddr("0.0.0.0").port(8080);
}
