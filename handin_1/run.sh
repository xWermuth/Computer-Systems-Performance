#!/bin/bash
cd ./build || exit
cmake ..
make
./handin_1 -t 32 -h 16 -a parallel -q