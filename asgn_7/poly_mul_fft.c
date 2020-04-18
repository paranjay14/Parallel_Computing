#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<complex.h>
#include<math.h>
#include<stdbool.h>
#include<omp.h>

#define cd complex double
const double PI = acos(-1);
int N,X,P,M,lg_N,twiceP,NbyTwiceP,NbyM,Mby2, *reverse;
cd *A, *array_w;
bool invertFlg;

void *PolyMulFFT(void *thread_arg)
{	
	int *my_data = (int *)thread_arg ;
	int t_id = *my_data;
    int indx1, indx2, iter, x;
    int shift = M*(t_id/Mby2) + (t_id%Mby2);
    cd w, t, u;

    indx1 = shift ;
    indx2 = indx1 + Mby2;

    if(twiceP > Mby2){
	    for (iter = 0; iter < NbyTwiceP; ++iter){
	    	if(invertFlg)
	    		w = array_w[N - ((indx1%Mby2)*NbyM)];
	    	else
	    		w = array_w[(indx1%Mby2)*NbyM];
	 
	        u = A[indx1];
	        t = w*A[indx2];
	        A[indx1] = u + t;
	        A[indx2] = u - t;
	    
	        if(invertFlg){
	            A[indx1]/=2.0;
	            A[indx2]/=2.0;
	        }
	            
	    	indx1+=twiceP;
	    	indx2+=twiceP;
	    }
	}
	else{
		shift = 0;
		x = M/twiceP;
		for (shift = 0; shift < N; shift+=M){
			indx1=t_id+shift;
			indx2=indx1+Mby2;
			for (iter = 0; iter < x; ++iter)
			{
				if(invertFlg){
		    		w = array_w[N - ((indx1%Mby2)*NbyM)];
		    	}
		    	else{
		    		w = array_w[(indx1%Mby2)*NbyM];
		    	}
		 
		        u = A[indx1];
		        t = w*A[indx2];
		        A[indx1] = u + t;
		        A[indx2] = u - t;
		    
		        if(invertFlg){
		            A[indx1]/=2.0;
		            A[indx2]/=2.0;
		        }

				indx1+=P;
				indx2+=P;
			}
		}
	}

	return NULL;
}


cd* FFT(cd* array, bool invert){
	A = (cd*)malloc(sizeof(cd)*N);
	for (int i = 0; i < N; ++i){
		A[i] = array[reverse[i]];
	}

	pthread_t thread[P];
	int thread_arg_array[P], rc;
	M=1;
	Mby2=1; 
	NbyM=N;
	invertFlg = invert;

	for (int lvl = 1; lvl<=lg_N; ++lvl){
		M<<=1;
		NbyM>>=1;

		for(int t=0; t<P; t++){
			thread_arg_array[t] = t;
			rc = pthread_create(&thread[t], NULL, PolyMulFFT, (void *)&thread_arg_array[t]);  
			if(rc) exit(-1);
		}

		for(int t=0; t<P; t++){
			rc = pthread_join(thread[t],NULL);
			if(rc) exit(-1);
		}

		Mby2=M;
	}

	return A;
}


int main (int argc, char *argv[])
{
	P = atoi(argv[1]);
	twiceP=2*P;
	int halfN = atoi(argv[2]);
	N = 2*halfN;
	X = N/P;
	NbyTwiceP= N/twiceP;

	int rc;
	cd *arrayA, *arrayB, *arrayC, *dft_A, *dft_B, *dft_AB;
	arrayA = (cd*)malloc(sizeof(cd)*N);
	arrayB = (cd*)malloc(sizeof(cd)*N);
	dft_AB = (cd*)malloc(sizeof(cd)*N);

	for (int i = 0; i < halfN; ++i){
		arrayA[i] = 1 ;
		arrayB[i] = 2 ;
	}
	for (int i = halfN; i < N; ++i){
		arrayA[i] = 0 ;
		arrayB[i] = 0 ;
	}

	lg_N = log2(N);
	int bit;

	reverse = (int*)malloc(sizeof(int)*N);
	reverse[0]=0;
	for (int i = 1, j=0; i < N; ++i)
	{
		bit = N >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        reverse[i]=j;
	}

	double ang = (2.0 * PI) / (double)(N) ;
	array_w = (cd*)malloc(sizeof(cd)*(N+1));
	array_w[0] = 1;
	array_w[N] = 1;
	array_w[1] = cos(ang) + (sin(ang))*I;
	for (int i = 2; i < N; ++i)
		array_w[i] = array_w[1]*array_w[i-1];

	double start, end; 
	start = omp_get_wtime();

	dft_A = FFT(arrayA,0);
	dft_B = FFT(arrayB,0);

	for (int i = 0; i < N; ++i)
		dft_AB[i] = dft_A[i]*dft_B[i];

	arrayC = FFT(dft_AB,1);
	end = omp_get_wtime(); 

	// for (int i = 0; i < N-1; ++i){
	// 	printf("%0.f ", round(creal(arrayC[i])));
	// }
	// printf("\n");

	printf("%f\n", end - start);
	pthread_exit(NULL);
}

