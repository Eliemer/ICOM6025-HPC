#!/bin/bash

gcc parallel.c -o parallel -fopenmp -lm
gcc serial.c -o serial -lm
gcc comparator.c -o comparator

echo "Running parrallel program ..."
echo "with arguments $1, $3"
./parallel $1 $3

echo "Running serial program ..."
echo "with arguments $2, $3"
./serial $2 $3

echo "Running comparator to verify results ..."
x="$(./comparator)"
echo "${x}"
if [ "${x}" = "Both files are equal." ]; then
  echo "Verified results are correct"
else
  echo "Results are incorrect"
fi
