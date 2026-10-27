#include "command.hpp"

const char *commandToString(Command command)
{
    switch (command)
    {
    case Command::Stop:
        return "STOP";
    case Command::Forward:
        return "FORWARD";
    case Command::Backward:
        return "BACKWARD";
    case Command::Left:
        return "LEFT";
    case Command::Right:
        return "RIGHT";
    }

    return "UNKNOWN";
}

char commandToChar(Command command)
{
    switch (command)
    {
    case Command::Forward:
        return 'w';
    case Command::Backward:
        return 's';
    case Command::Left:
        return 'a';
    case Command::Right:
        return 'd';
    case Command::Stop:
        return ' ';
    }

    return ' ';
}