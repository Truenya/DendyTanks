# Toolchain file for Nix environments
# This file helps CMake find compilers in Nix environments

# Specify the system name
set(CMAKE_SYSTEM_NAME Linux)

# Specify the compilers (use simple names that will be found in PATH)
set(CMAKE_C_COMPILER cc)
set(CMAKE_CXX_COMPILER c++)

# Skip compiler checks
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

# Skip compiler ABI detection
set(CMAKE_C_ABI_COMPILED TRUE)
set(CMAKE_CXX_ABI_COMPILED TRUE)

# Use pkg-config for finding libraries
set(PKG_CONFIG_EXECUTABLE pkg-config)

# Print some information
message(STATUS "Using Nix toolchain file")
message(STATUS "C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")