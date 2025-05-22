#!/usr/bin/env bash

# Check if nix is installed
if ! command -v nix &> /dev/null; then
    echo "Nix is not installed. Please install Nix first."
    echo "Visit https://nixos.org/download.html for installation instructions."
    exit 1
fi

# Check if flakes are enabled
if nix flake --help &> /dev/null; then
    echo "Building with flakes..."
    nix build
else
    echo "Building with legacy Nix..."
    nix-build
fi

echo "Build completed. The result is in ./result/bin/dendy-tanks"