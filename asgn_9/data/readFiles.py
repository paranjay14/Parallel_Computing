import math

Strt_N=512
Fin_N=1073741824
range_N=int(math.log2(Fin_N*2) - math.log2(Strt_N) + 1)

Strt_P=1
Fin_P=1024
range_P=int(math.log2(Fin_P) - math.log2(Strt_P) + 1)

Strt_FileNum=5
Fin_FileNum=5
range_FileNum=Fin_FileNum - Strt_FileNum + 1


FileNum=Strt_FileNum
for i in range(range_FileNum):
    print(FileNum)
    N=Strt_N
    for j in range(range_N):
        P=Strt_P
        print(int(N/2),end=" ")
        for k in range(range_P):
            if N==Strt_N:
                print(P,end=" ")
            else:
                with open("iter_"+str(int(N/2))+"_"+str(P)+"_"+str(FileNum)+".txt") as fp: 
                    Lines = fp.readlines() 
                    line = Lines[len(Lines)-1]
                    line = line.strip()
                    print(line,end=" ")
            P=P*2
        N=N*2
        print()
    FileNum=FileNum+1
    print()
    print()