#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<omp.h>

#define INFINITY 9999

void floyd(int n, int **G);
void gen_adj_matrix(int n, int **G);
void printSolution(int n, int **D);
void file_results(double comp_time, int np);

double get_walltime()
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return (double) (tp.tv_sec + tp.tv_usec*1e-6);
}

int main(int argc, char *argv[])
{
    int i,j,n,u;
    double time0, time1;

    omp_set_num_threads(omp_get_max_threads());
    srand(4);

    if (argc > 1){
      n = atoi(argv[1]);
    } else {
      printf("Enter no. of vertices:");
      scanf("%d",&n);
    }

    int **G = (int**)malloc(sizeof(int*)*n);
    for (i=0; i< n; i++)
      G[i] = (int*)malloc(sizeof(int)*n);

    gen_adj_matrix(n, G);
    // printf("\nEnter the adjacency matrix:\n");
    //
    // for(i=0;i<n;i++)
    //     for(j=0;j<n;j++)
    //         scanf("%d",&G[i][j]);
    if (n < 21) {
      for (i=0; i<n; i++){
        for (j=0; j<n; j++){
          printf("%d\t", G[i][j]);
        }
        printf("\n");
      }
      printf("\n\n");
    }

    time0 = get_walltime();
    floyd(n, G);
    time1 = get_walltime();

    for (i=0; i< n; i++)
      free(G[i]);
    free(G);

    printf("\n\ntime elapsed:  %f\n", time1 - time0);

    file_results(time1 - time0, n);

    return 0;
}

void gen_adj_matrix(int n, int **G)
{
    int i, j, random;
    for (i = 0; i < n; i++)
    {
      for (j = i; j < n; j++)
      {
        if (i == j)
          G[i][j] = 0;
        else
        {
          if ((rand()% 10) > 5){
            random = rand() % 100;
          } else {
            random = 0;
          }

          G[i][j] = random;
          G[j][i] = random;

        }
      }
    }
}

void floyd(int n, int **G)
{
    int i, j, k;
    int **distance = (int**)malloc(sizeof(int*)*n);
    for (i=0; i<n; i++)
      distance[i] = (int*)malloc(sizeof(int)*n);

    for (i = 0; i< n; i++){
      for (j=0;j<n;j++){
        if (G[i][j]==0)
          distance[i][j] = INFINITY;
        else
          distance[i][j] = G[i][j];
      }
    }




    for(k=0;k<n;k++)
    {
      #pragma omp parallel for private (i)
      for(i=0;i<n;i++)
      {
        #pragma omp parallel for private(j) shared(i)
        for(j=0;j<n;j++)
        {
          if(distance[i][k] + distance[k][j] < distance[i][j])
          {
            distance[i][j] = distance[i][k] + distance[k][j];
          }
        }
      }
    }
    if (n < 15)
      printSolution(n, distance);
}

void printSolution(int n,int **D)
{
  int i,j;
  for(i=0;i<n;i++){
    for(j=0;j<n;j++){
      if(D[i][j] == INFINITY)
        printf("%s\t", "INF");
      else
        printf("%d\t", D[i][j]);
    }
    printf("\n");
  }
}

void file_results(double comp_time, int np) {
  int i = 0;
  int thread_num = 1;
  if (omp_get_num_threads() > 1){
    thread_num = omp_get_num_threads();
  }
  printf("%d\n", omp_get_num_threads());
  printf("%d\n", omp_get_max_threads());
  FILE *fptr;
  char filename[] = {"floyd_results"};
  fptr = fopen(filename, "w+");

  if(fptr == NULL)
  {
    printf("Error creating or opening file");
    exit(1);
  }

  fprintf(fptr, "floyd's algorithm\nnumber of nodes: %d\nnumber of threads: %d\ncomputational time: %f\n\n", np, thread_num, comp_time);

  fclose(fptr);
}

// void dijkstra(int n, int **G, int startnode)
// {
//     int count,mindistance,nextnode,i,j;
//
//     int **cost = (int**)malloc(sizeof(int*)*n);
//     for (i=0; i< n; i++)
//       cost[i] = (int*)malloc(sizeof(int)*n);
//
//     int *distance = (int*)malloc(sizeof(int)*n);
//     int *pred = (int*)malloc(sizeof(int)*n);
//     int *visited = (int*)malloc(sizeof(int)*n);
//
//     //pred[] stores the predecessor of each node
//     //count gives the number of nodes seen so far
//     //create the cost matrix
//     for(i=0;i<n;i++)
//         for(j=0;j<n;j++)
//             if(G[i][j]==0)
//                 cost[i][j]=INFINITY;
//             else
//                 cost[i][j]=G[i][j];
//
//     //initialize pred[],distance[] and visited[]
//     for(i=0;i<n;i++)
//     {
//         distance[i]=cost[startnode][i];
//         pred[i]=startnode;
//         visited[i]=0;
//     }
//
//     distance[startnode]=0;
//     visited[startnode]=1;
//     count=1;
//
//     while(count<n-1)
//     {
//         mindistance=INFINITY;
//
//         //nextnode gives the node at minimum distance
//         for(i=0;i<n;i++)
//             if(distance[i]<mindistance&&!visited[i])
//             {
//                 mindistance=distance[i];
//                 nextnode=i;
//             }
//
//             //check if a better path exists through nextnode
//             visited[nextnode]=1;
//             for(i=0;i<n;i++)
//                 if(!visited[i])
//                     if(mindistance+cost[nextnode][i]<distance[i])
//                     {
//                         distance[i]=mindistance+cost[nextnode][i];
//                         pred[i]=nextnode;
//                     }
//         count++;
//     }
//
//     //print the path and distance of each node
//     for(i=0;i<n;i++)
//         if(i!=startnode)
//         {
//             printf("\nDistance to node %d = %d",i,distance[i]);
//             printf("\nPath = %d",i);
//
//             j=i;
//             do
//             {
//                 j=pred[j];
//                 printf("<-%d",j);
//             }while(j!=startnode);
//     }
//     for (i=0; i< n; i++)
//       free(cost[i]);
//     free(cost);
//     free(distance);
//     free(pred);
//     free(visited);
//
// }
