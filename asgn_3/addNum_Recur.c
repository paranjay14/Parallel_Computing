#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
    int rank, size, barrier, N = atoi(argv[1]);
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int x, currSize=0, tmpSize=size, localSum=0, updtSum=0, level=0, maxLevel=0;
    // int *data1 = (int*) malloc(sizeof(int) * N);
    // int *data2 = (int*) malloc(sizeof(int) * N/2);
    int *data = (int*) malloc(sizeof(int) * N);
    double minTime, maxTime, timeTaken, timeStart, timeEnd;
    MPI_Status status;

    while(tmpSize != 1){
        maxLevel++;
        tmpSize = tmpSize >> 1;
    }

    /* Initialisation */
    if(rank == 0){
        for (int i = 0; i < N; ++i)
            data[i] = 1;
    }
    
    /* Distribution */
    currSize = size;
    x=N;
    for (int i = 1; i <= maxLevel; ++i)
    {
        x = x >> 1;
        currSize = currSize >> 1;
        if((rank % (currSize*2)) == 0)
            MPI_Send(data+x, x, MPI_INT, rank+currSize, i, MPI_COMM_WORLD);
        else if((rank % currSize) == 0)
            MPI_Recv(data, x, MPI_INT, rank-currSize, i, MPI_COMM_WORLD, &status);
    }
    x = N/size;
    
    /* time Starts */
    timeStart = MPI_Wtime();

    /* Computation */
    for (int i = 0; i < x; ++i)
    {
        localSum += data[i];
    }

    /* Gathering/Collecting */
    currSize=1;
    for (int i = maxLevel; i >= 1; i--)
    {
        tmpSize = currSize << 1;
        if((rank % tmpSize) == 0){
            MPI_Recv(&updtSum, 1, MPI_INT, rank+currSize, i, MPI_COMM_WORLD, &status);
            localSum += updtSum;
        }
        else if((rank % currSize) == 0)
            MPI_Send(&localSum, 1, MPI_INT, rank-currSize, i, MPI_COMM_WORLD);

        currSize = tmpSize;
    }

    /* time Ends */
    timeEnd = MPI_Wtime();

    barrier = MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce(&timeStart,&minTime,1,MPI_DOUBLE,MPI_MIN,0,MPI_COMM_WORLD);
    MPI_Reduce(&timeEnd,&maxTime,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

    if(rank == 0){
        timeTaken = maxTime-minTime;
        // printf("Total Sum = %d\n", localSum);
        // printf("Total Time Taken = ");
        printf("%f\n", timeTaken);
    }

    MPI_Finalize();
    return 0;
}