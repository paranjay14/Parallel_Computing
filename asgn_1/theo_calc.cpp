#include <iostream>
using namespace std;

double func(double n){
	return (4*n*n*n - n*n)/(2.8e9);
}

int main(){

    for (int i = 32; i <= 8192; i*=2)
    {
        cout<<func(i)<<endl;
    }
    
	return 0;
}