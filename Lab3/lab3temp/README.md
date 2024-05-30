# Lab 3 - Maze Router

## Directory Structure

Lab3

├── main.cpp
├── Grid.cpp
├── Grid.h
├── Router.cpp
├── Router.h
├── Makefile
└── README.md

## Prerequisites

Ensure you have the following installed on your system:

- GCC (GNU Compiler Collection)
- Make

## Compilation

To compile the project, navigate to the project directory and use the `make` command:

```bash
make
```
This command will compile main.cpp, Grid.cpp, and Router.cpp into an executable named lab3.

## Using g++
Alternatively, you can compile the code directly using g++:
```bash
g++ main.cpp Grid.cpp Router.cpp -o lab3
```

## Execution
After compiling the code, you can run the executable with an input file and specify an output file to store the results:
```bash
./lab3 input_file.in output_file.out
```

## Cleaning up
To clean up the generated files, use the `make clean` command:
```bash
make clean
```
This command will remove the `lab3` executable.
