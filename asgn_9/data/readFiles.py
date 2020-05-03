Prod=1 #16
Cons=1 #16
FullSize=64 #16384
Ops=128 #8388608

Cons=1
for i in range(5):
    # print(Prod)
    Prod=Cons
    FullSize=16
    for j in range(6):
        # print(FullSize)
        Ops=128
        print(FullSize,end=" ")
        for k in range(5):
            if FullSize==16:
                print(Ops,end=" ")
            else:
                with open("iterC_"+str(Prod)+"_"+str(Cons)+"_"+str(FullSize)+"_"+str(Ops)+".txt") as fp: 
                    Lines = fp.readlines() 
                    line = Lines[len(Lines)-1]
                    line = line.strip()
                    print(line,end=" ")
            Ops=Ops*16
        FullSize=FullSize*4
        print()
    Cons=Cons*2
    print()
