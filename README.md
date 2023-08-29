# chippy
A modern C++ chip8 compiler that tries to minimize OpenGL driver overhead by vertex pulling to draw the framebuffer texture.

## Requirements:
  - GLFW
  - OpenGL 4.6 capable device
  - CMake
  - Ninja/Make
  - A C++23 compiler

## Building(Ninja):
1. `cd build && cmake -DCMAKE_BUILD_TYPE=release -G Ninja .`
2. `ninja`

## Building(Make):
1. `cd build && cmake -DCMAKE_BUILD_TYPE=release .`
2. `make`

## Usage:
`./output/chippy -scale 16 -program assets/programs/program_name.ch8 -tickrate 700`
