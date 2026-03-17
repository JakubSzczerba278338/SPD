cmake_minimum_required(VERSION 3.10)

project(my_project VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(SOURCES
    src/tasks.cpp
    # Add more source files here
)

# Add executable target placeholder. Replace or extend sources as needed.
add_executable(${PROJECT_NAME} ${SOURCES})

# TODO: add more targets, link libraries, and set include directories
