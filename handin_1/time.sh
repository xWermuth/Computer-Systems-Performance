#! /bin/bash
datetime=$(date --iso-8601=seconds)
outFile="${datetime}_out.log"

rm -f $outFile
echo "algo,ms,t,h" > $outFile

Green='\033[0;32m' 

threads="1
2
4
8
16
32"

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

echo "Measuring Concurrent algorithm"
for h in {1..18}
do
    for t in $threads;
    do
        for _ in {1..10}
        do
            ./build/handin_1 -t "$t" -h "$h" -a concurrent -q >> $outFile
            if [ $? -ne 0 ];
            then
                echo "Non-zero exit code with params '-t $t -h $h -a concurrent -q'" >> $outFile
                break 1
            fi
        done
    done    
done


echo "Measuring Parallel-Buffers algorithm"
for h in {1..18}
do
    for t in $threads;
    do
        for _ in {1..10}
        do
            ./build/handin_1 -t "$t" -h "$h" -a parallel -q >> $outFile
            if [ $? -ne 0 ];
            then
                echo "Non-zero exit code with params '-t $t -h $h -a parallel -q'" >> $outFile
                break 1
            fi
        done
    done    
done

echo "${Green}Successfully measured both algorithms"