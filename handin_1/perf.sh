#! /bin/bash

# screen -S perf_data_script ./perf.sh

outFile="out.perf.log"

datetime=$(date --iso-8601=seconds)

rm -f $outFile
echo "algo,ms,t,h" > $outFile
events="context-switches,cpu-migrations,cycles,instructions,cache-references,cache-misses"


delay="2500"

if [[ $OSTYPE == "linux-gnu" ]];
then
    delay="8000"
fi

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
    echo "Measuring $algo algorithm" >> $outFile
    for h in {1..18}
    do
        for t in $threads;
        do
            foldername="$datetime/$algo-$t-$h"
            mkdir -p "$foldername"
            printf "\tThreads %d hashbits %d\n" "$t" "$h" >> $outFile
            for r in {1..10}
            do
                printf "\t\tIter %d\n" "$r" >> $outFile
                rm -f perf.data perf.data.txt
                perf record -e $events -o perf.data -D $delay ./build/handin_1 -t "$t" -h "$h" -a "$algo" -q -p
                perf script -i perf.data > perf.data.txt
                perf report |& tee "./$foldername/report-$r.txt"
                if [ $? -ne 0 ];
                then
                    echo "Non-zero exit code with params '-t $t -h $h -a $algo -q -p'" >> $outFile
                    break 1
                fi
            done
        done    
    done
done

echo "${Green}Successfully measured both algorithms"