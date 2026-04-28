cmake_minimum_required(VERSION 3.10)

project(SPD VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(SOURCES
    src/main.cpp
    # Add more source files here
)

# Add executable target placeholder. Replace or extend sources as needed.
add_executable(${PROJECT_NAME} ${SOURCES})

# TODO: add more targets, link libraries, and set include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/inc)

set(TASK_FILE "taski317.txt")

file(COPY "${PROJECT_SOURCE_DIR}/${TASK_FILE}"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/"
)