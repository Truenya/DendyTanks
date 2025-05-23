#!/usr/bin/env bash

# This script builds the project in a Nix environment using a toolchain file

# Clean build directory if it exists
if [ -d "build" ]; then
    echo "Cleaning existing build directory..."
    rm -rf build
fi

# Create build directory
mkdir -p build
cd build

# Configure with toolchain file
echo "Configuring with CMake using toolchain file..."
cmake -DCMAKE_TOOLCHAIN_FILE=../nix-toolchain.cmake ..

# Build
echo "Building..."
cmake --build .

# Return to original directory
cd ..

echo "Build complete. Check the 'build' directory for output files."