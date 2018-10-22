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

void generate_vector(int *D, int n){
  D = (int*)malloc(sizeof(int)*n);
  for (size_t i = 0; i < n; i++)
    D[i] = rand()%100;

}

void bitonic_convert(int *D, int n, int k){

}
void bitonic_recursive(int *D, int a, int b, int k){
  if (k > 3){
    int temp;
    for(size_t j = a; j < b; j++){
      temp = min(D[j], D[b/2 + j]);
      D[b/2 + j] = max(D[j], D[b/2 + j]);
      D[j] = temp;
    }
    bitonic_recursive(D, 0, b/2, k-1);
    bitonic_recursive(D, b/2 +1, b, k-1);
  }
}

void bitonic_sort(int *D, int n, int k){
  //bitonic_convert(*D, n, k);

  bitonic_recursive(D, 0, n, k);

}
