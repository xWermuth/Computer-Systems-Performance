#!/bin/bash
cd ./build || exit
cmake ..
make
./handin_1 -t 32 -h 14 -a parallel