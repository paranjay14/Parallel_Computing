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

void multiplyNaive(int **arrayA, int **arrayB, int **arrayC){
	for (int i = 0; i < N; ++i)
	{	
		for (int j = 0; j < N; ++j)
		{
			int temp=0;
			for (int k = 0; k < N; ++k)
			{
				temp += arrayA[i][k] * arrayB[k][j];
			}
			arrayC[i][j] = temp;
		}
	}

	return;
}

int main(int argc,char* argv[]){
	N = atoi(argv[1]);
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

	// Get starting timepoint 
    auto start = high_resolution_clock::now();

	multiplyNaive(arrayA, arrayB, arrayC);

	// Get ending timepoint 
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start); 
    cout <<fixed << duration.count() / power << endl;
    // cout << "Time taken by function - multiplyNaive : "<< duration.count() / power << " seconds" << endl;

	return 0;
}