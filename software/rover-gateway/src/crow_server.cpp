#include "crow_server.hpp"

#include <string>
#include <iostream>

void configureServer(crow::SimpleApp &app)
{
    app.static_file("/", "web/index.html");
    app.static_file("/style.css", "web/style.css");
    app.static_file("/controller.js", "web/controller.js");

    // Add a WebSocket route so we can send stuff from the browser
    // See https://crowcpp.org/master/guides/websockets/#routes
    CROW_WEBSOCKET_ROUTE(app, "/ws")
        .onopen([&](crow::websocket::connection &conn)
                { std::cout << "Connected to Browser!" << std::endl; })
        .onclose([&](crow::websocket::connection &conn, const std::string &reason,
                     uint16_t status_code)
                 { std::cout << "Disconnecting from Browser: " << reason << " (Status code: " << status_code << ") " << std::endl; })
        .onerror([&](crow::websocket::connection &conn, const std::string &error_message)
                 { std::cout << "Encountered error on WebSocket: " << error_message << std::endl; })
        .onmessage([&](crow::websocket::connection &conn, const std::string &message,
                       bool is_binary)
                   { std::cout << "Received command: " << message << std::endl; });

    app.bindaddr("0.0.0.0")
        .port(8080);
}
