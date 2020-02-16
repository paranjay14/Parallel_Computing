#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int main(int argc, char ** argv)
{
    int rank, size, barrier, N = atoi(argv[1]);
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int x = N/size , tmpSum=0;
    int *data2 = (int*) malloc(sizeof(int) * x);
    int *data = (int*) malloc(sizeof(int) * x);
    int *sortedData = (int*) malloc(sizeof(int) * (x*2));
    double minTime, maxTime, timeTaken, timeStart, timeEnd;

    if(rank == 0){
        int *data1 = (int*) malloc(sizeof(int) * N);
        for (int i = 0; i < N; ++i){
            data1[i] = N-i;
            if(i<x) data2[i] = N-i;
        }
        
        for (int i = 1; i < size; ++i)
            MPI_Send(data1 + i*x,x,MPI_INT,i,0,MPI_COMM_WORLD); 
    }
    else
        MPI_Recv(data2,x,MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    timeStart = MPI_Wtime();
    
    qsort(data2, x, sizeof(int), cmpfunc);
        
    int t_i=0, t_rank=rank%2, cnt1=0, cnt2=0, cnt=0, tmp=0, t_x=0;
    for (int i = 0; i < size; ++i)
    {   
        t_i=i%2;
        if(t_i == 0){
            if(t_rank == 1){
                MPI_Send(data2,x,MPI_INT,rank-1,i+1,MPI_COMM_WORLD);
                MPI_Recv(data2,x,MPI_INT,rank-1,i+1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            }
            else if(rank != size-1){
                MPI_Recv(data,x,MPI_INT,rank+1,i+1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

                cnt1=0, cnt2=0, cnt=0, t_x=x*2;
                while(cnt != t_x)
                {
                    if(cnt1 == x){
                        sortedData[cnt++] = data2[cnt2++];
                    }
                    else if(cnt2 == x){
                        sortedData[cnt++] = data[cnt1++];
                    }
                    else if(data2[cnt2] > data[cnt1]){
                        sortedData[cnt++] = data[cnt1++];
                    }
                    else{
                        sortedData[cnt++] = data2[cnt2++];
                    }
                }

                for (int j = 0; j < x; ++j)
                {
                    data2[j] = sortedData[j];
                }

                MPI_Send(sortedData + x,x,MPI_INT,rank+1,i+1,MPI_COMM_WORLD);
            }
        }
        else{
            if((t_rank == 1) && (rank != size-1)){
                MPI_Send(data2,x,MPI_INT,rank+1,i+1,MPI_COMM_WORLD);
                MPI_Recv(data2,x,MPI_INT,rank+1,i+1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            }
            else if(t_rank==0 && rank!=0){
                MPI_Recv(data,x,MPI_INT,rank-1,i+1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

                cnt1=0, cnt2=0, cnt=0, t_x=x*2;
                while(cnt != t_x)
                {
                    if(cnt1 == x){
                        sortedData[cnt++] = data2[cnt2++];
                    }
                    else if(cnt2 == x){
                        sortedData[cnt++] = data[cnt1++];
                    }
                    else if(data2[cnt2] > data[cnt1]){
                        sortedData[cnt++] = data[cnt1++];
                    }
                    else{
                        sortedData[cnt++] = data2[cnt2++];
                    }
                }

                for (int j = 0; j < x; ++j)
                {
                    data2[j] = sortedData[x+j];
                }

                MPI_Send(sortedData,x,MPI_INT,rank-1,i+1,MPI_COMM_WORLD);
            }
        }
    }


    if(rank!=0)
        MPI_Send(data2,x,MPI_INT,0,0,MPI_COMM_WORLD);
    else{
        int *data1 = (int*) malloc(sizeof(int) * N);

        for (int i = 0; i < x; ++i)
            data1[i] = data2[i];

        for (int i = 1; i < size; ++i)
        {
            MPI_Recv(data1 + i*x,x,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }

        // for (int i = 0; i < N; ++i)
        //     printf("%d ", data1[i]);
        // printf("\n");
    }


    timeEnd = MPI_Wtime();

    // barrier = MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce(&timeStart,&minTime,1,MPI_DOUBLE,MPI_MIN,0,MPI_COMM_WORLD);
    MPI_Reduce(&timeEnd,&maxTime,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

    if(rank == 0){
        timeTaken = maxTime-minTime;
        // printf("Total Sum = %d\n", tmpSum);
        // printf("Total Time Taken = ");
        printf("%f\n", timeTaken);
    }

    MPI_Finalize();
    return 0;
}