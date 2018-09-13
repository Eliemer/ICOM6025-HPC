#!/bin/bash

gcc parallel2_dynamic.c -o dynamic -fopenmp -lm
gcc parallel2_static.c -o static -fopenmp -lm
gcc parallel2_guided.c -o guided -fopenmp -lm

# first argument, number of times the program is ran
# subsequent arguments are the number of particles to be simulated



echo "Running parallel suite: Dynamic"

for i in 100 1000 10000 100000
do
  for j in {1..5}
  do
    if [ $j -eq 1 ]
    then
      > dynamic_$i.txt
    fi
    ./dynamic $i $RANDOM |& tee -a "dynamic_$i.txt"
  done
done

echo "Running parallel suite: Static"

for i in 100 1000 10000 100000
do
  for j in {1..5}
  do
    if [ $j -eq 1 ]
    then
      > static_$i.txt
    fi
    ./static $i $RANDOM |& tee -a "static_$i.txt"
  done
done


echo "Running parallel suite: Guided"

for i in 100 1000 10000 100000
do
  for j in {1..5}
  do
    if [ $j -eq 1 ]
    then
      > guided_$i.txt
    fi
    ./guided $i $RANDOM |& tee -a "guided_$i.txt"
  done
done
