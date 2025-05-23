{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    # Build tools
    cmake
    pkg-config
    gnumake
    ninja
    
    # Development tools
    gdb
    valgrind
    clang-tools # For clang-format, clang-tidy
    
    # Libraries
    SDL2
    SDL2_image
    libuuid
    gtest
    
    # Compilers
    gcc
  ];
  
  # Set environment variables to fix compiler detection
  shellHook = ''
    export CMAKE_C_COMPILER=cc
    export CMAKE_CXX_COMPILER=c++
    export CC=cc
    export CXX=c++
    
    echo "DendyTanks development environment"
    echo "Run 'cmake -B build && cmake --build build' to build the project"
    echo "Compiler environment variables have been set automatically"
  '';
}