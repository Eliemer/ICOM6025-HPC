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

struct node {
  int label;
  int data;
}

double get_walltime()
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return (double) (tp.tv_sec + tp.tv_usec*1e-6);
}

int main(int argc, char const *argv[]) {
  /* code */
  struct node *D;
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
  D = (int*)malloc(sizeof(struct node)*8);
  for (size_t i = 0; i < 8; i++){
    D[i].label = i;
    if (i > 4){
      D[i].data = 7 - i;
    } else {
      D[i].data = i;
    }
  }

  printf("printing array\n\n");
  for (size_t i = 0; i < 8; i++) {
    printf("%d ", D[i].data);
  }

  printf("\n\nbegin bitonic sort\n");
  time0 = get_walltime();
  bitonic_sort(D, n, k);
  time1 = get_walltime();

  printf("finalize bitonic sort\n\n");
  for (size_t i = 0; i < 8; i++) {
    printf("%d ", D[i].data);
  }
  printf("\n\n");
  return 0;
}

void generate_vector(int *D, int n){
  D = (int*)malloc(sizeof(int)*n);
  for (size_t i = 0; i < n; i++)
    D[i].data = rand()%100;

}

// void bitonic_convert(int *D, int n, int k){
//
// }
// void bitonic_recursive(int *D, int a, int b, int k){
//   if (k > 3){
//     int temp;
//     for(size_t j = a; j < b; j++){
//       temp = min(D[j], D[b/2 + j]);
//       D[b/2 + j] = max(D[j], D[b/2 + j]);
//       D[j] = temp;
//     }
//     bitonic_recursive(D, 0, b/2, k-1);
//     bitonic_recursive(D, b/2 +1, b, k-1);
//   }
// }

 void bitonic_sort_step(struct node *D, int i, int j, int k){
   int m, mxj;
   for (size_t m = 0; m < n; m++) {
     mxj = m^j;
     if ((mxj) > i){
       compare_and_swap(D, m, mxj, m&i);
     }
   }
 }

void bitonic_sort(struct node *D, int n, int k){
  bool dir;
  for (size_t i = 2; i < n; i = i << 1) {
    for (size_t j = k >> 1; j > 0; j = j >> 1) {
      bitonic_sort_step(*D, i, j, k);
    }
  }
}
void compare_and_swap(struct node *arr,int a, int b, bool dir){
  if(dir == (arr[a].data > arr[b].data)){
    int temp = arr[a].data;
    arr[a].data = arr[b].data;
    arr[b].data = temp;
  }
}
