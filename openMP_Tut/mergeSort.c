#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<time.h>

void Merge(int* A, int l, int m, int r){
	int *lArr = (int*)malloc((m-l+1)*sizeof(int)), *rArr = (int*)malloc((r-m)*sizeof(int));
	int lcnt=0, rcnt=0;

	for (int i = 0; i < m-l+1; ++i)
	{
		lArr[i] = A[l+i];
	}

	for (int i = 0; i < r-m; ++i)
	{
		rArr[i] = A[m+1+i];
	}

	int indx=l;
	while(lcnt+rcnt<r-l+1){
		if(lArr[lcnt]<=rArr[rcnt]){
			A[indx++] = lArr[lcnt];
			lcnt++;
			if(lcnt==m-l+1)
				while(rcnt!=r-m) A[indx++] = rArr[rcnt++];	
		}
		else{
			A[indx++] = rArr[rcnt];
			rcnt++;
			if(rcnt==r-m)
				while(lcnt!=m-l+1) A[indx++] = lArr[lcnt++];
		}
	}

}

void MergeSort(int* A, int l, int r){
	if(l<r){
		int m = (l+r)/2;
		MergeSort(A,l,m);
		MergeSort(A,m+1,r);

		Merge(A,l,m,r);
	}
}

int main(int argc, char* argv[]){

	int num_threads, N_threads;
	num_threads = atoi(argv[1]);
	double start, stop;
	omp_set_num_threads(num_threads);

	int n=32;
	int *A = (int*)calloc(n,sizeof(int));
	for (int i = 0; i < n; ++i)
	{
		A[i] = n-i;
	}
	start = omp_get_wtime();
	#pragma omp parallel
	{	
		N_threads = omp_get_num_threads();
		int ID=omp_get_thread_num();
		printf("ID: %d\n", ID);

		MergeSort(A,0,n-1);

		// #pragma omp critical

	}
	stop = omp_get_wtime();
	for (int i = 0; i < n; ++i)
	{
		printf("%d ", A[i]);
	}


	return 0;
}
