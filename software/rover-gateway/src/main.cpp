#include "command.hpp"
#include "crow_server.hpp"
#include "rover_state.hpp"
#include "serial_port.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <sys/types.h>
#include <thread>

#include <crow.h>

const auto serial_port =
    "/dev/serial/by-id/usb-STMicroelectronics_STLINK-V3_0049001C3335510333383531-if02";

int main() {
    crow::SimpleApp app;
    RoverState roverState;

    // Configure but don't run yet, it is blocking. First we need to setup our UART loop
    configureServer(app, roverState);

    SerialPort port(serial_port);

    if (!port.openPort()) {
        std::cerr << "Failed to open serial port. Terminating..." << std::endl;
        return 1;
    }

    // A flag to indicate whether or not we should be running the UART-worker.
    // It should run by default, until the crow server shuts down.
    std::atomic<bool> uartWorkerRunning{true};

    std::thread uartThread([&]() {
        Command previousCommand = Command::Stop;

        while (uartWorkerRunning.load()) {
            Command command = roverState.getSafeCommand();

            // Log to terminal only when command changes to avoid spamming
            if (command != previousCommand) {
                std::cout << "UART TX: " << commandToString(command) << std::endl;

                previousCommand = command;
            }

            char character = commandToChar(command);

            ssize_t bytesWritten = port.writeBytes(&character, 1);

            if (bytesWritten != 1) {
                // Something fishy is going on
                std::cerr << "UART transmission failed! " << std::endl;
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    app.run();

    // When app shuts down, signal to the UART worker to stop running
    uartWorkerRunning.store(false);
    uartThread.join();

    return 0;
}