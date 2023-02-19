cmake_minimum_required(VERSION 3.16)

project(advent-of-code)

set(CMAKE_UTILS ${CMAKE_SOURCE_DIR}/cmake/utils.cmake)

add_compile_options("/std:c++latest")

add_subdirectory(2022)