#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void swap(int *xp, int *yp) 
{ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 

int main(int argc, char ** argv)
{
    int rank, size, barrier, N = atoi(argv[1]);
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int x = N/size;
    int xN = x*N;
    int **matrix;

    int *data = (int *)malloc(xN*sizeof(int));
    int **partialMatrix = (int**) malloc(sizeof(int*) * N);
    for (int i = 0; i < N; ++i)
        partialMatrix[i] = &(data[x*i]);

    int *trans_data = (int *)malloc(xN*sizeof(int));
    int **trans_partialMatrix = (int**) malloc(sizeof(int*) * x);
    for (int i = 0; i < x; ++i)
        trans_partialMatrix[i] = &(trans_data[N*i]);

    int *fullbVector;
    int *bVector = (int*) malloc(sizeof(int) * x);
    int *partResult = (int*) malloc(sizeof(int) * N);
    int *ResVector;
    double minTime, maxTime, timeTaken, timeStart, timeEnd;

    if(rank == 0){
        fullbVector = (int*) malloc(sizeof(int) * N);
        ResVector = (int*) malloc(sizeof(int) * N);
        int *fullData = (int *)malloc(N*N*sizeof(int));
        matrix = (int**) malloc(sizeof(int*) * N);

        for (int i = 0; i < N; ++i){
            fullbVector[i] = 1;
            matrix[i] = &(fullData[N*i]);
            for (int j = 0; j < N; ++j)
                matrix[i][j] = 1;
        }

        for (int i = 0; i < N; ++i)
            for (int j = i+1; j < N; ++j)
                swap(&matrix[i][j], &matrix[j][i]);

    }
    else matrix = (int**) malloc(sizeof(int*));

    MPI_Scatter(&(matrix[0][0]),xN,MPI_INT,&(trans_partialMatrix[0][0]),xN,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Scatter(fullbVector,x,MPI_INT,bVector,x,MPI_INT,0,MPI_COMM_WORLD);
    
    for (int j = 0; j < N; ++j)
        for (int i = 0; i < x; ++i)
            partialMatrix[j][i] = trans_partialMatrix[i][j];
    

    timeStart = MPI_Wtime();
        
    int tmpSum=0;
    for (int i = 0; i < N; ++i)
    {   
        tmpSum = 0;
        for (int j = 0; j < x; ++j)
        {
            tmpSum += (partialMatrix[i][j] * bVector[j]) ;
        }

        partResult[i] = tmpSum;
    }

    // barrier = MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce(partResult, ResVector, N, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    timeEnd = MPI_Wtime();


    // barrier = MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce(&timeStart,&minTime,1,MPI_DOUBLE,MPI_MIN,0,MPI_COMM_WORLD);
    MPI_Reduce(&timeEnd,&maxTime,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

    if(rank == 0){
        timeTaken = maxTime-minTime;
        // for (int i = 0; i < N; ++i)
        // {
        //     printf("%d ", ResVector[i]);
        // }
        // printf("\n");
        // printf("Total Time Taken = ");
        printf("%f\n", timeTaken);
    }

    MPI_Finalize();
    return 0;
}