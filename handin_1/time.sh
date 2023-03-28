#! /bin/bash


if [[ $OSTYPE == "darwin22.0" ]] || [[ $OSTYPE == "darwin22" ]];
then
    datetime=$(date -Iseconds)
else 
    datetime=$(date --iso-8601=seconds)
fi

logFile="${datetime}.log"
dataFile="${datetime}.data"

echo "algo,ms,t,h" > "$dataFile"

Green='\033[0;32m' 

threads="1
2
4
8
16"

algos="concurrent
parallel
"

cd ./build || exit
if ! cmake .. >> /dev/null 2>> "../$logFile";
then
    exit
fi
if ! make >> /dev/null 2>> "../$logFile";
then 
    exit
fi
cd .. || exit

for algo in $algos
do
    echo "Measuring $algo algorithm" >> "$logFile"
    for h in {1..18}
    do

        for t in $threads;
        do
            printf "\tThreads %d hashbits %d\n" "$t" "$h" >> "$logFile"
            iters=10
            par_limit=24

            while [ $iters -ge 1 ]; 
            do
                started=1
                while [ $(( started * t )) -le $par_limit ] && [ $iters -ge 1 ]; 
                do
                    printf "\t\tIter %d\n" "$iters" >> "$logFile"
                    ./build/handin_1 -t "$t" -h "$h" -a "$algo" -q >> "$dataFile" &
                    started=$((started+1))
                    iters=$((iters-1))
                done
                echo "wait" >> "$logFile"
                wait
            done 
        done   

        
        # Handle special case for 32 threads
        printf "\tThreads 32 hashbits %d\n" "$h" >> "$logFile"
        for r in {1..10}
        do
            printf "\t\tIter %d\n" "$r" >> "$logFile"
            ./build/handin_1 -t 32 -h "$h" -a "$algo" -q >> "$dataFile"
            
            if [ $? -ne 0 ];
            then
                echo "Non-zero exit code with params '-t $t -h $h -a $algo -q'" >> "$logFile"
                break 1
            fi
        done 
    done
done


echo "${Green}Successfully measured both algorithms"