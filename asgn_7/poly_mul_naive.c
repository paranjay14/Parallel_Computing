#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<omp.h>

int *A, *B, *C, N, X, NUM_THREADS;

void *PolyMul(void *thread_id_ptr)
{
	int* tid = (int*)thread_id_ptr;
	int iter, i, indx, thread_id, start, lastIter, result, islastThread=0;

	/* indx represents the indices of the C array to be updated by the current thread  */
	indx = *tid;
	thread_id = *tid;

	/* First half of the iterations */
	for (iter = 0; iter < X; ++iter){
		result=0;
		for (i = 0; i <= indx; ++i){
			result += (A[i]*B[indx-i]) ;
		}
		C[indx] = result;
		indx+=NUM_THREADS;
	}

	/* reducing the count of iterations by 1 only for the last indexed thread id */
	if(thread_id==NUM_THREADS-1) islastThread = 1;
	lastIter = 2*X-islastThread;

	/* Second half of the iterations */
	for (iter = X; iter < lastIter; ++iter){
		result=0;
		start=indx-N+1;
		for (i = start; i < N; ++i){
			result += (int)(A[i]*B[indx-i]) ;
		}
		C[indx] = result;
		indx+=NUM_THREADS;
	}
	pthread_exit(NULL);
}


int main (int argc, char *argv[])
{
	N = atoi(argv[2]);
	NUM_THREADS = atoi(argv[1]);
	X = N/NUM_THREADS;
	pthread_t thread[NUM_THREADS];
	pthread_attr_t attr;
	int t,rc;
	int *thread_id = (int*)malloc(sizeof(int)*NUM_THREADS);
	A = (int*)malloc(sizeof(int)*N);
	B = (int*)malloc(sizeof(int)*N);
	C = (int*)malloc(sizeof(int)*(2*N-1));

	for (int i = 0; i < N; ++i){
		A[i] = 1;
		B[i] = 2;
	}
	for (int i = 0; i < NUM_THREADS; ++i){
		thread_id[i] = i;
	}

	double start; 
	double end; 
	start = omp_get_wtime(); 

	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for(t=0; t<NUM_THREADS; t++){
		rc = (int)pthread_create(&thread[t], NULL, PolyMul, (void *)&thread_id[t]);  
		if(rc){
			printf("Error while creating thread id:%d\n", t);
			exit(-1);
		}
	}

	/* Joining all the created threads */
	for(t=0; t<NUM_THREADS; t++){
		rc = pthread_join(thread[t],NULL);
		if(rc){
			printf("Error while joining thread id:%d\n", t);
			exit(-1);
		}
	}

	/* Free attribute and wait for the other threads */
	pthread_attr_destroy(&attr);

	// for (int i = 0; i < 2*N-1; ++i){
	// 	printf("%d ", C[i]);
	// }
	// printf("\n");
	
	end = omp_get_wtime(); 
	printf("%f\n", end - start);
	pthread_exit(NULL);
}