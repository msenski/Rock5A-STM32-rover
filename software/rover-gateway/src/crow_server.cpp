#include "crow_server.hpp"

#include <string>
#include <iostream>

void configureServer(crow::SimpleApp &app, RoverState &roverState)
{
    app.static_file("/", "web/index.html");
    app.static_file("/style.css", "web/style.css");
    app.static_file("/controller.js", "web/controller.js");

    // Add a WebSocket route so we can send stuff from the browser
    // See https://crowcpp.org/master/guides/websockets/#routes
    CROW_WEBSOCKET_ROUTE(app, "/ws")
        .onopen([&](crow::websocket::connection &conn)
                { std::cout << "Connected to Browser!" << std::endl; })
        .onmessage([&roverState](crow::websocket::connection &conn, const std::string &message,
                                 bool is_binary)
                   {
                       std::cout << "Received command: " << message << std::endl;
                       if (is_binary)
                           return;

                       Command command;
                       if (message == "FORWARD")
                       {
                           command = Command::Forward;
                       }
                       else if (message == "BACKWARD")
                       {
                           command = Command::Backward;
                       }
                       else if (message == "LEFT")
                       {
                           command = Command::Left;
                       }
                       else if (message == "RIGHT")
                       {
                           command = Command::Right;
                       }
                       else if (message == "STOP")
                       {
                           command = Command::Stop;
                       }
                       else
                       {
                           std::cout << "Unknown command: " << message << std::endl;
                           return;
                       }

                       roverState.currentCommand.store(command); })
        .onclose([&roverState](crow::websocket::connection &conn, const std::string &reason,
                               uint16_t status_code)
                 { std::cout << "Disconnecting from Browser: " << reason << " (Status code: " << status_code << ") " << std::endl;
                    roverState.currentCommand.store(Command::Stop); })
        .onerror([&roverState](crow::websocket::connection &conn, const std::string &error_message)
                 { std::cout << "Encountered error on WebSocket: " << error_message << std::endl;
                roverState.currentCommand.store(Command::Stop); });

    app.bindaddr("0.0.0.0")
        .port(8080);
}
