#include "serial_port.hpp"
#include "crow_server.hpp"

#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <cstddef>
#include <sys/types.h>
#include <unistd.h>

#include <crow.h>

const auto serial_port = "/dev/serial/by-id/usb-STMicroelectronics_STLINK-V3_0049001C3335510333383531-if02";

enum class Command
{
    Stop,
    Forward,
    Backward,
    Left,
    Right
};

int main()
{
    // Start with a Stop command
    std::atomic<Command> currentCommand{Command::Stop};

    // A flag to indicate whether or not we should be running the UART-worker.
    // It should run by default, until the crow server shuts down.
    std::atomic<bool> uartWorkerRunning{true};

    std::thread uartThread([&]() { // with & we can reference the outer scope
        while (uartWorkerRunning.load() == true)
        {
            Command command = currentCommand.load();

            // TODO send stuff over UART to STM32

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    crow::SimpleApp app;

    configureServer(app);

    app.run();

    // When app shuts down, signal to the UART worker to stop running
    uartWorkerRunning.store(false);
    uartThread.join();

    return 0;
}