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

### Quick build script

You can also use the provided build script:

```bash
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
# Option 1: Use the helper script (recommended for Nix environments)
./nix-cmake-build.sh

# Option 2: Manual CMake commands
cmake -B build -DCMAKE_C_COMPILER=$(which cc) -DCMAKE_CXX_COMPILER=$(which c++)
cmake --build build
```

If you encounter compiler detection issues in Nix, always use the explicit compiler path approach shown above.

The executable will be available at `build/ssdl`.
