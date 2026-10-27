#pragma once

enum class Command { Stop, Forward, Backward, Left, Right };

const char* commandToString(Command command);
char commandToChar(Command command);