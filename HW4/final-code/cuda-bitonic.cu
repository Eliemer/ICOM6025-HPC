#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

double get_walltime()
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return (double) (tp.tv_sec + tp.tv_usec*1e-6);
}

void file_results(double time_elapsed, int k) {
  FILE *fptr;
  fptr = fopen("cuda_times.txt", "a");

  if(fptr == NULL){
    printf("\nError creating or opening file\n");
    exit(1);
  }

  fprintf(fptr, "cuda\npower of 2: %d\ntime elapsed: %f\n\n", k, time_elapsed);
  fclose(fptr);
}

void compare_and_swap(int *arr,int a, int b, int dir){
  if(dir == (arr[a] > arr[b])){
    int temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
  }
}

void generate_vector(int *D, int n){
  for (size_t i = 0; i < n; i++)
    D[i] = rand()%100;
}

void print_array(int *D, int n) {
  for (size_t i = 0; i < n; i++) {
    printf("%d  ", D[i]);
  }
}

__global__
void bitonic_sort_step(int *D, int n, int a, int b){
  int i, ixa, temp;
  i = threadIdx.x + (blockDim.x * blockIdx.x);
  ixa = i^a;

  if (ixa > i){
    if ((i & b) == 0){
      if (D[i] > D[ixa]){
        temp = D[i];
        D[i] = D[ixa];
        D[ixa] = temp;
      }
    }
    if ((i & b) != 0){
      if (D[i] < D[ixa]){
        temp = D[i];
        D[i] = D[ixa];
        D[ixa] = temp;
      }
    }
  }
    //print_array(D, n);
    //printf("\t\ti: %d\tixa: %d\ta: %d\tb: %d\n", i, ixa, a, b);
}

void bitonic_sort(int *D, int n, int k){
  int a, b;
  int thread_num=512;
  int block_num=1;
  int *dev_D;

  if (k > 9){
    block_num = pow(2, k-8);
  }
  while (thread_num > n){
    thread_num >>= 1;
  }

  cudaMalloc((void**) &dev_D, sizeof(int) * n);
  cudaMemcpy(dev_D, D, sizeof(int) * n, cudaMemcpyHostToDevice);

  dim3 blocks(block_num);
  dim3 threads(thread_num);

  for (b = 2; b <= n; b <<= 1){
    for (a = b>>1; a > 0; a >>= 1){
      bitonic_sort_step<<<blocks, threads>>>(dev_D, n, a, b);
    }
  }

  cudaMemcpy(D, dev_D, sizeof(int) * n, cudaMemcpyDeviceToHost);
  cudaFree(dev_D);
}

int main(int argc, char const *argv[]) {
  /* code */
  int *D; // Array of values
  int n, k; // 2^k = n, number of elements in the array
  double time0, time1;

  if (argc > 1){
    k = atoi(argv[1]);
  } else {
    printf("number of nodes = 2^k\nenter the value of k: \n");
    scanf("%d", &k);
  }

  if (argc > 2){
    srand(atoi(argv[2]));
  } else {
    srand(get_walltime());
  }

  printf("\npopulating array\n");
  n = pow(2, k);
  D = (int*)malloc(sizeof(int)*n);
  generate_vector(D, n);

  if (n < 33){
    printf("printing array\n\n");
    for (size_t i = 0; i < n; i++) {
      printf("%d ", D[i]);
    }
  }

  printf("\n\nbegin bitonic sort\n");
  time0 = get_walltime();
  bitonic_sort(D, n, k);
  time1 = get_walltime();

  if (n < 33){
    printf("finalize bitonic sort\n\n");
    for (size_t i = 0; i < n; i++) {
      printf("%d ", D[i]);
    }
  }

  printf("\n\ntime elapsed: %f s\n\n ", time1 - time0);
  file_results(time1-time0, k);

  free(D);
  return 0;
}
