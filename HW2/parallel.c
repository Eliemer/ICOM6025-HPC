#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>

double get_walltime()
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return (double) (tp.tv_sec + tp.tv_usec*1e-6);
}

void force_repulsion(int np, const double *pos, double L, double krepulsion, double *forces)
{

  int i, j, k;
  double rvec [4];
  double s2, s, f;

  // loop over all pairs
  // redistribution and collapse of for loops
  #pragma omp parallel private(i,j,k,rvec,s2,s,f)

  #pragma omp master
  printf("number of cores: %d\n", omp_get_num_threads());

  #pragma omp for
  for(k = 0; k < (np * (np -1))/2; k++)
  {  //np: number of particles
  //*pos:
    // calculate equivalent i,j indexes
    i = k/np;
    j = k%np;

    // convert data dependency between indexes
    if(j<=i)
    {
      i = np - i - 2;
      j = np - j - 1;
    }

    // compute minimum image difference
    rvec[0] = remainder(pos[3*i ] - pos[3*j ], L);
    rvec[1] = remainder(pos[3*i+1] - pos[3*j+1], L);
    rvec[2] = remainder(pos[3*i+2] - pos[3*j+2], L);

    s2 = (rvec [0]* rvec [0]) + (rvec [1]* rvec [1]) + (rvec [2]* rvec [2]);

    if (s2 < 4)
    {
      s = sqrt(s2);
      rvec[0] /= s;
      rvec[1] /= s;
      rvec[2] /= s;
      f = krepulsion*(2.-s);
      //possible section A
      forces[3*i ] += f*rvec[0];
      forces[3*i+1] += f*rvec[1];
      forces[3*i+2] += f*rvec[2];
      //section B
      forces[3*j ] += -f*rvec[0];
      forces[3*j+1] += -f*rvec[1];
      forces[3*j+2] += -f*rvec[2];
    }
  }
}

void file_results(double *forces, int np) {
  int i = 0;
  FILE *fptr;
  fptr = fopen("parallel_forces_results.txt", "w+");

  if(fptr == NULL)
  {
    printf("Error creating or opening file");
    exit(1);
  }
  printf("writing results\n");
  for (i; i < np; i++){
    fprintf(fptr, "force %d\nx = %f\ny = %f\nz = %f\n\n", i, forces[3*i], forces[3*i +1], forces[3*i +2]);
  }
  printf("done\n");
  fclose(fptr);
}

int main(int argc, char *argv[]) {
  int i;
  int np = 100; // default number of particles
  double phi = 0.3; // volume fraction
  double krepulsion = 125.; // force constant
  double *pos; double *forces;
  double L, time0 , time1;

  if (argc > 1)
    np = atoi(argv[1]);
  if (argc > 2)
    srand(atoi(argv[2]));

  L = pow(4./3.*3.1415926536*np/phi, 1./3.);

  // generate random particle positions inside simulation box
  forces = (double *) malloc(3*np*sizeof(double));
  pos = (double *) malloc(3*np*sizeof(double));

  for (i=0; i<3*np; i++)
    pos[i] = rand()/(double)RAND_MAX*L;
    forces[i] = 0.;

  // measure execution time of this function
  time0 = get_walltime ();
  force_repulsion(np, pos, L, krepulsion, forces);
  time1 = get_walltime ();

  printf("number of particles: %d\n", np);
  printf("elapsed time: %f\n", time1-time0);

  file_results(forces, np);

  free(forces);
  free(pos);
  return 0;
}
