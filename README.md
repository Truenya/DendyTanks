# DendyTanks - Game Programming Patterns Training

This project is focused on learning and practicing game programming patterns through the development of a simple tank game. The goal is to implement various design patterns commonly used in game development while creating an engaging gameplay experience.

## Table of Contents
- [Building on Ubuntu](#building-on-ubuntu)
- [Building with Nix/NixOS](#building-with-nixnixos)
  - [Using flakes (modern Nix)](#using-flakes-modern-nix)
  - [Using legacy Nix](#using-legacy-nix)
  - [Quick Build Script](#quick-build-script)
- [Development](#development)

## Building on Ubuntu

```bash
apt update && apt install cmake clang pkg-config libsdl2-dev google-perftools libgtest-dev git 

git clone https://github.com/libsdl-org/SDL_image

cd SDL_image && mkdir build && cd build && cmake .. && make -j8 && make install
```

## Building with Nix/NixOS

This project can be built using the Nix package manager, which provides a reproducible build environment.

### Using flakes (modern Nix)

```bash
# Build the project
nix build

# Enter development shell
nix develop
```

### Using legacy Nix

```bash
# Build the project
nix-build

# Enter development shell
nix-shell
```

### Quick Build Script

For convenience, you can use the provided build script:

```bash
# Standard Nix build
./nix-build.sh
```

## Development

For development, enter the Nix shell which provides all necessary dependencies:

```bash
nix-shell
# or with flakes
nix develop
```

Then build the project with:

```bash
cmake -B build && cmake --build build
```

The executable will be available at `build/ssdl` after a successful build.
