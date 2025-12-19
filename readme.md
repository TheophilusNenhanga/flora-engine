# Flora Engine

Flora Engine is a 2D UI engine

## Build Instructions

This project uses Cmake and can be used on multiple platforms provided you have the necessary dependencies.

### Dependencies
- SDL3
- MinGW (Windows)

Before building check the TODO in the cmakelists.txt file.

### Building on Windows

```
cmake -B build -G "MinGW Makefiles"
cmake --build build
```

### Building on Linux
```
cmake -B build
cmake --build build
```
