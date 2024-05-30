#!/bin/bash

# Define the source file and the output program
SOURCE_FILE="Lab2.cpp"
OUTPUT_PROGRAM="Lab2"

# Compile the C++ program
g++ -o $OUTPUT_PROGRAM $SOURCE_FILE

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program..."
    ./$OUTPUT_PROGRAM
else
    echo "Compilation failed."
fi