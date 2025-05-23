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
  ];
  
  shellHook = ''
    echo "DendyTanks development environment"
    echo "Run 'cmake -B build && cmake --build build' to build the project"
  '';
}