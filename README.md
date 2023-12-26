# emuzeta80

emuzeta80 is a minimalistic Z80 emulator written in C++ for fun and educational purposes. It provides a simple implementation of the Z80 instruction set, making it an good resource for learning about CPU architecture and emulation.


## Getting Started

### Prerequisites

- C++ Compiler: [GCC](https://gcc.gnu.org/) or [Clang](https://clang.llvm.org/).
- [CMake](https://cmake.org/).

### Building

1. Clone the repository:

    ```bash
    git clone https://codeberg.org/iflumpi/emuzeta80.git
    cd emuzeta80
    ```

2. Create a build directory:

    ```bash
    mkdir build
    cd build
    ```

3. Generate build files using CMake:

    ```bash
    cmake ..
    ```

4. Build the project:

    ```bash
    make
    ```

5. After building, you will find the generated dynamic library (`libemuzeta80.so` in the `build` directory.


## Usage

1. Integrate the generated dynamic library into your project.

2. Include the necessary headers in your code.

3. Initialize and use the Z80 emulator functions as needed.


## License

This project is licensed under the GNU General Public License v3.0 - see the LICENSE file for details.
