#!/bin/bash
cd ./build || exit
cmake ..
make
./handin_1 -t 1 -h 1 -a parallel