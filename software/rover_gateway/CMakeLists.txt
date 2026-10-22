cmake_minimum_required(VERSION 3.16)

project(rover_gateway LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(rover_gateway
    src/main.cpp
)

target_compile_options(rover_gateway PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)