#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<omp.h>

int MAXLEVEL;
// int totalThreads=1;

typedef struct thread_data{
	int *A,*B,**C; 
   	int lA,rA,lB,rB;
   	int lvl;
} thread_args;

void *PolyMulKaratsuba(void *thread_arg)
{	
	thread_args *my_data = (thread_args *)thread_arg ;

	int lA,lB,rA,rB,midA,midB,lvl;
	lA=my_data->lA;
	rA=my_data->rA;
	lB=my_data->lB;
	rB=my_data->rB;
	lvl=my_data->lvl;
	midA=(lA+rA)/2;
	midB=(lB+rB)/2;
	int *partialA=my_data->A, *partialB=my_data->B, **partialC = my_data->C;
	int tmpHalf=midA-lA;
	int tmpFull=2*(rA-lA), tmpHalfx2=2*tmpHalf;


	// stores the final result for the current recursion
	int *tmpPartialC = (int*)malloc(sizeof(int)*(tmpFull+1));

	/************ Base Case ************/
	if(tmpFull == 0){
		tmpPartialC[0] = partialA[lA]*partialB[lB];
		*partialC = tmpPartialC;
		return NULL;
	}

	/* stores temp result of 3 muliplications */
	int *array_LL, *array_HH, *array_LH;	

	/* creating valid args for first 2 muliplications */
	thread_args t_argsLL = (thread_args){partialA,partialB,&array_LL,lA,midA,lB,midB,lvl+1};
	thread_args t_argsHH = (thread_args){partialA,partialB,&array_HH,midA+1,rA,midB+1,rB,lvl+1};

	int rc1=0,rc2=0;
	pthread_t tid1,tid2,tid3;

	/*	IF current level is less than MAXLEVEL, then create new threads and parallelize, 
		ELSE execute serially */
	if(lvl<=MAXLEVEL){
		rc1 = pthread_create(&tid1, NULL, PolyMulKaratsuba, (void *)&t_argsLL);  
		rc2 = pthread_create(&tid2, NULL, PolyMulKaratsuba, (void *)&t_argsHH);
		// totalThreads+=2;
	}
	else{
		rc1 = (long)PolyMulKaratsuba((void *)&t_argsLL);  
		rc2 = (long)PolyMulKaratsuba((void *)&t_argsHH);
	}

	/* creating temp arrays used for 3rd multiplication */
	int *halfsumA, *halfsumB; 
	halfsumA = (int*)malloc(sizeof(int)*(tmpHalf+1));
	halfsumB = (int*)malloc(sizeof(int)*(tmpHalf+1));

	for (int i = 0; i < tmpHalf+1; ++i)
		halfsumA[i] = partialA[lA+i]+partialA[midA+i+1];

	for (int i = 0; i < tmpHalf+1; ++i)
		halfsumB[i] = partialB[lB+i]+partialB[midB+i+1];

	/*	making the current thread work too and hence execute the 3rd multiplication
		Also, creating valid args for the 3rd muliplication... 							*/
	thread_args t_argsLH = (thread_args){halfsumA,halfsumB,&array_LH,0,tmpHalf,0,tmpHalf,lvl+1};
	long rc3 = (long)PolyMulKaratsuba((void*)&t_argsLH);
	
	/* if new threads were created, then execute join for them */
	if(lvl<=MAXLEVEL){
		rc1 = pthread_join(tid1, NULL);
		rc2 = pthread_join(tid2, NULL);
	}

	/* freeing the temporarily created array resources */
	free(halfsumA);
	free(halfsumB);

	if(rc1 || rc2) exit(-1);
	
	/* combining the resultant 3 arrays to form the result for the current recursion */
	tmpPartialC[tmpHalfx2+1]=0;
	for (int i = 0; i < tmpHalfx2+1; ++i)
		array_LH[i] -= (array_HH[i]+array_LL[i]);

	for (int i = 0; i < tmpHalfx2+1; ++i)
		tmpPartialC[i] = array_LL[i];

	for (int i = tmpHalfx2+2; i < tmpFull+1; ++i)
		tmpPartialC[i] = array_HH[i-tmpHalfx2-2];

	for (int i = tmpHalf+1; i < tmpHalfx2+tmpHalf+2; ++i)
		tmpPartialC[i] += array_LH[i-tmpHalf-1];

	/* Making the final solution point to the final resultant array */
	*partialC = tmpPartialC;

	/* freeing the temporarily created resultant array resources */
	free(array_LH);
	free(array_HH);
	free(array_LL);

	return NULL;
}


int main (int argc, char *argv[])
{
	int N = atoi(argv[2]);
	int initNumThreads = 1;
	pthread_t thread[initNumThreads];
	thread_args thread_data_array[initNumThreads];
	pthread_attr_t attr;
	int rc;
	void *status;
	int *arrayA, *arrayB, *arrayC;
	arrayA = (int*)malloc(sizeof(int)*N);
	arrayB = (int*)malloc(sizeof(int)*N);
	arrayC = (int*)malloc(sizeof(int)*2*(N-1));
	MAXLEVEL = atoi(argv[1]);;

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

	for(int t=0; t<initNumThreads; t++){
		thread_data_array[t] = (thread_args){arrayA,arrayB,&arrayC,0,N-1,0,N-1,1};
		rc = pthread_create(&thread[t], &attr, PolyMulKaratsuba, (void *)&thread_data_array[t]);  
		if(rc) exit(-1);
	}

	/* Free attribute and wait for the other threads */
	pthread_attr_destroy(&attr);
	for(int t=0; t<initNumThreads; t++){
		rc = pthread_join(thread[t], &status);
		if(rc) exit(-1);
	}

	// for (int i = 0; i < 2*N-1; ++i){
	// 	printf("%d ", arrayC[i]);
	// }
	// printf("\n");

	// printf("Total Threads: %d\n", totalThreads);
	end = omp_get_wtime(); 
	printf("%f\n", end - start);
	pthread_exit(NULL);
}