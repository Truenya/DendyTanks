#!/usr/bin/env bash

# This script sets the correct compiler environment variables for Nix
# and then runs CMake with the provided arguments

# Find the actual compiler paths
CC_PATH=$(which cc)
CXX_PATH=$(which c++)

if [ -z "$CC_PATH" ] || [ -z "$CXX_PATH" ]; then
    echo "Error: Could not find C/C++ compilers in PATH"
    exit 1
fi

echo "Using C compiler: $CC_PATH"
echo "Using C++ compiler: $CXX_PATH"

# Set environment variables
export CC="$CC_PATH"
export CXX="$CXX_PATH"

# Run CMake with all arguments passed to this script
cmake "$@"