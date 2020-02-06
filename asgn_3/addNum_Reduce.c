#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
    int rank, size, N = atoi(argv[1]);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int x = N/size, tmpSum=0, finalSum=0;
    int *data1 = (int*) malloc(sizeof(int) * N);
    int *data2 = (int*) malloc(sizeof(int) * x);
    int *data = (int*) malloc(sizeof(int) * x);
    double maxTime, timeTaken, timeStart, timeEnd;
    MPI_Status status;

    if(rank == 0){
        for (int i = 0; i < N; ++i)
            data1[i] = 1;
        
        for (int i = 1; i < size; ++i)
            MPI_Send(data1 + i*x,x,MPI_INT,i,0,MPI_COMM_WORLD); 
    }

    timeStart = MPI_Wtime();
    if(rank == 0){
        for (int i = 0; i < x; ++i)
            tmpSum += data1[i];
    }
    else{
        MPI_Recv(data2,x,MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        for (int i = 0; i < x; ++i)
            tmpSum += data2[i];
    }

    MPI_Reduce(&tmpSum,&finalSum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

    timeEnd = MPI_Wtime();
    timeTaken = timeEnd-timeStart;
    MPI_Reduce(&timeTaken,&maxTime,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

    if(rank == 0){
        // printf("Total Sum = %d\n", finalSum);
        // printf("Total Time Taken = ");
        printf("%f\n", maxTime);
    }

    MPI_Finalize();
    return 0;
}