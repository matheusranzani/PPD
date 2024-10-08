#!/bin/bash

echo "100 100 100" > 2048

# Compilando!
gcc -Wall mm1.c -o mm1 -O3 -fopenmp

for i in {1,2,4,8,16,32,64,128}; do
        for j in {1..3}; do
                echo "$i thread(s) / run $j"
                OMP_NUM_THREADS=$i time ./mm1 < 2048 > /dev/null
        done
        echo
done
