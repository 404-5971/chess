cmake_minimum_required(VERSION 3.10)
project(chess VERSION 0.0.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Ensure the compile_commands.json file is generated
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set output directory within the build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (WIN32)
    message(STATUS "Configuring for Windows...")
    if (MSVC)
        set(CMAKE_GENERATOR_PLATFORM x64)
    else()
        find_program(GCC_BIN gcc)
        find_program(CLANG_BIN clang)
        if (GCC_BIN)
            set(CMAKE_C_COMPILER ${GCC_BIN})
        elseif (CLANG_BIN)
            set(CMAKE_C_COMPILER ${CLANG_BIN})
        else()
            message(FATAL_ERROR "Neither MSVC, GCC, nor Clang found! Please install a compiler.")
        endif()
    endif()
elseif (APPLE)
    set(CMAKE_C_COMPILER clang)
elseif (UNIX)
    find_program(GCC_BIN gcc)
    find_program(CLANG_BIN clang)
    if (GCC_BIN)
        set(CMAKE_C_COMPILER ${GCC_BIN})
    elseif (CLANG_BIN)
        set(CMAKE_C_COMPILER ${CLANG_BIN})
    else()
        message(FATAL_ERROR "Neither GCC nor Clang found! Please install a compiler.")
    endif()
endif()

# Locate source files
file(GLOB SOURCES "src/*.c")
find_package(raylib)

# Add executable target with source files
add_executable(chess ${SOURCES})

# Link header file directory
target_include_directories(chess PRIVATE header raylib)
target_link_libraries(raylib)

if (MSVC)
    target_compile_options(chess PRIVATE /W4 /permissive-)
else()
    target_compile_options(chess PRIVATE -Wall -Wextra -Wpedantic)
endif()
