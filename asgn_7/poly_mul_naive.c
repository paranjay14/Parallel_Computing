#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<omp.h>

#define ll long long
int *A, *B;
ll *C, N, X, NUM_THREADS;

void *PolyMul(void *t)
{
	long tid = (long)t;
	ll iter, i, indx, start, lastIter, result, islastThread=0;

	indx = tid;
	for (iter = 0; iter < X; ++iter){
		result=0;
		for (i = 0; i <= indx; ++i){
			result += (ll)(A[i]*B[indx-i]) ;
		}
		C[indx] = result;
		indx+=NUM_THREADS;
	}

	if(tid==NUM_THREADS-1) islastThread = 1;
	lastIter = 2*X-islastThread;

	for (iter = X; iter < lastIter; ++iter){
		result=0;
		start=indx-N+1;
		for (i = start; i < N; ++i){
			result += (ll)(A[i]*B[N-1-i+start]) ;
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
	long t, i;
	int rc;
	A = (int*)malloc(sizeof(int)*N);
	B = (int*)malloc(sizeof(int)*N);
	C = (ll*)malloc(sizeof(ll)*2*(N-1));

	for (i = 0; i < N; ++i){
		A[i] = 1;
		B[i] = 2;
	}

	double start; 
	double end; 
	start = omp_get_wtime(); 

	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for(t=0; t<NUM_THREADS; t++){
		rc = pthread_create(&thread[t], &attr, PolyMul, (void *)t);  
		if (rc){
			exit(-1);
		}
	}

	/* Free attribute and wait for the other threads */
	pthread_attr_destroy(&attr);
	for(t=0; t<NUM_THREADS; t++){
		rc = pthread_join(thread[t],NULL);
		if (rc) {
			exit(-1);
		}
	}

	// for (i = 0; i < 2*N-1; ++i){
	// 	printf("%lld ", C[i]);
	// }
	// printf("\n");
	end = omp_get_wtime(); 
	printf("%f\n", end - start);
	pthread_exit(NULL);
}