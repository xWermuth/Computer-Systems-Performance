#!/bin/bash
cd ./build
cmake ..
make
./handin_1 -t 32 -h 2 -a parallel