#!/bin/bash
# Check if the build directory exists
if [ ! -d "build" ]; then
    echo "Build directory does not exist. Please build the project first using build.sh."
    exit 1
fi

# Navigate to the build directory and run the executable

cd build
cd bin
./app
