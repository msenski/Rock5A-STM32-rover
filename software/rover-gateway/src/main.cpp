#include "crow_server.hpp"
#include "rover_state.hpp"
#include "serial_port.hpp"

#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <cstddef>
#include <sys/types.h>
#include <unistd.h>

#include <crow.h>

const auto serial_port = "/dev/serial/by-id/usb-STMicroelectronics_STLINK-V3_0049001C3335510333383531-if02";

int main()
{
    crow::SimpleApp app;
    RoverState roverState;

    // Configure but don't run yet, it is blocking. First we need to setup our UART loop
    configureServer(app, roverState);

    // A flag to indicate whether or not we should be running the UART-worker.
    // It should run by default, until the crow server shuts down.
    std::atomic<bool> uartWorkerRunning{ true };

    std::thread uartThread([&]()
        {
            Command previousCommand = Command::Stop;

            while (uartWorkerRunning.load() == true)
            {
                Command command = roverState.currentCommand.load();
                if (command != previousCommand) {
                    std::cout << "Received UART: " << commandToString(command) << std::endl;
                    previousCommand = command;
                }


                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            } });

            app.run();

            // When app shuts down, signal to the UART worker to stop running
            uartWorkerRunning.store(false);
            uartThread.join();

            return 0;
}