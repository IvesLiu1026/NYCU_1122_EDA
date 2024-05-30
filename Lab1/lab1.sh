#!/bin/bash
make

for i in {1..8}; do
    ./Lab1 "testcases/case${i}.txt" "output/out${i}.txt"
done