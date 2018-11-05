#!/usr/bin/env bash

gcc serial-bitonic.c -lm -o serial
gcc-8 openacc-bitonic.c -lm -fopenacc -o openacc
nvcc cuda-bitonic.cu -o cuda

echo "Running Serial Suite"

for i in 5 10 15 20 25
do
  if [ $i -eq 1 ]
  then
    > serial_times.txt
  fi

  for j in {1..3}
  do
    ./serial $i |& tee -a "serial_times.txt"
  done
done

echo "Running Openacc Suite"

for i in 5 10 15 20 25
do
  if [ $i -eq 1 ]
  then
    > openacc_times.txt
  fi

  for j in {1..3}
  do
    ./serial $i |& tee -a "openacc_times.txt"
  done
done

echo "Running CUDA Suite"

for i in 5 10 15 20 25
do
  if [ $i -eq 1 ]
  then
    > cuda_times.txt
  fi

  for j in {1..3}
  do
    ./serial $i |& tee -a "cuda_times.txt"
  done
done

echo "Suites completed successfully"
