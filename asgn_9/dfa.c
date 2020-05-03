#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <omp.h>

int N,P,Q,NbyP;
pthread_barrier_t barrier, *barrierIter;
char redn_flg, *inpString;
int **Trans, **Lmatrix, *start_pos_array, *end_pos_array;


void* runDFA(void* arg){
	int tid = (int)(long)(arg);
	int start_pos,end_pos, m, prev_m,iter=0,l;
	start_pos = start_pos_array[tid];
	end_pos = end_pos_array[tid];
	
	if(tid==0){
		for (int j = start_pos; j <= end_pos; ++j)
			Lmatrix[0][0] = Trans[Lmatrix[0][0]][inpString[j]-'a'];
	}
	else{
		for (int j = start_pos; j <= end_pos; ++j)
			for (int k = 0; k < Q; ++k)
				Lmatrix[tid][k] = Trans[Lmatrix[tid][k]][inpString[j]-'a'];
	}

	pthread_barrier_wait(&barrier);
	
	if(redn_flg=='1'){
		prev_m=1;
		for (m=2; m<=P; m<<=1){
			if((tid%m == 0) && (tid+prev_m<P)){
				for (int k = 0; k < Q; ++k)
					Lmatrix[tid][k] = Lmatrix[tid+prev_m][Lmatrix[tid][k]];
				pthread_barrier_wait(&barrierIter[iter++]);
			}
			else return NULL;
			prev_m=m;
		}
	}
	else if(tid==0){
		l=0;
		for (int k = 0; k < P; ++k){
			l = Lmatrix[k][l];
		}
		Lmatrix[0][0] = l;
	}
	
	return NULL;
}

void* StartDFARun(){
	pthread_t thread[P-1];
	int rc;
	for (long i = 0; i < P-1; ++i){
    	rc = (int)pthread_create(&thread[i], NULL, runDFA, (void*)(i+1)); 
	}
	
	runDFA((void*)(0));
	return NULL;
}


int main(int argc, char* argv[]){
	N=atoi(argv[1]);
	P=atoi(argv[2]);
	NbyP = N/P;
	int fileNum = atoi(argv[3]);
	int i,q1,q2,rc,lenTrans,F,inputSymbols,barCnt=P>>1;
    char c;
	FILE *fptr;
	pthread_barrier_init(&barrier, NULL, P);
	barrierIter = (pthread_barrier_t*)malloc(log2(P)*sizeof(pthread_barrier_t));
	for (i = 0; i < log2(P); ++i){
		pthread_barrier_init(&barrierIter[i], NULL, barCnt);
		barCnt>>=1;
	}
	start_pos_array = (int*)malloc(P*sizeof(int));
	end_pos_array = (int*)malloc(P*sizeof(int));
	start_pos_array[0]=0;
	end_pos_array[0]=NbyP-1;
	for (i = 1; i < P; ++i){
		start_pos_array[i] = start_pos_array[i-1]+NbyP;
		end_pos_array[i] = end_pos_array[i-1]+NbyP;
	}

	char fileName[]="dfa_input1.txt";
	fileName[9]=fileNum+'0';
	fptr = fopen(fileName, "r");
	if(fptr == NULL){ 
        printf("Cannot open file \n"); 
        exit(0); 
    }
    fscanf(fptr,"%d %d %d %d",&Q,&inputSymbols,&F,&lenTrans);
    Trans = (int**)malloc(Q*sizeof(int*));
    for (i = 0; i < Q; ++i)
    	Trans[i] = (int*)malloc(inputSymbols*sizeof(int));

    for (i = 0; i < lenTrans; ++i){
    	fscanf(fptr,"%d %c %d",&q1,&c,&q2);
    	Trans[q1][c-'a'] = q2;
    }
    fclose(fptr);

    srand(time(0));
	inpString = (char*)malloc(N*sizeof(char));;
	for(i=0;i<N;i++){
        inpString[i] = 'a'+(rand()%inputSymbols);
    }

	Lmatrix = (int**)malloc(P*sizeof(int*));
    for (i = 0; i < P; ++i){
    	Lmatrix[i] = (int*)malloc(Q*sizeof(int));
    		for (int j = 0; j < Q; ++j)
    			Lmatrix[i][j] = j;
    }

    if(P>Q*log2(P)) redn_flg='1';
    else redn_flg='0';

    double start, end;  
	start = omp_get_wtime();
	
	StartDFARun();
	
    end = omp_get_wtime();

    // if(Lmatrix[0][0]==F) printf("ACCEPTED\n");
    // else  printf("REJECTED\n");

    printf("%f\n", end - start);

	return 0;
}
