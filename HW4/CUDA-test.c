#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
//#include <openacc.h>

double get_walltime()
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return (double) (tp.tv_sec + tp.tv_usec*1e-6);
}

void generate_vector(int* D, int n){
  for (size_t i = 0; i < n; i++){
    D[i] = rand()%100;
  }

  printf("%d, %d, %d, %d... \n", D[0], D[1], D[2], D[3]);
}

//__global__
int serial_vector_sum(int *D, int n){
  int sum = 0;
  for (size_t i = 0; i < n; i++){
    sum = sum + D[i];
  }
  return sum;
}

 int cuda_vector_sum(int* D, int n){
   int sum = 0;
   for (int i = 0; i < n; i++){
     sum += D[i];
   }
 }

int main(int argc, char const *argv[]) {

  int *D, n, sum;

  sum = 0;

  if (argc > 1){
    n = atoi(argv[1]);
  } else {
    n = 100;
  }

  //cudaMallocManaged(&D_gpu, n*sizeof(int));
  D = (int*)malloc(sizeof(int) * n);

  srand(get_walltime());
  generate_vector(D, n);

  //#pragma acc data copy(D, )
  sum = serial_vector_sum(D, n);
  printf("%d\n", sum);
  //serial_vector_sum<<<1,1>>>(*D, n, sum);


  //cudaFree(D_gpu);
  free(D);
  return 0;
}
