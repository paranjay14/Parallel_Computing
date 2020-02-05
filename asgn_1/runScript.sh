#!/bin/bash
echo "Value of N : " $1
echo "Value of S : " $2
echo "File : " $3
echo "Opt : -" $4
Iter=$5

# g++ -o naive naive.cpp 
# g++ -o opt1 opt1.cpp 
# g++ -o opt2_0 opt2_0.cpp 
# g++ -o opt2_1 opt2_1.cpp 
# g++ -o opt2_2 opt2_2.cpp 
# g++ -o opt2_3 opt2_3.cpp 
# g++ -o opt3 opt3.cpp

g++ -$4 naive.cpp -o naive_$4
g++ -$4 opt1.cpp -o opt1_$4
g++ -$4 opt2_3.cpp -o opt2_3_$4
g++ -$4 opt3.cpp -o opt3_$4


s0=0
s1=0
s2_0=0
s2_1=0
s2_2=0
s2_3=0
s3=0
> $3

for (( i=1; i<=$Iter; i++ ))
do  
    echo "Iteration No. : " $i

    t0=`./naive_$4 $1`
    echo $t0 >> $3
    s0=`echo $s0 + $t0 | bc -l`

    t1=`./opt1_$4 $1`
    echo $t1 >> $3
    s1=`echo $s1 + $t1 | bc -l`

    # t2_0=`./opt2_0 $1 $2`
    # echo $t2_0 >> $3
    # s2_0=`echo $s2_0 + $t2_0 | bc -l`

    # t2_1=`./opt2_1 $1 $2`
    # echo $t2_1 >> $3
    # s2_1=`echo $s2_1 + $t2_1 | bc -l`

    # t2_2=`./opt2_2 $1 $2`
    # echo $t2_2 >> $3
    # s2_2=`echo $s2_2 + $t2_2 | bc -l`

    t2_3=`./opt2_3_$4 $1 $2`
    echo $t2_3 >> $3
    s2_3=`echo $s2_3 + $t2_3 | bc -l`

    t3=`./opt3_$4 $1`
    echo $t3 >> $3
    s3=`echo $s3 + $t3 | bc -l`
done

echo "" >> $3
s0=`echo $s0 / $Iter | bc -l  | grep -o '.*[1-9]'`
s1=`echo $s1 / $Iter | bc -l  | grep -o '.*[1-9]'`
# s2_0=`echo $s2_0 / $Iter | bc -l  | grep -o '.*[1-9]'`
# s2_1=`echo $s2_1 / $Iter | bc -l  | grep -o '.*[1-9]'`
# s2_2=`echo $s2_2 / $Iter | bc -l  | grep -o '.*[1-9]'`
s2_3=`echo $s2_3 / $Iter | bc -l  | grep -o '.*[1-9]'`
s3=`echo $s3 / $Iter | bc -l  | grep -o '.*[1-9]'`

echo $s0 >> $3
echo $s1 >> $3
# echo $s2_0 >> $3
# echo $s2_1 >> $3
# echo $s2_2 >> $3
echo $s2_3 >> $3
echo $s3 >> $3
