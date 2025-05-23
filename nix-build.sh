#!/usr/bin/env bash

# Check if nix is installed
if ! command -v nix &> /dev/null; then
    echo "Nix is not installed. Please install Nix first."
    echo "Visit https://nixos.org/download.html for installation instructions."
    exit 1
fi

# Find compilers in the PATH
CC_PATH=$(which cc 2>/dev/null || which gcc 2>/dev/null)
CXX_PATH=$(which c++ 2>/dev/null || which g++ 2>/dev/null)

if [ -z "$CC_PATH" ] || [ -z "$CXX_PATH" ]; then
    echo "Warning: Could not find C/C++ compilers in PATH"
    echo "Will try to continue with Nix build, but CMake might fail to detect compilers"
fi

# Check if flakes are enabled
if nix flake --help &> /dev/null; then
    echo "Building with flakes..."
    nix build
else
    echo "Building with legacy Nix..."
    nix-build
fi

echo "Nix build completed. The result is in ./result/bin/dendy-tanks"

# Offer to build with CMake directly as an alternative
echo ""
echo "Alternatively, you can build with CMake directly:"
echo "To build with CMake, run:"
echo "  mkdir -p build && cd build"
echo "  cmake -DCMAKE_C_COMPILER=\"$CC_PATH\" -DCMAKE_CXX_COMPILER=\"$CXX_PATH\" .."
echo "  cmake --build ."