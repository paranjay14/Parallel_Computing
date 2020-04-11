#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
    int rank, P, barrier, N = atoi(argv[1]);
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    int x = N/P;
    float *passArray = (float*) malloc(sizeof(float) * N);
    float **matrix;
    float *data = (float *)malloc(x*N*sizeof(float));
    float **partialMatrix = (float**) malloc(sizeof(float*) * x);
    for (int i = 0; i < x; ++i)
        partialMatrix[i] = &(data[N*i]);

    double minTime, maxTime, timeTaken, timeStart, timeEnd;
    float tmpval, pivot;
    int indx, offset, sz, cnt,topRowNum;

	if(rank==0){
		float *fullData = (float *)malloc(N*N*sizeof(float));
        matrix = (float**) malloc(sizeof(float*) * N);

        for (int i = 0; i < N; ++i)
        	matrix[i] = &(fullData[N*i]);

		for (int i = 0; i < N; ++i){
			indx = i%P;			
			for (int j = i; j < N; ++j){
				tmpval = i*j + i+1; 
				matrix[j][i] = tmpval;
				matrix[i][j] = tmpval;
	
				if((j%P == 0) && ((j/P)<x))
					partialMatrix[j/P][i] = 0;
				if((indx == 0) && ((i/P)<x))
					partialMatrix[i/P][j] = tmpval;
			}
			
        	if(P!=1 && indx!=0) MPI_Send(&(matrix[i][0]), N, MPI_FLOAT, indx, 0, MPI_COMM_WORLD);
		}
	}
	else{
		for (int i = 0; i < x; ++i)
			MPI_Recv(&(partialMatrix[i][0]), N, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	MPI_Barrier(MPI_COMM_WORLD);


    timeStart = MPI_Wtime();

    offset=0;
    sz=N;
    indx=-1;
    topRowNum=0;
	for (int iter = 0; iter < N-1; ++iter){
		indx++;
		if(indx==P) topRowNum++, indx=0;

		if(rank==indx){
			cnt=0;
			pivot = partialMatrix[topRowNum][offset];
			for (int j = offset; j < N; ++j){
				tmpval = partialMatrix[topRowNum][j] / pivot;
				partialMatrix[topRowNum][j] = tmpval;
				passArray[cnt++] = tmpval;
			}

			if(P!=1) MPI_Send(passArray, sz, MPI_FLOAT, (rank<P-1)?(rank+1):(0), indx, MPI_COMM_WORLD);

			for (int i = topRowNum+1; i < x; ++i){			
				pivot = partialMatrix[i][offset];
				cnt=0;
				for (int j = offset; j < N; ++j)
					partialMatrix[i][j] -= (pivot*passArray[cnt++]);
			}

			offset++;
			sz--;
		}
		else{
			MPI_Recv(passArray, sz, MPI_FLOAT, (rank>0)?(rank-1):(P-1), indx, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			if((rank+1)%P != indx)
				MPI_Send(passArray, sz, MPI_FLOAT, (rank<P-1)?(rank+1):(0), indx, MPI_COMM_WORLD);

			for (int i = (rank>indx)?(topRowNum):(topRowNum+1) ; i<x; ++i){
				pivot = partialMatrix[i][offset];
				cnt=0;
				for (int j = offset; j < N; ++j)
					partialMatrix[i][j] -= (pivot*passArray[cnt++]);
			}
			offset++;
			sz--;
		}
	}
	if(rank==P-1) partialMatrix[x-1][N-1]=1;
	// 	MPI_Barrier(MPI_COMM_WORLD);

	for (int i = 0; i < x; ++i){
    	if(rank!=0) MPI_Gather(&(partialMatrix[i][0]), N, MPI_FLOAT, NULL, 0, MPI_FLOAT, 0, MPI_COMM_WORLD);	
    	else MPI_Gather(&(partialMatrix[i][0]), N, MPI_FLOAT, &(matrix[i*P][0]), N, MPI_FLOAT, 0, MPI_COMM_WORLD);	
	}

    timeEnd = MPI_Wtime();


    // MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce(&timeStart,&minTime,1,MPI_DOUBLE,MPI_MIN,0,MPI_COMM_WORLD);
    MPI_Reduce(&timeEnd,&maxTime,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

    if(rank == 0){
        timeTaken = maxTime-minTime;
        // for (int i = 0; i < N; ++i){	
        // 	for (int j = 0; j < N; ++j)
        //     	printf("%f ", matrix[i][j]);
        //     printf("\n");
        // }
        // printf("\n\n");
        // printf("Total Time Taken = ");
        printf("%f\n", timeTaken);
    }

    MPI_Finalize();
    return 0;
}