# cmake_minimum_required(VERSION <specify CMake version here>)
project(market C)

set(CMAKE_C_STANDARD 99)

add_executable(market main.c gps.h gps.c)