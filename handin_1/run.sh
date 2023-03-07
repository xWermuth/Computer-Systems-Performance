#!/bin/bash
cd ./build || exit
cmake ..
make
./handin_1 -t 16 -h 8 -a parallel