#!/usr/bin/env bash

# This script helps build the project with CMake in a Nix environment
# by setting the compiler paths explicitly

# Find compilers in the PATH
CC_PATH=$(which cc 2>/dev/null || which gcc 2>/dev/null)
CXX_PATH=$(which c++ 2>/dev/null || which g++ 2>/dev/null)

if [ -z "$CC_PATH" ] || [ -z "$CXX_PATH" ]; then
    echo "Error: Could not find C/C++ compilers in PATH"
    echo "Please make sure gcc/g++ or cc/c++ are available in your Nix environment"
    exit 1
fi

echo "Using C compiler: $CC_PATH"
echo "Using C++ compiler: $CXX_PATH"

# Clean build directory if it exists
if [ -d "build" ]; then
    echo "Cleaning existing build directory..."
    rm -rf build
fi

# Create build directory
mkdir -p build
cd build

# Configure with explicit compiler paths
echo "Configuring with CMake..."
cmake -DCMAKE_C_COMPILER="$CC_PATH" -DCMAKE_CXX_COMPILER="$CXX_PATH" ..

# Build
echo "Building..."
cmake --build .

# Return to original directory
cd ..

echo "Build complete. Check the 'build' directory for output files."