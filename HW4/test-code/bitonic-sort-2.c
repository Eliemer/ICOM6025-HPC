#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define max(a,b) \
  ({__typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define min(a,b) \
  ({__typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })

void bitonic_convert(int *D, int n, int k);
void bitonic_recursive(int *D, int a, int b, int k);
void bitonic_sort(int *D, int n, int k);
void generate_vector(int *D, int n);
void compare_and_swap(int a, int b, int dir);

double get_walltime()
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return (double) (tp.tv_sec + tp.tv_usec*1e-6);
}

int main(int argc, char const *argv[]) {
  /* code */
  int *D;
  int n, k;
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
  D = (int*)malloc(sizeof(int)*8);
  for (size_t i = 0; i < 8; i++){
    if (i > 4){
      D[i] = 7 - i;
    } else {
      D[i] = i;
    }
  }

  printf("printing array\n\n");
  for (size_t i = 0; i < 8; i++) {
    printf("%d ", D[i]);
  }

  printf("\n\nbegin bitonic sort\n");
  time0 = get_walltime();
  bitonic_sort(D, n, k);
  time1 = get_walltime();

  printf("finalize bitonic sort\n\n");
  for (size_t i = 0; i < 8; i++) {
    printf("%d ", D[i]);
  }
  printf("\n\n");
  return 0;
}

void compare_and_swap(int *arr,int a, int b, int dir){
  if(dir == (arr[a] > arr[b])){
    int temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
  }
}

void generate_vector(int *D, int n){
  D = (int*)malloc(sizeof(int)*n);
  for (size_t i = 0; i < n; i++)
    D[i] = rand()%100;
}

// void bitonic_convert(int *D, int n, int k){
//
// }

void bitonic_sort(int *D, int n, int k){
  // bitonic_convert(*D, n, k);
  // every thread block contains n/(k - 5) of the contents of the array

  // for (size_t i = 0; i < k-5; i++) {
    // local_sort()
  // }
  // compare_and_swap between thread blocks

}
