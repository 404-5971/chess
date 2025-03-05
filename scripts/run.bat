@echo off
REM Check if the build directory exists
if not exist "build" (
    echo Build directory does not exist. Please build the project first using build.bat.
    exit /b 1
)

REM Navigate to the build/bin directory and run the executable
cd build
cd bin

REM Run the executable
chess.exe

