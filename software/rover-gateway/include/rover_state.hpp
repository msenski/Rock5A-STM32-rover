#pragma once

#include "command.hpp"

#include <chrono>
#include <mutex>

class RoverState {
  public:
    // Need to add explicitly because we delete the copy functionality below
    RoverState() = default;

    // Disable copying
    RoverState(const RoverState& rhs) = delete;
    RoverState& operator=(const RoverState& rhs) = delete;

    /**
     * @brief Updates the requested movement command.
     *
     * Stores the command and records its arrival time.
     * Thread-safe: protects shared state with a mutex.
     *
     * @param command The new movement command.
     */
    void updateCommand(Command command);

    /**
     * @brief Updates the browser connection state.
     *
     * Resets the movement command to STOP whenever
     * the connection state changes.
     *
     * @param connected True if the controller is connected.
     */
    void setConnected(bool connected);

    /**
     * @brief Returns the current safe movement command.
     *
     * Returns STOP if the browser is disconnected or if
     * the last command is older than COMMAND_TIMEOUT.
     * Otherwise, returns the most recently received command.
     *
     * @return The movement command that is safe to transmit.
     */
    Command getSafeCommand();

  private:
    using Clock = std::chrono::steady_clock;

    static constexpr std::chrono::milliseconds COMMAND_TIMEOUT{300};

    std::mutex mutex_;
    Command currentCommand_{Command::Stop};
    Clock::time_point lastCommandTime_{};
    bool connected_{false};
};
