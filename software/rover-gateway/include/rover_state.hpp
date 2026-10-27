#pragma once

#include <atomic>

enum class Command
{
    Stop,
    Forward,
    Backward,
    Left,
    Right
};

struct RoverState
{
    std::atomic<Command> currentCommand{ Command::Stop };
};

inline const char* commandToString(Command command)
{
    switch (command) {
    case Command::Stop:     return "STOP";
    case Command::Forward:  return "FORWARD";
    case Command::Backward: return "BACKWARD";
    case Command::Left:     return "LEFT";
    case Command::Right:    return "RIGHT";
    }

    return "UNKNOWN";
}