#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char ** argv)
{
    int world_rank, world_size, row_rank, row_size, row_color, col_rank, col_size, col_color, barrier, N = atoi(argv[1]);
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int sqrt_p = (int)sqrt((double)world_size);
    row_color = world_rank/sqrt_p;
    col_color = world_rank % sqrt_p;

    MPI_Comm row_comm, col_comm;
    MPI_Comm_split(MPI_COMM_WORLD, row_color, world_rank, &row_comm);
    MPI_Comm_split(MPI_COMM_WORLD, col_color, world_rank, &col_comm);
    // MPI_Comm_rank(row_comm, &row_rank);
    // MPI_Comm_size(row_comm, &row_size);
    // MPI_Comm_rank(col_comm, &col_rank);
    // MPI_Comm_size(col_comm, &col_size);

    int x = N/sqrt_p;
    int xN = x*N, x_sq = x*x;
    int **matrix;
    int *blockData = (int *)malloc(x_sq*sizeof(int));
    int **blockMatrix = (int**) malloc(sizeof(int*) * x);
    for (int i = 0; i < x; ++i)
        blockMatrix[i] = &(blockData[x*i]);

    int **partialMatrix;
    int *fullbVector;
    int *bVector = (int*) malloc(sizeof(int) * x);
    int *partResult = (int*) malloc(sizeof(int) * x);
    int *partSumResult;
    int *ResVector;
    double minTime, maxTime, timeTaken, timeStart, timeEnd;

    if(world_rank == 0){
        fullbVector = (int*) malloc(sizeof(int) * N);
        ResVector = (int*) malloc(sizeof(int) * N);
        int *fullData = (int *)malloc(N*N*sizeof(int));
        matrix = (int**) malloc(sizeof(int*) * N);

        for (int i = 0; i < N; ++i){
            fullbVector[i] = 1;
            matrix[i] = &(fullData[N*i]);
            for (int j = 0; j < N; ++j)
                matrix[i][j] = i;
        }
    }
    else matrix = (int**) malloc(sizeof(int*));

    if(col_color == 0){
        partSumResult = (int*) malloc(sizeof(int) * x);
        int *partialData = (int *)malloc(xN*sizeof(int));
        partialMatrix = (int**) malloc(sizeof(int*) * x);
        for (int i = 0; i < x; ++i)
            partialMatrix[i] = &(partialData[N*i]);

        MPI_Scatter(&(matrix[0][0]),xN,MPI_INT,&(partialMatrix[0][0]),xN,MPI_INT,0,col_comm);
    }
    else partialMatrix = (int**) malloc(sizeof(int*));

    for (int i = 0; i < x; ++i)    
        MPI_Scatter(&(partialMatrix[i][0]),x,MPI_INT,&(blockMatrix[i][0]),x,MPI_INT,0,row_comm);

    if(row_color == 0)
        MPI_Scatter(fullbVector,x,MPI_INT,bVector,x,MPI_INT,0,row_comm);
    
    MPI_Bcast(bVector,x,MPI_INT,0,col_comm);


    timeStart = MPI_Wtime();
    
    int tmpSum=0;
    for (int i = 0; i < x; ++i)
    {   
        tmpSum = 0;
        for (int j = 0; j < x; ++j)
        {
            tmpSum += (blockMatrix[i][j] * bVector[j]) ;
        }

        partResult[i] = tmpSum;
    }

    MPI_Reduce(partResult, partSumResult, x, MPI_INT, MPI_SUM, 0, row_comm);
    
    if(col_color == 0)
        MPI_Gather(partSumResult, x, MPI_INT, ResVector, x, MPI_INT, 0, col_comm);

    timeEnd = MPI_Wtime();


    // barrier = MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce(&timeStart,&minTime,1,MPI_DOUBLE,MPI_MIN,0,MPI_COMM_WORLD);
    MPI_Reduce(&timeEnd,&maxTime,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

    if(world_rank == 0){
        timeTaken = maxTime-minTime;
        // for (int i = 0; i < N; ++i)
        // {
        //     printf("%d ", ResVector[i]);
        // }
        // printf("\n");
        // printf("Total Time Taken = ");
        printf("%f\n", timeTaken);
    }

    MPI_Comm_free(&row_comm);
    MPI_Comm_free(&col_comm);
    MPI_Finalize();
    return 0;
}