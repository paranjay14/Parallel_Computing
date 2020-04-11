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
    int indx, offset, sz;

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
	for (int iter = 0; iter < x; ++iter){
		for (int recv_rank = 0; recv_rank < rank; ++recv_rank){
			MPI_Bcast(passArray,sz,MPI_FLOAT,recv_rank,MPI_COMM_WORLD);

			for (int i = iter; i < x; ++i){
				pivot = partialMatrix[i][offset];
				indx=0;
				for (int j = offset; j < N; ++j)
					partialMatrix[i][j] -= (pivot*passArray[indx++]);
			}
			offset++;
			sz--;
		}

		indx=0;
		pivot = partialMatrix[iter][offset];
		for (int j = offset; j < N; ++j){
			tmpval = partialMatrix[iter][j] / pivot;
			partialMatrix[iter][j] = tmpval;
			passArray[indx++] = tmpval;
		}

		MPI_Bcast(passArray,sz,MPI_FLOAT,rank,MPI_COMM_WORLD);

		for (int i = iter+1; i < x; ++i){			
			pivot = partialMatrix[i][offset];
			indx=0;
			for (int j = offset; j < N; ++j)
				partialMatrix[i][j] -= (pivot*passArray[indx++]);
		}

		offset++;
		sz--;
		for (int recv_rank = rank+1; recv_rank < P; ++recv_rank){
			MPI_Bcast(passArray,sz,MPI_FLOAT,recv_rank,MPI_COMM_WORLD);
			
			for (int i = iter+1; i < x; ++i){	
				pivot = partialMatrix[i][offset];
				indx=0;
				for (int j = offset; j < N; ++j)
					partialMatrix[i][j] -= (pivot*passArray[indx++]);
			}
			offset++;
			sz--;
		}
		// MPI_Barrier(MPI_COMM_WORLD);
	}

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