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

void multiplyOPT3(int Arow1, int Arow2, int Acol1, int Acol2, int **arrayA, 
					int Brow1, int Brow2, int Bcol1, int Bcol2, int **arrayB, int **arrayC) 
{ 
	// cout<<Arow1<<", "<<Arow2<<", "<<Acol1<<", "<<Acol2<<", "<<Brow1<<", "<<Brow2<<", "<<Bcol1<<", "<<Bcol2<<endl;
	if(Arow1 == Arow2){
		arrayC[Arow1][Bcol2] += arrayA[Arow1][Acol1] * arrayB[Brow1][Bcol1];
	}
	else{
		int a1 = (Arow1+Arow2) >> 1 ;
		int a2 = (Acol1+Acol2) >> 1 ;
		int b1 = (Brow1+Brow2) >> 1 ;
		int b2 = (Bcol1+Bcol2) >> 1 ;

		multiplyOPT3(Arow1, a1, Acol1, a2, arrayA, Brow1, b1, Bcol1, b2, arrayB, arrayC); // A11,B11
		multiplyOPT3(Arow1, a1, a2 + 1, Acol2, arrayA, b1 + 1, Brow2, Bcol1, b2, arrayB, arrayC);  // A12,B21
		multiplyOPT3(Arow1, a1, Acol1, a2, arrayA, Brow1, b1, b2 + 1, Bcol2, arrayB, arrayC); // A11,B12
		multiplyOPT3(Arow1, a1, a2 + 1, Acol2, arrayA, b1 + 1, Brow2, b2 + 1, Bcol2, arrayB, arrayC);  // A12,B22

		multiplyOPT3(a1 + 1, Arow2, Acol1, a2, arrayA, Brow1, b1, Bcol1, b2, arrayB, arrayC); // A21,B11
		multiplyOPT3(a1 + 1, Arow2, a2 + 1, Acol2, arrayA, b1 + 1, Brow2, Bcol1, b2, arrayB, arrayC);  // A22,B21
		multiplyOPT3(a1 + 1, Arow2, Acol1, a2, arrayA, Brow1, b1, b2 + 1, Bcol2, arrayB, arrayC); // A21,B12
		multiplyOPT3(a1 + 1, Arow2, a2 + 1, Acol2, arrayA, b1 + 1, Brow2, b2 + 1, Bcol2, arrayB, arrayC);  // A22,B22
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

    auto start = high_resolution_clock::now();
	multiplyOPT3(0, N-1, 0, N-1, arrayA, 0, N-1, 0, N-1, arrayB, arrayC);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start); 
    cout <<fixed << duration.count() / power << endl;
    // cout << "Time taken by function - multiplyOPT3 : "<< duration.count() / power << " seconds" << endl;

	return 0;
}