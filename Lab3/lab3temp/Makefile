# Makefile for Lab3 project

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17

# Target executable
TARGET = lab3

# Source files
SOURCES = main.cpp Grid.cpp Router.cpp

# Default target
all: $(TARGET)

# Rule to compile the source files into the executable
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# Clean up the generated files
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean
