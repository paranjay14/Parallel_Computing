#include<bits/stdc++.h>
using namespace std;

int main(int argc,char** argv){
    int n,alp;
    srand(time(0));
    n= atoi(argv[1]);
    alp= atoi(argv[2]);
    for(int i=0;i<n;i++){
        char c = 'a'+rand()%alp;
        cout<<c;
    }
    return 0;

}