#pragma once

#include "rover_state.hpp"
#include <crow.h>

void configureServer(crow::SimpleApp &app, RoverState &roverState);