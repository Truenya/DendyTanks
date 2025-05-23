# DendyTanks
Currently, training of this project in game programming patterns.

## Building on Ubuntu

```bash
apt update && apt install cmake clang pkg-config libsdl2-dev google-perftools libgtest-dev git 

git clone https://github.com/libsdl-org/SDL_image

cd SDL_image && mkdir build && cd build && cmake .. && make -j8 && make install
```

## Building with Nix/NixOS

This project can be built using Nix package manager, which provides a reproducible build environment.

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

### Quick build scripts

You can use the provided build script:

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
# Use the nix-cmake.sh wrapper script (recommended for Nix environments)
./nix-cmake.sh -B build && cmake --build build
```

If you encounter compiler detection issues in Nix, you can also try:

```bash
# Alternative: Set environment variables manually
export CC=$(which cc)
export CXX=$(which c++)
cmake -B build && cmake --build build
```

The executable will be available at `build/ssdl`.
