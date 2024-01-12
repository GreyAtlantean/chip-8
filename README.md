# chip-8
## A basic chip 8 emulator

### Written in C++ and using SDL2

### Running the program

To run the emulator and a ROM simply run:

./chip-8 location/of/rom/rom.ch8

### Controls

The chip 8 keyboard is 

1  2  3  C

4  5  6  D

7  8  9  E

A  0  B  F

These keys have been mapped to

1  2  3  4

Q  W  E  R

A  S  D  F

Z  X  C  V

To exit the emulator press ESC

### Compilation

This project is built using CMake.

Ensure SDL2 is installed on your system.

Run the following from the build folder:

cmake ..

make

./chip-8

### Screenshots

IBM Logo 
![ScreenShot One](/screenshots/IBM.png?raw=true "Screenshot One IBM")
Corax instructions test
![ScreenShot Two](/screenshots/CoraxInstructions.png?raw=true "Screenshot Two CoraxInstructions")
VF Flags test
![ScreenShot Three](/screenshots/Flags.png?raw=true "Screenshot Three Flags")
Sierpinski triangle program
![ScreenShot Four](/screenshots/Sierpinski.png?raw=true "Screenshot One Sierpinski")
Particles
![ScreenShot Five](/screenshots/Particles.png?raw=true "Screenshot One Particles")
Tron 
![ScreenShot Six](/screenshots/Tron.png?raw=true "Screenshot One Tron")

### Missing Features
    - Sound
    - Debugging information displayed on screen

### Known Issues
    - Test 6 - keyboard not passing block on release
    - Test 5 - quirks - fails with segfault!
