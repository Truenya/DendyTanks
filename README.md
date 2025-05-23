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

You can use one of the provided build scripts:

```bash
# Option 1: Standard Nix build
./nix-build.sh

# Option 2: Build with CMake using toolchain file (recommended for Nix environments)
./nix-build-with-toolchain.sh
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
# Option 1: Use the toolchain file (recommended for Nix environments)
cmake -B build -DCMAKE_TOOLCHAIN_FILE=./nix-toolchain.cmake
cmake --build build

# Option 2: Use the helper script
./nix-build-with-toolchain.sh

# Option 3: Manual CMake commands with explicit compiler paths
cmake -B build -DCMAKE_C_COMPILER=$(which cc) -DCMAKE_CXX_COMPILER=$(which c++)
cmake --build build
```

If you encounter compiler detection issues in Nix, always use the toolchain file approach shown above.

The executable will be available at `build/ssdl`.
