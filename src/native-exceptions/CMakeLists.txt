cmake_minimum_required(VERSION 3.22)
project(native_exceptions)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(CLI11)

add_executable(native_exceptions main.cpp native-exceptions.cpp native-exceptions.h)
TARGET_LINK_LIBRARIES(native_exceptions CLI11)