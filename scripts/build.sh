#!/bin/bash

# Check if the build directory exists, create if not
if [ ! -d "build" ]; then
    echo "Build directory doesn't exist. Creating build directory."
    mkdir build
fi

# Navigate to the build directory and build the project
cd build || exit
cmake ..
make
