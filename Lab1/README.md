# BDD Min Node Count

## Overview
This program calculates the minimum number of nodes in a Binary Decision Diagram (BDD) for a given Boolean expression and a set of variable orderings. It utilizes the CUDD package to manage the decision diagrams efficiently.

## Prerequisites
- C++ compiler (e.g., g++, clang++)
- CUDD library installed on your system
- Basic knowledge of running C++ programs from the command line

## Compilation
Ensure that the CUDD library is correctly installed and accessible in your environment. You may need to specify the include path to the CUDD headers and the library path for linking.

Here is an example script of how to compile the program:

```bash
#!/bin/bash

# Paths
CUDDDIR="path/to/your/cudd-3.0.0"
CUDD_INCLUDE="$CUDDDIR/cudd"
UTIL_INCLUDE="$CUDDDIR/util"
CUDD_LIB="$CUDDDIR/cudd/.libs/libcudd.a"

# Compile options
OPTIONS="-I $CUDD_INCLUDE -I $UTIL_INCLUDE -lm -O3"

if [ $# -lt 2 ]; then
    echo "Usage: $0 <input_file> <output_file>"
    exit 1
fi

INPUT_FILE="$1"
OUTPUT_FILE="$2"

g++ $OPTIONS lab1.cpp $CUDD_LIB -o Lab1 && ./Lab1 $INPUT_FILE $OUTPUT_FILE
```
To run this script:
```bash
chmod +x Lab1.sh
./Lab1.sh [inputFile] [outputFile]
```


Or, for example if CUDD is installed in `/home/ivesliu/EDA_2024_Spring/cudd-3.0.0`, directly using g++:
```bash
g++ -I /uhome/chome/2024EDA/2024EDA017/cudd-3.0.0/cudd -I /uhome/chome/2024EDA/2024EDA017/cudd-3.0.0/util -I /uhome/chome/2024EDA/2024EDA017/cudd-3.0.0 Lab1.cpp /uhome/chome/2024EDA/2024EDA017/cudd-3.0.0/cudd/.libs/libcudd.a -lm -O3 -o Lab1
```

## Makefile Usage

To simplify the compilation process, a Makefile is provided. Here's how to use it along with a description of its contents:

### Paths and Directories
- `CUDDDIR`: Path to the CUDD library directory.
- `CUDD_INCLUDE`: Path to the CUDD library header files.
- `UTIL_INCLUDE`: Path to CUDD's utility header files.
- `CUDD_LIB`: Path to the CUDD static library file.

### Compiler Options
- `CXX`: Specifies the C++ compiler (g++ in this case).
- `CXXFLAGS`: Compiler flags for including CUDD header files, linking with math library (`-lm`), and optimization level (`-O3`).

### Executable
- `EXEC`: Name of the generated executable file (Lab1 in this case).

### Source and Object Files
- `SRCS`: Lists the source file(s) for the project.
- `OBJS`: Lists the object file(s) generated from the source file(s).

### Targets
- `all`: The default target, which compiles the source files and links them to create the executable.
- `$(EXEC)`: A target to link the object files and CUDD library to create the executable.
- `%.o`: A pattern rule to compile source files into object files.
- `clean`: A target to clean up by removing object files and the executable.

### Makefile Commands
To compile the project, simply run the `make` command in the directory containing the Makefile and source files:

```bash
make
```
### Execution
```bash
./Lab1 [inputFile] [outputFile]
```
## Source Code Review
The program is structured as follow:
- ``buildBDD``: Constructs the BDD for the given Boolean expression and variable ordering.
- ``main``: Reads the Boolean expression and variable orderings from input.txt, processes them to construct the BDD using the CUDD library, and writes the minimum node count to output.txt.

## CUDD Package Usage

The program leverages the CUDD (Colorado University Decision Diagram) package for efficient manipulation and analysis of Binary Decision Diagrams (BDDs). Below are the key functions from the CUDD library used in this program:

- `Cudd_ReadLogicZero(manager)`: Returns the logical zero node of the BDD.
- `Cudd_ReadOne(manager)`: Returns the logical one node of the BDD.
- `Cudd_bddIthVar(manager, index)`: Retrieves the BDD variable corresponding to the given index.
- `Cudd_Not(node)`: Computes the logical NOT of a node.
- `Cudd_bddAnd(manager, node1, node2)`: Computes the logical AND of two nodes.
- `Cudd_bddOr(manager, node1, node2)`: Computes the logical OR of two nodes.
- `Cudd_BddToAdd(manager, bdd)`: Converts a BDD to an ADD (Algebraic Decision Diagram), which is useful for certain types of analysis and optimization.
- `Cudd_DagSize(node)`: Calculates the size of the decision diagram, represented as the number of nodes in the DAG (Directed Acyclic Graph).

### Installation and Configuration

Ensure that the CUDD library is properly installed on your system. The `Makefile` in this project relies on the following variables to locate the CUDD library components:

- `CUDDDIR`: The root directory of the CUDD library installation.
- `CUDD_INCLUDE`: The directory containing the header files of the CUDD library, typically `$(CUDDDIR)/cudd`.
- `UTIL_INCLUDE`: The directory containing utility header files of CUDD, often `$(CUDDDIR)/util`.
- `CUDD_LIB`: The full path to the CUDD static library file, usually found at `$(CUDDDIR)/cudd/.libs/libcudd.a`.

You must update these paths in the `Makefile` to match the actual locations of the CUDD library on your system. Incorrect paths will lead to compilation errors.

For example, if CUDD is installed in `/uhome/chome/2024EDA/2024EDA017/cudd-3.0.0`, you should set:

```makefile
CUDDDIR = /uhome/chome/2024EDA/2024EDA017/cudd-3.0.0
CUDD_INCLUDE = $(CUDDDIR)/cudd
UTIL_INCLUDE = $(CUDDDIR)/util
CUDD_LIB = $(CUDDDIR)/cudd/.libs/libcudd.a
```
### Linking with CUDD
The CUDD library is linked statically in this project. Ensure that the libcudd.a file is present in the specified CUDD_LIB path and that your compiler can access it during the linking phase of the build process.