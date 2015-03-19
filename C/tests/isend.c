#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Status status;

  // We are assuming at least 2 processes for this task
  if (world_size < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1); 
  }
  int number;
  if (world_rank == 0) {
    // If we are rank 0, set the number to -1 and send it to process 1
    number = -1;
    MPI_Request request;
    MPI_Isend(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);
    MPI_Finalize();
  } else if (world_rank == 1) {

    MPI_Request request;
    
    MPI_Irecv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);
    MPI_Finalize();
    
    printf("Process 1 received number %d from process 0\n", number);
  }
  MPI_Finalize();
}
