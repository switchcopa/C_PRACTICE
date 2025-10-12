cmake_minimum_required(VERSION 3.10)
project(mini-shell C)

# Use GCC explicitly
set(CMAKE_C_COMPILER gcc)

# Source directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(MINI_SHELL_DIR ${SRC_DIR}/mini-shell)

# Source files
set(SOURCES
    ${SRC_DIR}/main.c
    ${MINI_SHELL_DIR}/mini-shell.c
)

# Include directory for headers
include_directories(${MINI_SHELL_DIR})

# Output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Define the executable
add_executable(mini-shell ${SOURCES})
