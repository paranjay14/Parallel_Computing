#!/bin/bash
echo "Value of N : " $1
echo "Value of P : " $2
echo "Num of File : " $3
echo "File : " $4
Iter=$5
# Power2=$((2**$1))
# echo $Power2

gcc dfa.c -lpthread -fopenmp -lm -o dfa

s0=0
# s1=0

> $4

for (( i=1; i<=$Iter; i++ ))
do  
    echo "Iteration No. : " $i

    t0=`./dfa $1 $2 $3`
    echo $t0 >> $4
    s0=`echo $s0 + $t0 | bc -l`

    # t1=`./pmk $1 $2`
    # echo $t1 >> $4
    # s1=`echo $s1 + $t1 | bc -l`
    
done

echo " " >> $4
echo "Mean Values :- " >> $4
s0=`echo $s0 / $Iter | bc -l  | grep -o '.*[1-9]'`
# s1=`echo $s1 / $Iter | bc -l  | grep -o '.*[1-9]'`

echo $s0 >> $4
# echo $s1 >> $4