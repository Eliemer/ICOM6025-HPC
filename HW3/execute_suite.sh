#!/usr/bin/env bash

gcc Dijkstra\'s\ All\ Shortest\ Path\ Openmp.c -fopenmp -o parallel_dijkstra
gcc Floyd\'s\ OpenMP.c -fopenmp -o double_parallel_floyd

echo "Running Parallel Suite: 2D Floyd"

for i in 100 500 1000 5000 10000
do

  if [ $i -eq 1 ]
  then
    > floyd_results
  fi

  for j in {1..5}
  do
    ./double_parallel_floyd $i |& tee -a "floyd_results"
  done
done

echo "Running Paralle Suite: All Shortest Dijkstra"

for i in 100 500 1000 5000 10000
do

  if [ $i -eq 1 ]
  then
    > dijkstra_results
  fi

  for j in {1..5}
  do
    ./parallel_dijkstra $i |& tee -a "dijkstra_results"
  done
done
