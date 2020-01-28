#include<iostream>
#include <algorithm> 
#include <chrono> 
#include <cmath>
#include<time.h>

using namespace std;
using namespace std::chrono;

int N = 512;
int M = 32768; //  M/B = 512, sqrt(M) = 181
int B = 64;
int S=32; // 64 <= sqrt(M)=90; <= N

void multiplyOPT2_0(int **arrayA, int **arrayB, int **arrayC){
	for(int i1=0; i1<N; i1+=S){
		for(int j1=0; j1<N; j1+=S){
			for(int k1=0; k1<N; k1+=S){
				for(int i = i1; i < i1+S; ++i){
					for(int j = j1; j < j1+S; ++j){
						int temp = 0;
						for(int k = k1; k < k1+S; ++k){
							temp += arrayA[i][k] * arrayB[k][j];
						}
						arrayC[i][j] = temp;
					}
				}
			}
		}
	}

	return;
}

int main(int argc,char* argv[]){
	N = atoi(argv[1]);
	S = atoi(argv[2]);
	int **arrayA = new int*[N], **arrayB = new int*[N], **arrayC = new int*[N];
	double power = pow(10,6);

	for (int i = 0; i < N; ++i)
	{	
		arrayA[i] = new int[N];
		arrayB[i] = new int[N];
		arrayC[i] = new int[N];

		for (int j = 0; j < N; ++j)
		{
			arrayA[i][j] = 1;
			arrayB[i][j] = 1;
			arrayC[i][j] = 0;
		}
	}

    auto start = high_resolution_clock::now();
	multiplyOPT2_0(arrayA, arrayB, arrayC);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start); 
    cout <<fixed << duration.count() / power << endl;
    // cout << "Time taken by function - multiplyOPT2_0 : "<< duration.count() / power << " seconds" << endl;

	return 0;
}