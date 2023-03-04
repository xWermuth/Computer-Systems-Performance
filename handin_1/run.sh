#!/bin/bash
cd ./build
cmake ..
make
./handin_1 -t 4 -h 8 -a buffer