#include "rover_state.hpp"

#include <mutex>

void RoverState::updateCommand(Command command)
{
    std::lock_guard<std::mutex>
        lockGuard(mutex_);

    currentCommand_ = command;

    // Remember when we last received a command
    lastCommandTime_ = Clock::now();
};

void RoverState::setConnected(bool connected)
{
    std::lock_guard<std::mutex> lockGuard(mutex_);

    connected_ = connected;

    // Set the currentCommand to Stop whenever the connection status changes just in case
    currentCommand_ = Command::Stop;

    // Reset the clock on browser connection
    lastCommandTime_ = Clock::now();
};

Command RoverState::getSafeCommand()
{
    std::lock_guard<std::mutex> lockGuard(mutex_);

    if (!connected_)
    {
        // If the browser disconnects, set command to STOP just in case
        return Command::Stop;
    }

    auto elapsed = Clock::now() - lastCommandTime_;

    if (elapsed > COMMAND_TIMEOUT)
    {
        // If we didn't receive a command in a while, set to STOP, too
        return Command::Stop;
    }

    return currentCommand_;
};