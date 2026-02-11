cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(EmptyApp)

add_subdirectory("src" "${CMAKE_CURRENT_BINARY_DIR}/src")
