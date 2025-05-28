
# DendyTanks

Welcome to DendyTanks! This is a game project built with an innovative new architecture.

## About the Project

DendyTanks is a game that utilizes a modern architectural approach to create an efficient and maintainable codebase. The project showcases several advanced techniques including:

- Entity-Component-System (ECS) pattern
- Modular design with clear separation of concerns
- Flexible configuration system

## Getting Started

### Prerequisites

To build and run DendyTanks, you'll need the following tools installed on your system:

- CMake 3.20 or newer
- A C++ compiler that supports C++23 (such as GCC or Clang)
- Conan package manager (for dependency management)

### Building the Project

To build the project from source, follow these steps:

1. Clone the repository:
   ```bash
   git clone https://github.com/Truenya/DendyTanks.git
   cd DendyTanks
   ```

2. Create a build directory and navigate to it:
   ```bash
   mkdir build
   cd build
   ```

3. Run CMake to configure the project:
   ```bash
   cmake ..
   ```

4. Build the project using your preferred build tool (make, ninja, etc.):
   ```bash
   make -j$(nproc)
   ```

### Running the Game

Once built, you can run the game with:

```bash
./ssdl
```

## Architecture Overview

For more details about the architecture, please see our [Architecture Guide](ARCHITECTURE.md).

## Contributing

Contributions are welcome! Please read our contributing guidelines before submitting a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
