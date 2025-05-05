cmake_minimum_required(VERSION 3.10)
project(chess VERSION 0.1.0)

# Set the build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build: Debug or Release." FORCE)
endif()

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")

# Set debug/release flags based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Debug mode")
    add_compile_options(-g -O0)  # Debug flags
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in Release mode")
    add_compile_options(-O3)  # Release flags
endif()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Platform-specific configuration
if (WIN32)
    message(STATUS "Configuring for Windows...")

    if (MSVC)  # If we're using MSVC (Microsoft Visual Studio Compiler)
        message(STATUS "Using MSVC compiler")

        # Set platform for MSVC
        set(CMAKE_GENERATOR_PLATFORM x64)  # You can customize this if you want a 32-bit build

        # MSVC specific flags (MSVC doesn't support -g or -O0, so we use /Zi for debugging and /O2 for optimization)
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            add_compile_options(/Zi)  # Enable debugging symbols for MSVC
            set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od")  # Disable optimization for debugging in MSVC
        elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
            add_compile_options(/O2)  # MSVC optimization flag for release
        endif()

    else()  # If we are using a non-MSVC compiler (GCC/Clang)
        find_program(GCC_BIN gcc)
        find_program(CLANG_BIN clang)
        if (GCC_BIN)
            set(CMAKE_C_COMPILER ${GCC_BIN})
        elseif (CLANG_BIN)
            set(CMAKE_C_COMPILER ${CLANG_BIN})
        else()
            message(FATAL_ERROR "Neither MSVC, GCC, nor Clang found! Please install a compiler.")
        endif()

        # For non-MSVC compilers (like GCC or Clang)
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            add_compile_options(-g -O0)  # Debug flags for GCC/Clang
        elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
            add_compile_options(-O3)  # Release flags for GCC/Clang
        endif()

        # Link raylib for non-MSVC (including math library for UNIX systems)
        target_link_libraries(chess PRIVATE raylib m)
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

# Include the source files for the project
file(GLOB SOURCES "src/*.c")

# Create the executable target
add_executable(chess ${SOURCES})

# Add necessary compile definitions and include directories
add_compile_definitions(ASSET_PATH="${CMAKE_SOURCE_DIR}/assets/")
target_include_directories(chess PRIVATE ${CMAKE_SOURCE_DIR}/vendor/raylib/include ${CMAKE_SOURCE_DIR}/header)

set(RAYLIB_LIB ${CMAKE_SOURCE_DIR}/vendor/raylib/lib/libraylib.a)
# Apply compiler-specific options
if (MSVC)
    target_compile_options(chess PRIVATE /W4 /permissive-)
else()
    target_compile_options(chess PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(NOT MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

target_link_libraries(chess
    ${RAYLIB_LIB}
    m
    pthread
    dl
    rt
    X11
)
# Ensure assets are copied to the build directory after build (for all platforms)
add_custom_command(TARGET chess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:chess>/assets
)
