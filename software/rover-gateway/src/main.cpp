#include <cerrno>
#include <cstring>
#include <string>
#include <iostream>

#include <cstddef>
#include <sys/types.h>

#include "serial_port.hpp"
#include <unistd.h>

#include <crow.h>

const auto serial_port = "/dev/serial/by-id/usb-STMicroelectronics_STLINK-V3_0049001C3335510333383531-if02";

int main()
{
    crow::SimpleApp app;

    app.static_file("/", "web/index.html");
    app.static_file("/style.css", "web/style.css");
    app.static_file("/controller.js", "web/controller.js");

    std::cout << "Starting Rock5A rover server..." << std::endl;

    app.bindaddr("0.0.0.0").port(8080).run();

    // // Open our serial port to the STM
    // SerialPort serial = SerialPort(serial_port);

    // if (!serial.openPort())
    // {
    //     return 1;
    // }

    // while (1)
    // {
    //     std::string input;
    //     if (!std::getline(std::cin, input))
    //     {
    //         break;
    //     }

    //     if (input.empty())
    //     {
    //         continue;
    //     }

    //     char command = input[0];

    //     if (command == 'w' || command == 'a' || command == 's' || command == 'd' || command == ' ')
    //     {
    //         serial.writeBytes(&command, 1);
    //     }
    //     else if (command == 'q')
    //     {
    //         char stop = ' ';
    //         serial.writeBytes(&stop, 1);
    //         return 0;
    //     }
    //     else
    //     {
    //         std::cout << "Unknown command. Can be one of 'w', 'a','s','d', ' ' or 'q'" << std::endl;
    //     }
    // }

    return 0;
}