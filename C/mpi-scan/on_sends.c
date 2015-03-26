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
	//MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_RETURN);
	int i;
	double time=MPI_Wtime();	
	int result=1;
	const int number_of_iterations=10000;
	//loading the operation by repeating the scan many times
		
	for(i=0;i<number_of_iterations;i++) {
	//	sleep(1);
		if(world_rank>0)
			if(MPI_Recv(&result,1,MPI_INT,world_rank-1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE)!=MPI_SUCCESS) 
				perror("Problems with MPI_Recv");
			//printf("Result: %d\n",result);
		result*=world_rank+1;
		if(world_rank<world_size-1)
			if(MPI_Send(&result,1,MPI_INT,world_rank+1,0,MPI_COMM_WORLD)!=MPI_SUCCESS) 
				perror("Problems with MPI_Send");
		MPI_Barrier(MPI_COMM_WORLD);//!!!!!!!!!!!!!!!!!!!!!!!!!!! Can be commented out while code below uncommented. The results are 100 times 
		//better than regular MPI_Scan
	}
	//MPI_Barrier(MPI_COMM_WORLD); //!!!!!!!!!!!!!!!!!!!!!!!!! Can be uncommented while commenting out the code above. 
	time=MPI_Wtime()-time;
  	MPI_Finalize();
	if(world_rank==world_size-1) {
		//printf("Result: %d. Elapsed time: %f",result,time);
		//!!! Format wyjscia: liczta procesow[spacja]liczba iteracji[spacja]czas
		printf("%d %d %f\n",world_size,number_of_iterations,time);
	}
}
