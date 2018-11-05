/*
 * Parallel bitonic sort using CUDA.
 * Compile with
 * nvcc -arch=sm_11 bitonic_sort.cu
 * Based on http://www.tools-of-computing.com/tc/CS/Sorts/bitonic_sort.htm
 * License: BSD 3
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* Every thread gets exactly one value in the unsorted array. */
#define THREADS 4 // 2^9
#define BLOCKS 16 // 2^15
#define NUM_VALS THREADS*BLOCKS

void print_elapsed(clock_t start, clock_t stop);
int random_integer();
void array_print(int *arr, int length);
void array_fill(int *arr, int length);
void bitonic_sort_step(int *dev_values, int j, int k);
void bitonic_sort(int *values);

int main(void)
{
  clock_t start, stop;

  int *values = (int*) malloc( NUM_VALS * sizeof(int));
  array_fill(values, NUM_VALS);

  start = clock();
  bitonic_sort(values); /* Inplace */
  stop = clock();

  print_elapsed(start, stop);

  free(values);
}

void print_elapsed(clock_t start, clock_t stop)
{
  double elapsed = ((double) (stop - start)) / CLOCKS_PER_SEC;
  printf("Elapsed time: %.3fs\n", elapsed);
}

int random_integer()
{
  return rand();
}

void array_print(int *arr, int length)
{
  int i;
  for (i = 0; i < length; ++i) {
    printf("%d ",  arr[i]);
  }
  printf("\n");
}

void array_fill(int *arr, int length)
{
  srand(time(NULL));
  int i;
  for (i = 0; i < length; ++i) {
    arr[i] = random_integer();
  }
}


 __global__ void bitonic_sort_step(int *dev_values, int j, int k)
 {
   unsigned int i, ixj; /* Sorting partners: i and ixj */
   i = threadIdx.x + blockDim.x * blockIdx.x; /* Iteration mapping to individual threads */
   ixj = i^j;

   /* The threads with the lowest ids sort the array. */
   if ((ixj)>i) {
     if ((i&k)==0) {
       /* Sort ascending */
       if (dev_values[i]>dev_values[ixj]) {
         /* exchange(i,ixj); */
         int temp = dev_values[i];
         dev_values[i] = dev_values[ixj];
         dev_values[ixj] = temp;
       }
     }
     if ((i&k)!=0) {
       /* Sort descending */
       if (dev_values[i]<dev_values[ixj]) {
         /* exchange(i,ixj); */
         int temp = dev_values[i];
         dev_values[i] = dev_values[ixj];
         dev_values[ixj] = temp;
       }
     }
   }
 }

/**
 * Inplace bitonic sort using CUDA.
 */
void bitonic_sort(int *values)
{
  int *dev_values;
  size_t size = NUM_VALS * sizeof(int);

  cudaMalloc((void**) &dev_values, size);
  cudaMemcpy(dev_values, values, size, cudaMemcpyHostToDevice);

  dim3 blocks(BLOCKS,1);    /* Number of blocks   */
  dim3 threads(THREADS,1);  /* Number of threads  */

  int j, k;
  /* Major step */
  for (k = 2; k <= NUM_VALS; k<<=1) {
    /* Minor step */
    for (j = k>>1; j > 0; j>>=1) {
      bitonic_sort_step<<<blocks, threads>>>(dev_values, j, k);
    }
  }
  cudaMemcpy(values, dev_values, size, cudaMemcpyDeviceToHost);
  cudaFree(dev_values);

}
