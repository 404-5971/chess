@echo off
REM Check if the build directory exists, create if not
if not exist "build" (
    echo Build directory doesn't exist. Creating build directory.
    mkdir build
)

REM Navigate to the build directory and build the project
cd build || exit /b
cmake ..
mingw32-make

