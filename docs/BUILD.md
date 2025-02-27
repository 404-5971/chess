# Building Chess

This document describes how to build the Chess game in both Debug and Release modes.

## Prerequisites

### Linux (Debian/Ubuntu)
```bash
sudo apt-get update
sudo apt-get install build-essential git cmake
```

### Linux (Fedora)
```bash
sudo dnf install gcc-c++ git cmake
```

### Linux (Arch)
```bash
sudo pacman -S base-devel git cmake
```

### macOS
```bash
brew install cmake
```

### Windows
```bash
# Using Chocolatey
choco install mingw cmake git
```

## Installing Raylib

### Automatic Installation
```bash
./scripts/install_raylib.sh  # Unix
# OR
scripts\install_raylib.bat   # Windows
```

### Manual Installation
```bash
git clone https://github.com/raysan5/raylib.git
cd raylib
mkdir build && cd build
cmake -DBUILD_SHARED_LIBS=ON ..
make
sudo make install  # Unix
# OR
make install       # Windows (run as administrator)
```

## Building the Game

### Debug Build
```bash
git clone https://github.com/tsdios/chess.git
cd chess
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

The debug build includes:
- Additional debugging symbols
- No optimization
- Extra runtime checks
- Assertion statements enabled

### Release Build
```bash
git clone https://github.com/tsdios/chess.git
cd chess
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

The release build includes:
- Full optimization (-O3)
- No debugging symbols
- No runtime checks
- Assertions disabled

## Running the Game

### Debug Mode
```bash
# From the build directory
./bin/chess_debug
```

### Release Mode
```bash
# From the build directory
./bin/chess
```

## Common Build Issues

### Raylib Not Found
Ensure Raylib is properly installed and CMake can find it:
```bash
# Unix
export CMAKE_PREFIX_PATH=/usr/local:$CMAKE_PREFIX_PATH

# Windows
set CMAKE_PREFIX_PATH=C:\Program Files\raylib;%CMAKE_PREFIX_PATH%
```

### Build Fails
1. Clean the build directory:
```bash
rm -rf build/*  # Unix
# OR
rd /s /q build  # Windows
```

2. Retry the build process

### Wrong Compiler
Specify the compiler explicitly:
```bash
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
```

## Development Setup

For development, it's recommended to use the debug build with:
- VSCode with C/C++ extension
- CMake Tools extension
- Git integration

## Additional Notes

- Debug builds are larger due to included symbols
- Release builds have better performance
- Use debug builds for development
- Use release builds for distribution
