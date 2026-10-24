#include <cerrno>
// strerror() to print errors nicely
#include <cstring>
#include <iostream>

#include <cstddef>
#include <sys/types.h>

#include "serial_port.hpp"

const auto serial_port = "/dev/ttyACM0";

int main()
{
    std::cout << "Rock5A rover gateway started." << std::endl;

    // Open our serial port to the STM
    SerialPort serial = SerialPort(serial_port);

    if (!serial.openPort())
    {
        return 1;
    }

    // Fiiinaaaallyyyyy
    char buffer[100];
    while (1)
    {
        ssize_t bytes_read = serial.readBytes(buffer, sizeof(buffer));

        if (bytes_read < 0)
        {
            std::cerr << "Reading from serial port " << serial_port << " failed with: " << std::strerror(errno) << std::endl;
        }

        if (bytes_read == 0)
        {
            std::cout << "No bytes received.\n";
            break;
        }

        std::cout.write(buffer, bytes_read);
        std::cout.flush();
    }

    return 0;
}