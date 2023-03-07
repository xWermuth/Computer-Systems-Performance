#!/bin/bash
cd ./build || exit
cmake ..
make
./handin_1 -t 2 -h 1 -a concurrent