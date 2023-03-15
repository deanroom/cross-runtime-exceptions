cmake_minimum_required(VERSION 3.22)
project(native_exceptions)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory(crashpad)
endif ()

add_executable(native_exceptions main.cpp native-exceptions.cpp native-exceptions.h)
TARGET_LINK_LIBRARIES(native_exceptions PRIVATE CLI11 client)



# add_executable(demo_macos demo.cpp)
# target_compile_features(demo_macos PRIVATE cxx_std_14)
# target_link_libraries(demo_macos PRIVATE client)
