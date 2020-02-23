#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
    int rank, size, N = atoi(argv[1]);
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *data = (int *)malloc(N*sizeof(int));
    int *recv_data = (int *)malloc(N*sizeof(int));
    double minTime, maxTime, timeTaken, timeStart, timeEnd;

    for (int i = 0; i < N; ++i)
        data[i] = 1;

    int buffer_size = (MPI_BSEND_OVERHEAD + (N-2)*sizeof(int));
    char* buffer = malloc(buffer_size);

    // Pass the buffer allocated to MPI so it uses it when we issue MPI_Bsend
    MPI_Buffer_attach(buffer, buffer_size);

    timeStart = MPI_Wtime();
        
    MPI_Bsend(data,N,MPI_INT,(rank+1)%size, 0, MPI_COMM_WORLD);
    MPI_Recv(recv_data,N,MPI_INT, (rank+size-1)%size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    timeEnd = MPI_Wtime();

    // Detach the buffer no-longer used (it will wait for MPI_Bsend message to be sent first)
    MPI_Buffer_detach(&buffer, &buffer_size);

    MPI_Reduce(&timeStart,&minTime,1,MPI_DOUBLE,MPI_MIN,0,MPI_COMM_WORLD);
    MPI_Reduce(&timeEnd,&maxTime,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

    if(rank == 0){
        timeTaken = maxTime-minTime;
        // for (int i = 0; i < N; ++i)
        //     printf("%d ", recv_data[i]);
        // printf("\n");
        // printf("Total Time Taken = ");
        printf("%f\n", timeTaken);
    }

    MPI_Finalize();
    return 0;
}

// MPI_BSEND_OVERHEAD = 96
// for data of size = N*(sizeof(int)), Min buffer size = MPI_BSEND_OVERHEAD + (N-2)*sizeof(int) 