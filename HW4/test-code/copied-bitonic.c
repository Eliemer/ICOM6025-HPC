#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

double get_walltime()
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return (double) (tp.tv_sec + tp.tv_usec*1e-6);
}

void generate_vector(int *D, int n){
  printf("Initial Array: \n");
  for (size_t i = 0; i < n; i++){
    D[i] = rand()%100;
    printf("%d ", D[i]);
  }
}

void generate_bitonic_vector(int *a, int N){
  int b = rand()%N;
  a[0] = rand()%25;
  for (size_t i = 1; i < b; i++){
    a[i] = a[i-1] + rand()%10;
  }
  for (size_t i = b; i < N; i++){
    a[i] = a[i-1] - rand()%10;
  }
  printf("\n\nInitial Bitonic Sequence:\n");
  for (size_t i = 0; i < N; i++){
    printf("%d ", a[i]);
  }
}

void compAndSwap(int *a, int i, int j, int dir){
  if (dir == (a[i]>a[j])){
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
  }
}

void bitonicMerge(int *a, int low, int cnt, int dir){
  if (cnt > 1){
    int u = cnt/2;
    for (size_t i = low; i < low+u; i++) {
      compAndSwap(a, i, i+u, dir);
    }
    bitonicMerge(a, low, u, dir);
    bitonicMerge(a, low+u, u, dir);
  }
}

void bitonicSort(int *a, int low, int cnt, int dir){
  if (cnt > 1){
    int u = cnt/2;
    bitonicSort(a, low, u, 1);
    bitonicSort(a, low+u, u, 0);
    bitonicMerge(a, low, cnt, dir);
  }
}

void sort(int *a, int N, int up){
  bitonicSort(a, 0, N, up);
}

int main(int argc, char const *argv[]) {

  int *a;
  int N,k;
  int up =1;

  if (argc > 1){
    k = atoi(argv[1]);
  } else {
    printf("number of nodes = 2^k\nenter the value of k: \n");
    scanf("%d", &k);
  }

  N = pow(2, k);
  a = (int*)malloc(sizeof(int)*N);

  if (argc > 2){
    srand(atoi(argv[2]));
  } else {
    srand(get_walltime());
  }

  generate_vector(a, N);
  //generate_bitonic_vector(a, N);

  sort(a, N, up);

  printf("\nSorted array: \n");
  for (size_t i = 0; i < N; i++) {
    printf("%d ", a[i]);
  }
  printf("\n\n");
  return 0;
}
