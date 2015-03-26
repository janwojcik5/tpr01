#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/*Program performs simple example of collective communication:
 * For each process, 
 * scan function computes the product of rank of all processes increased
 * by one (each rank)
 * up to that process (inclusive).
 * Every process prints down its number, the result and elapsed time.
 */
int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // We are assuming at least 2 processes for this task
  if (world_size < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1); 
  }
	int numbers,i;
	double time=MPI_Wtime();	
	int number=world_rank+1,result;
	//loading the operation by repeating the scan many times
	for(i=0;i<100000;i++) {
	//	printf("From inside the loop\n");
	//	sleep(1);
		MPI_Scan(&number,&result,1,MPI_INT,MPI_PROD,MPI_COMM_WORLD);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	time=MPI_Wtime()-time;
  	MPI_Finalize();
	if(world_rank==world_size-1) {
		//printf("Result: %d. Elapsed time: %f",result,time);
		//!!! Format: liczba_procesow[spacja]czas !!!
		printf("%d %f\n",world_size,time);
	}
}
