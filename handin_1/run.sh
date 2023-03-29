#!/bin/bash
cd ./build || exit
cmake ..
make
./handin_1 -t 4 -h 4 -a concurrent
