#pragma once

#include <cstddef>
#include <sys/types.h>

class SerialPort {
  public:
    explicit SerialPort(const char* device);
    ~SerialPort();

    // Disable copying so we don't have two objects competing for the same device accidentally
    SerialPort(const SerialPort& rhs) = delete;
    SerialPort& operator=(const SerialPort& rhs) = delete;

    /**
     * @brief Opens a serial port and configures defaults for UART communication.
     *
     *  Settings 8n1, 115200 baud rate.
     * @return true Port was opened successfully.
     * @return false An error occured while opening the port.
     */
    bool openPort();
    /**
     * @brief Reads bytes from the serial port.
     *
     * @param buffer Pointer to the buffer the bytes should be written to.
     * @param length Number of to receive.
     * @return Number of bytes written, or -1 on error.
     */
    ssize_t readBytes(char* buffer, size_t length);
    /**
     * @brief Writes bytes to the serial port.
     *
     * @param buffer Pointer to the data to transmit.
     * @param length Number of bytes to transmit.
     * @return Number of bytes written, or -1 on error.
     */
    ssize_t writeBytes(const char* buffer, size_t length);

  private:
    const char* device_;
    int fd_ = -1; // No file descriptior by default
};