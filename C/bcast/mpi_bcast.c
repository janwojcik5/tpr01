#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	const int number_of_iterations=100000;
	int data[64]; 
  // We are assuming at least 2 processes for this task
  if (world_size < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1); 
  }
	if(world_rank==0) 
		data[0]=0;
	else 
		data[0]=1;
    // If we are rank 0, set the number to -1 and send it to process 1
    int i = 0;
    double time = MPI_Wtime();
    for(i = 0; i<number_of_iterations; ++i){
    		MPI_Bcast(data,64,MPI_INT,0,MPI_COMM_WORLD);
	}
	MPI_Barrier(MPI_COMM_WORLD);
    time = MPI_Wtime() - time;
	//printf("Prodess %d. Result: %d\n",world_rank,data[0]);
	//!!! Format wyjscia: ilosc procesow[spacja]liczba iteracji[spacja]czas
	if(world_rank==0) {
		printf("%d %d %f\n",world_size,number_of_iterations,time);
	}
  MPI_Finalize();
} 
