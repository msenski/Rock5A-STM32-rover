#pragma once

#include <cstddef>
#include <sys/types.h>

class SerialPort
{
public:
    explicit SerialPort(const char *device);
    ~SerialPort();

    // Disable copying so we don't have two objects competing for the same device accidentally
    SerialPort(const SerialPort &rhs) = delete;
    SerialPort &operator=(const SerialPort &rhs) = delete;

    /**
     * @brief Opens a serial port and configures defaults for UART communication.
     *
     *  Settings 8n1, 115200 baud rate.
     * @return true Port was opened successfully.
     * @return false An error occured while opening the port.
     */
    bool openPort();
    ssize_t readBytes(char *buffer, size_t length);

private:
    const char *device_;
    int fd_ = -1; // No file descriptior by default
};