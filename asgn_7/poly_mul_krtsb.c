#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<omp.h>

#define ll long long
int *arrayA, *arrayB, *arrayC;
int N, X, NUM_THREADS;

typedef struct thread_data{
	int *A,*B,**C; 
   	int lA,rA,lB,rB;
}thread_args;

/*
void *PolyMulNaive(void *t)
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
*/

void *PolyMulKaratsuba(void *thread_arg)
{	
	thread_args *my_data = (thread_args *)thread_arg ;

	// printf("Thread %ld starting...\n",tid);
	int lA,lB,rA,rB,midA,midB;
	lA=my_data->lA;
	rA=my_data->rA;
	lB=my_data->lB;
	rB=my_data->rB;
	midA=(lA+rA)/2;
	midA=(lB+rB)/2;
	int *partialA=my_data->A, *partialB=my_data->B, **partialC = my_data->C;

	// printf("%d %d %d %d\n",lA,rA,lB,rB);

	// stores the final result for the current recursion
	int *tmpPartialC = (int*)malloc(sizeof(int)*(rA-lA+rB-lB+1));

	////////////////// base case ///////////
	if(rA-lA == 0){
		tmpPartialC[0] = partialA[lA]*partialB[lB];
		*partialC = tmpPartialC;

		// printf("Thread done %d\n",lA);
		pthread_exit(NULL);
		return NULL;
	}



	// stores temp result of 3 multiplications
	int *array_LL, *array_HH, *array_LH;	
	thread_args t_argsLL = (thread_args){partialA,partialB,&array_LL,lA,midA,lB,midB};
	thread_args t_argsHH = (thread_args){partialA,partialB,&array_HH,midA+1,rA,midB+1,rB};

	int rc1,rc2;
	pthread_t tid1,tid2,tid3;
	rc1 = pthread_create(&tid1, NULL, PolyMulKaratsuba, (void *)&t_argsLL);  
	rc2 = pthread_create(&tid2, NULL, PolyMulKaratsuba, (void *)&t_argsHH);

	int *halfsumA, *halfsumB; 
	halfsumA = (int*)malloc(sizeof(int)*(midA-lA+1));
	halfsumB = (int*)malloc(sizeof(int)*(midB-lB+1));

	for (int i = 0; i < midA-lA+1; ++i)
		halfsumA[i] = partialA[lA+i]+partialA[midA+i+1];

	for (int i = 0; i < midB-lB+1; ++i)
		halfsumB[i] = partialB[lB+i]+partialB[midB+i+1];

	// making the current thread too work and hence execute the 3rd multiplication
	thread_args t_argsLH = (thread_args){halfsumA,halfsumB,&array_LH,0,midA-lA,0,midB-lB};
	// long rc3 = (long)PolyMulKaratsuba((void*)&t_argsLH);
	long rc3 = pthread_create(&tid3, NULL, PolyMulKaratsuba, (void *)&t_argsLH);

	free(halfsumA);
	free(halfsumB);

	rc1 = pthread_join(tid1, NULL);
	rc2 = pthread_join(tid2, NULL);
	rc3 = pthread_join(tid3, NULL);

	if (rc1 || rc2 || rc3) {
		// printf("ERROR; return code from pthread_join() is %d\n", rc1||rc2);
		exit(-1);
	}
	
	int indx = midA-lA+midB-lB+1;
	tmpPartialC[indx]=0;
	for (int i = 0; i < indx; ++i)
		array_LH[i] -= (array_HH[i]+array_LL[i]);

	for (int i = 0; i < indx; ++i)
		tmpPartialC[i] = array_LL[i];

	for (int i = indx+1; i < rA-lA+rB-lB+1; ++i)
		tmpPartialC[i] = array_HH[i-indx-1];

	for (int i = midA-lA+1; i < indx+midA-lA+1; ++i)
		tmpPartialC[i] += array_LH[i-midA+lA-1];

	*partialC = tmpPartialC;

	free(array_LH);
	free(array_HH);
	free(array_LL);
	// printf("Thread %ld done\n",tid);
	pthread_exit(NULL);
}


int main (int argc, char *argv[])
{
	N = atoi(argv[2]);
	NUM_THREADS = atoi(argv[1]);
	NUM_THREADS = 1;
	X = N/NUM_THREADS;
	pthread_t thread[NUM_THREADS];
	thread_args thread_data_array[NUM_THREADS];
	pthread_attr_t attr;
	int t;
	int rc;
	void *status;
	arrayA = (int*)malloc(sizeof(int)*N);
	arrayB = (int*)malloc(sizeof(int)*N);
	arrayC = (int*)malloc(sizeof(int)*2*(N-1));

	for (int i = 0; i < N; ++i){
		arrayA[i] = 1;
		arrayB[i] = 2;
	}

	double start; 
	double end; 
	start = omp_get_wtime();

	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for(t=0; t<NUM_THREADS; t++){
		thread_data_array[t] = (thread_args){arrayA,arrayB,&arrayC,0,N-1,0,N-1};
		// printf("Main: creating thread %ld\n", t);
		rc = pthread_create(&thread[t], &attr, PolyMulKaratsuba, (void *)&thread_data_array[t]);  
		if (rc){
			// printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	/* Free attribute and wait for the other threads */
	pthread_attr_destroy(&attr);
	for(t=0; t<NUM_THREADS; t++){
		rc = pthread_join(thread[t], &status);
		if (rc) {
			// printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
		// printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
	}

	// for (int i = 0; i < 2*N-1; ++i){
	// 	printf("%d ", arrayC[i]);
	// }
	// printf("\n");

	// printf("Main: program completed. Exiting.\n");
	end = omp_get_wtime(); 
	printf("%f\n", end - start);
	pthread_exit(NULL);
}