#! /bin/bash

outFile="out.log"

rm -f $outFile
echo "algo,ms,t,h" > $outFile
events="context-switches,cpu-migrations,cycles,instructions,cache-references,cache-misses"

Green='\033[0;32m' 

threads="1
2
4
8
16
32"

algos="concurrent
parallel
"

cd ./build || exit
if ! cmake .. >> /dev/null 2>> $outFile;
then
    exit
fi
if ! make >> /dev/null 2>> $outFile;
then 
    exit
fi
cd .. || exit

for algo in $algos
do
    echo "Measuring $algo algorithm"
    for h in {1..18}
    do
        for t in $threads;
        do
            foldername="$algo-$t-$h"
            mkdir -p "$foldername"
            for _ in {1..10}
            do
                rm -f perf.data perf.data.txt
                perf record -e $events -o perf.data ./build/handin_1 -t "$t" -h "$h" -a "$algo" -q
                perf script -i perf.data > perf.data.txt
                perf report |& tee "./$foldername/report.txt"
                if [ $? -ne 0 ];
                then
                    echo "Non-zero exit code with params '-t $t -h $h -a $algo -q'" >> $outFile
                    break 1
                fi
            done
        done    
    done
done

echo "${Green}Successfully measured both algorithms"