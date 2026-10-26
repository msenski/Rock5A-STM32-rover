#include <cstddef>
#include <sys/types.h>

#include <cerrno>
// strerror() to print errors nicely
#include <cstring>
#include <iostream>
// File control: How files are opened and configured.
#include <fcntl.h>

// General POSIX os-interface (read(), write(), close(), getpid(), etc.)
#include <unistd.h>

// POSIX terminal control definitions to control asynchronous communications ports.
// We will use it to set the baudrate etc.
#include <termios.h>

#include "serial_port.hpp"

SerialPort::SerialPort(const char *device) : device_(device) {};

SerialPort::~SerialPort()
{
    if (fd_ != -1)
    {
        close(fd_);
    }
};

bool SerialPort::openPort()
{
    // O_NOCTTY flag: if the file points to a terminal device, avoid this terminal
    // becoming the controlling terminal for the process.
    fd_ = open(device_, O_RDWR | O_NOCTTY);

    if (fd_ == -1)
    {
        std::cerr << "Failed to open serial port " << device_ << ": " << std::strerror(errno) << std::endl;
        return false;
    }

    std::cout << "Serial port opened successfully! fd = "
              << fd_ << "\n";

    /*
    We will read in the current configuration of the serial port at fd.

    Definition of termios (from termios-struct.h):

    #define NCCS 32
    struct termios
    {
        tcflag_t c_iflag;		// input mode flags     <-- Should Linux modify incoming bytes?
        tcflag_t c_oflag;		// output mode flags    <-- Should Linux modify outgoing bytes?
        tcflag_t c_cflag;		// control mode flags   <-- Serial comms settings, # data bits, parity, stop bits
        tcflag_t c_lflag;		// local mode flags     <-- Local terminal behaviour, like should it wait for `Enter`?
        cc_t c_line;			// line discipline
        cc_t c_cc[NCCS];		// control characters
        speed_t c_ispeed;		// input speed          <-- Input baud rate
        speed_t c_ospeed;		// output speed         <-- Output baud rate
    }
    */
    struct termios tty;

    if (tcgetattr(fd_, &tty) != 0)
    {
        std::cerr << "Failed to get attributes of serial port: " << std::strerror(errno) << std::endl;
        close(fd_);
        return false;
    }

    std::cout << "Successfully got attributes of serial port." << std::endl;

    // Disable terminal-specific processing first: We don't want linux to interpret any
    // special chars, echo data or change any incoming data.
    // Call it first, because it modifies some bits which we later need to overwrite.
    cfmakeraw(&tty);

    // Baudrate macros are defined in bits/termios-baud.h. We want B115200.
    // In UART both parties need the same config, so set both input and output
    if (cfsetispeed(&tty, B115200) != 0 || cfsetospeed(&tty, B115200) != 0)
    {
        std::cerr << "Failed to set baud rate: " << std::strerror(errno) << std::endl;
        close(fd_);
        return false;
    }

    /*
    We also need to set data bit count, parity and stop bits.

    We will do 8N1, so 8 data bits, no parity and 1 stop bit.

    Dedicated bit masks are defined in bits/termios-c_cflag.h. Note that the
    numbers there are in octal, so one number might represent multiple bits.

    Great reference:
    https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
    */
    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication

    tty.c_cflag &= ~CSIZE; // Clear all the size bits
    tty.c_cflag |= CS8;    // 8 bits per byte

    // CREAD: enables the receiver in the serial driver. Writing is enabled by default.
    //
    // CLOCAL: Ignore modem-control signals (e.g. carrier detection),
    // since we're communicating directly with an STM32, not a modem.
    tty.c_cflag |= CREAD | CLOCAL;

    // Disable hardware flow control (RTS/CTS). CRTSCTS controls hardware flow
    // control, which lets devices signal when they're ready to receive data.
    tty.c_cflag &= ~CRTSCTS;

    // Wait until at least one byte arrives, with no timeout.
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 0;

    // Apply all our settings to the actual device.
    if (tcsetattr(fd_, TCSANOW, &tty) == -1)
    {
        std::cerr << "Failed to configure UART: "
                  << std::strerror(errno) << "\n";
        close(fd_);
        return false;
    }

    return true;
}

ssize_t SerialPort::readBytes(char *buffer, size_t length)
{
    return read(fd_, buffer, length);
}

ssize_t SerialPort::writeBytes(const char *buffer, size_t length)
{
    return write(fd_, buffer, length);
}