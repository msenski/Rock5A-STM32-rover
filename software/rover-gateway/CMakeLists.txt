cmake_minimum_required(VERSION 3.16)

project(rover-gateway LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(rover-gateway
    src/main.cpp
    src/serial_port.cpp
    src/crow_server.cpp
    src/rover_state.cpp
    src/command.cpp
)

target_compile_options(rover-gateway PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# By PRIVATE we make it only visible for this target's linker
target_include_directories(rover-gateway PRIVATE include)

find_package(Crow CONFIG REQUIRED)

target_link_libraries(rover-gateway PRIVATE Crow::Crow)