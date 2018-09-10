gcc parallel.c -o parallel -fopenmp -lm
gcc serial.c -o serial -lm
gcc comparator.c -o comparator

echo "Running parrallel program ..."
./parallel $1 $3

echo "Running serial program ..."
./serial $2 $3

echo "Running comparator to verify results ..."
x = $(./comparator)
if [ "$x" = "0" ]; then
  echo "Verified results are correct"
else
  echo "Results are incorrect"
fi
