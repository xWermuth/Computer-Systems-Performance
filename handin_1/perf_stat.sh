#! /bin/bash

date=$(date --iso-8601=seconds)
algos="concurrent
parallel
"

for i in {1..10}
do
    for algo in $algos
    do
        dir="perf-stat/${algo}/${date}"

        mkdir -p $dir

        h=1
        perf stat -ddd -D 8000 -o "${dir}/t1-h$h-i$i.txt" ./build/handin_1 -t 1 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t2-h$h-i$i.txt" ./build/handin_1 -t 2 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t4-h$h-i$i.txt" ./build/handin_1 -t 4 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t8-h$h-i$i.txt" ./build/handin_1 -t 8 -h "$h" -a "$algo" -q -p # &
        # wait
        perf stat -ddd -D 8000 -o "${dir}/t16-h$h-i$i.txt" ./build/handin_1 -t 16 -h "$h" -a "$algo" -q -p
        perf stat -ddd -D 8000 -o "${dir}/t32-h$h-i$i.txt" ./build/handin_1 -t 32 -h "$h" -a "$algo" -q -p

        h=2
        perf stat -ddd -D 8000 -o "${dir}/t1-h$h-i$i.txt" ./build/handin_1 -t 1 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t2-h$h-i$i.txt" ./build/handin_1 -t 2 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t4-h$h-i$i.txt" ./build/handin_1 -t 4 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t8-h$h-i$i.txt" ./build/handin_1 -t 8 -h "$h" -a "$algo" -q -p # &
        # wait
        perf stat -ddd -D 8000 -o "${dir}/t16-h$h-i$i.txt" ./build/handin_1 -t 16 -h "$h" -a "$algo" -q -p
        perf stat -ddd -D 8000 -o "${dir}/t32-h$h-i$i.txt" ./build/handin_1 -t 32 -h "$h" -a "$algo" -q -p

        h=3
        perf stat -ddd -D 8000 -o "${dir}/t1-h$h-i$i.txt" ./build/handin_1 -t 1 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t2-h$h-i$i.txt" ./build/handin_1 -t 2 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t4-h$h-i$i.txt" ./build/handin_1 -t 4 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t8-h$h-i$i.txt" ./build/handin_1 -t 8 -h "$h" -a "$algo" -q -p # &
        # wait
        perf stat -ddd -D 8000 -o "${dir}/t16-h$h-i$i.txt" ./build/handin_1 -t 16 -h "$h" -a "$algo" -q -p
        perf stat -ddd -D 8000 -o "${dir}/t32-h$h-i$i.txt" ./build/handin_1 -t 32 -h "$h" -a "$algo" -q -p

        h=4
        perf stat -ddd -D 8000 -o "${dir}/t1-h$h-i$i.txt" ./build/handin_1 -t 1 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t2-h$h-i$i.txt" ./build/handin_1 -t 2 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t4-h$h-i$i.txt" ./build/handin_1 -t 4 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t8-h$h-i$i.txt" ./build/handin_1 -t 8 -h "$h" -a "$algo" -q -p # &
        # wait
        perf stat -ddd -D 8000 -o "${dir}/t16-h$h-i$i.txt" ./build/handin_1 -t 16 -h "$h" -a "$algo" -q -p
        perf stat -ddd -D 8000 -o "${dir}/t32-h$h-i$i.txt" ./build/handin_1 -t 32 -h "$h" -a "$algo" -q -p

        h=5
        perf stat -ddd -D 8000 -o "${dir}/t1-h$h-i$i.txt" ./build/handin_1 -t 1 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t2-h$h-i$i.txt" ./build/handin_1 -t 2 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t4-h$h-i$i.txt" ./build/handin_1 -t 4 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t8-h$h-i$i.txt" ./build/handin_1 -t 8 -h "$h" -a "$algo" -q -p # &
        # wait
        perf stat -ddd -D 8000 -o "${dir}/t16-h$h-i$i.txt" ./build/handin_1 -t 16 -h "$h" -a "$algo" -q -p
        perf stat -ddd -D 8000 -o "${dir}/t32-h$h-i$i.txt" ./build/handin_1 -t 32 -h "$h" -a "$algo" -q -p

        h=6
        perf stat -ddd -D 8000 -o "${dir}/t1-h$h-i$i.txt" ./build/handin_1 -t 1 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t2-h$h-i$i.txt" ./build/handin_1 -t 2 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t4-h$h-i$i.txt" ./build/handin_1 -t 4 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t8-h$h-i$i.txt" ./build/handin_1 -t 8 -h "$h" -a "$algo" -q -p # &
        # wait
        perf stat -ddd -D 8000 -o "${dir}/t16-h$h-i$i.txt" ./build/handin_1 -t 16 -h "$h" -a "$algo" -q -p
        perf stat -ddd -D 8000 -o "${dir}/t32-h$h-i$i.txt" ./build/handin_1 -t 32 -h "$h" -a "$algo" -q -p

        h=7
        perf stat -ddd -D 8000 -o "${dir}/t1-h$h-i$i.txt" ./build/handin_1 -t 1 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t2-h$h-i$i.txt" ./build/handin_1 -t 2 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t4-h$h-i$i.txt" ./build/handin_1 -t 4 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t8-h$h-i$i.txt" ./build/handin_1 -t 8 -h "$h" -a "$algo" -q -p # &
        # wait
        perf stat -ddd -D 8000 -o "${dir}/t16-h$h-i$i.txt" ./build/handin_1 -t 16 -h "$h" -a "$algo" -q -p
        perf stat -ddd -D 8000 -o "${dir}/t32-h$h-i$i.txt" ./build/handin_1 -t 32 -h "$h" -a "$algo" -q -p

        h=8
        perf stat -ddd -D 8000 -o "${dir}/t1-h$h-i$i.txt" ./build/handin_1 -t 1 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t2-h$h-i$i.txt" ./build/handin_1 -t 2 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t4-h$h-i$i.txt" ./build/handin_1 -t 4 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t8-h$h-i$i.txt" ./build/handin_1 -t 8 -h "$h" -a "$algo" -q -p # &
        # wait
        perf stat -ddd -D 8000 -o "${dir}/t16-h$h-i$i.txt" ./build/handin_1 -t 16 -h "$h" -a "$algo" -q -p
        perf stat -ddd -D 8000 -o "${dir}/t32-h$h-i$i.txt" ./build/handin_1 -t 32 -h "$h" -a "$algo" -q -p

        h=9
        perf stat -ddd -D 8000 -o "${dir}/t1-h$h-i$i.txt" ./build/handin_1 -t 1 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t2-h$h-i$i.txt" ./build/handin_1 -t 2 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t4-h$h-i$i.txt" ./build/handin_1 -t 4 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t8-h$h-i$i.txt" ./build/handin_1 -t 8 -h "$h" -a "$algo" -q -p # &
        # wait
        perf stat -ddd -D 8000 -o "${dir}/t16-h$h-i$i.txt" ./build/handin_1 -t 16 -h "$h" -a "$algo" -q -p
        perf stat -ddd -D 8000 -o "${dir}/t32-h$h-i$i.txt" ./build/handin_1 -t 32 -h "$h" -a "$algo" -q -p

        h=10
        perf stat -ddd -D 8000 -o "${dir}/t1-h$h-i$i.txt" ./build/handin_1 -t 1 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t2-h$h-i$i.txt" ./build/handin_1 -t 2 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t4-h$h-i$i.txt" ./build/handin_1 -t 4 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t8-h$h-i$i.txt" ./build/handin_1 -t 8 -h "$h" -a "$algo" -q -p # &
        # wait
        perf stat -ddd -D 8000 -o "${dir}/t16-h$h-i$i.txt" ./build/handin_1 -t 16 -h "$h" -a "$algo" -q -p
        perf stat -ddd -D 8000 -o "${dir}/t32-h$h-i$i.txt" ./build/handin_1 -t 32 -h "$h" -a "$algo" -q -p

        h=11
        perf stat -ddd -D 8000 -o "${dir}/t1-h$h-i$i.txt" ./build/handin_1 -t 1 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t2-h$h-i$i.txt" ./build/handin_1 -t 2 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t4-h$h-i$i.txt" ./build/handin_1 -t 4 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t8-h$h-i$i.txt" ./build/handin_1 -t 8 -h "$h" -a "$algo" -q -p # &
        # wait
        perf stat -ddd -D 8000 -o "${dir}/t16-h$h-i$i.txt" ./build/handin_1 -t 16 -h "$h" -a "$algo" -q -p
        perf stat -ddd -D 8000 -o "${dir}/t32-h$h-i$i.txt" ./build/handin_1 -t 32 -h "$h" -a "$algo" -q -p

        h=12
        perf stat -ddd -D 8000 -o "${dir}/t1-h$h-i$i.txt" ./build/handin_1 -t 1 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t2-h$h-i$i.txt" ./build/handin_1 -t 2 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t4-h$h-i$i.txt" ./build/handin_1 -t 4 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t8-h$h-i$i.txt" ./build/handin_1 -t 8 -h "$h" -a "$algo" -q -p # &
        # wait
        perf stat -ddd -D 8000 -o "${dir}/t16-h$h-i$i.txt" ./build/handin_1 -t 16 -h "$h" -a "$algo" -q -p
        perf stat -ddd -D 8000 -o "${dir}/t32-h$h-i$i.txt" ./build/handin_1 -t 32 -h "$h" -a "$algo" -q -p

        h=13
        perf stat -ddd -D 8000 -o "${dir}/t1-h$h-i$i.txt" ./build/handin_1 -t 1 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t2-h$h-i$i.txt" ./build/handin_1 -t 2 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t4-h$h-i$i.txt" ./build/handin_1 -t 4 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t8-h$h-i$i.txt" ./build/handin_1 -t 8 -h "$h" -a "$algo" -q -p # &
        # wait
        perf stat -ddd -D 8000 -o "${dir}/t16-h$h-i$i.txt" ./build/handin_1 -t 16 -h "$h" -a "$algo" -q -p
        perf stat -ddd -D 8000 -o "${dir}/t32-h$h-i$i.txt" ./build/handin_1 -t 32 -h "$h" -a "$algo" -q -p

        h=14
        perf stat -ddd -D 8000 -o "${dir}/t1-h$h-i$i.txt" ./build/handin_1 -t 1 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t2-h$h-i$i.txt" ./build/handin_1 -t 2 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t4-h$h-i$i.txt" ./build/handin_1 -t 4 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t8-h$h-i$i.txt" ./build/handin_1 -t 8 -h "$h" -a "$algo" -q -p # &
        # wait
        perf stat -ddd -D 8000 -o "${dir}/t16-h$h-i$i.txt" ./build/handin_1 -t 16 -h "$h" -a "$algo" -q -p
        perf stat -ddd -D 8000 -o "${dir}/t32-h$h-i$i.txt" ./build/handin_1 -t 32 -h "$h" -a "$algo" -q -p

        h=15
        perf stat -ddd -D 8000 -o "${dir}/t1-h$h-i$i.txt" ./build/handin_1 -t 1 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t2-h$h-i$i.txt" ./build/handin_1 -t 2 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t4-h$h-i$i.txt" ./build/handin_1 -t 4 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t8-h$h-i$i.txt" ./build/handin_1 -t 8 -h "$h" -a "$algo" -q -p # &
        # wait
        perf stat -ddd -D 8000 -o "${dir}/t16-h$h-i$i.txt" ./build/handin_1 -t 16 -h "$h" -a "$algo" -q -p
        perf stat -ddd -D 8000 -o "${dir}/t32-h$h-i$i.txt" ./build/handin_1 -t 32 -h "$h" -a "$algo" -q -p

        h=16
        perf stat -ddd -D 8000 -o "${dir}/t1-h$h-i$i.txt" ./build/handin_1 -t 1 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t2-h$h-i$i.txt" ./build/handin_1 -t 2 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t4-h$h-i$i.txt" ./build/handin_1 -t 4 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t8-h$h-i$i.txt" ./build/handin_1 -t 8 -h "$h" -a "$algo" -q -p # &
        # wait
        perf stat -ddd -D 8000 -o "${dir}/t16-h$h-i$i.txt" ./build/handin_1 -t 16 -h "$h" -a "$algo" -q -p
        perf stat -ddd -D 8000 -o "${dir}/t32-h$h-i$i.txt" ./build/handin_1 -t 32 -h "$h" -a "$algo" -q -p

        h=17
        perf stat -ddd -D 8000 -o "${dir}/t1-h$h-i$i.txt" ./build/handin_1 -t 1 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t2-h$h-i$i.txt" ./build/handin_1 -t 2 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t4-h$h-i$i.txt" ./build/handin_1 -t 4 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t8-h$h-i$i.txt" ./build/handin_1 -t 8 -h "$h" -a "$algo" -q -p # &
        # wait
        perf stat -ddd -D 8000 -o "${dir}/t16-h$h-i$i.txt" ./build/handin_1 -t 16 -h "$h" -a "$algo" -q -p
        perf stat -ddd -D 8000 -o "${dir}/t32-h$h-i$i.txt" ./build/handin_1 -t 32 -h "$h" -a "$algo" -q -p

        h=18
        perf stat -ddd -D 8000 -o "${dir}/t1-h$h-i$i.txt" ./build/handin_1 -t 1 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t2-h$h-i$i.txt" ./build/handin_1 -t 2 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t4-h$h-i$i.txt" ./build/handin_1 -t 4 -h "$h" -a "$algo" -q -p # &
        perf stat -ddd -D 8000 -o "${dir}/t8-h$h-i$i.txt" ./build/handin_1 -t 8 -h "$h" -a "$algo" -q -p # &
        # wait
        perf stat -ddd -D 8000 -o "${dir}/t16-h$h-i$i.txt" ./build/handin_1 -t 16 -h "$h" -a "$algo" -q -p
        perf stat -ddd -D 8000 -o "${dir}/t32-h$h-i$i.txt" ./build/handin_1 -t 32 -h "$h" -a "$algo" -q -p

    done