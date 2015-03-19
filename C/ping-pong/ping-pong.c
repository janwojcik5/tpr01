#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

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
  
  if (argc != 3) {
    fprintf(stderr, "correct usage:\nping_pong_c [number_of_sends] [bytes_per_send]\n");
    MPI_Abort(MPI_COMM_WORLD, 1); 
  }
  
  int number_of_sends = atoi(argv[1]);
  int bytes_per_send = atoi(argv[2]);
  char* data = (char*)malloc(bytes_per_send);
  
  double time = MPI_Wtime();
  
  if (world_rank == 0) {
    // If we are rank 0, set the number to -1 and send it to process 1
    int i = 0;
    for(i = 0; i<number_of_sends; ++i){
      MPI_Send(&data, bytes_per_send, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
      MPI_Recv(&data, bytes_per_send, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    time = MPI_Wtime() - time;
    MPI_Barrier(MPI_COMM_WORLD);
    double capacity=(bytes_per_send*number_of_sends*8)/(1000000.0*time*MPI_Wtick());
    printf("%lf Mbit/s capacity\n%lfs of delay on single message\n", capacity, (time*MPI_Wtick()/(float)number_of_sends));
  
  } else if (world_rank == 1) {
    int i = 0;
    for(i = 0; i<number_of_sends; ++i){
      MPI_Recv(&data, bytes_per_send, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(&data, bytes_per_send, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
  }
  
  MPI_Finalize();
} 
