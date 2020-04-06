#!/bin/bash
echo "Value of N : " $1
echo "Value of P : " $2
echo "File : " $3
Iter=$4

gcc -fopenmp pi_critical.c -o ompC
gcc -fopenmp pi_reduce.c -o ompR

s0=0
s1=0

> $3

for (( i=1; i<=$Iter; i++ ))
do  
    echo "Iteration No. : " $i

    t0=`./ompC $1 $2`
    echo $t0 >> $3
    s0=`echo $s0 + $t0 | bc -l`

    t1=`./ompR $1 $2`
    echo $t1 >> $3
    s1=`echo $s1 + $t1 | bc -l`

done

echo " " >> $3
echo "Mean Values :- " >> $3
s0=`echo $s0 / $Iter | bc -l  | grep -o '.*[1-9]'`
s1=`echo $s1 / $Iter | bc -l  | grep -o '.*[1-9]'`

echo $s0 >> $3
echo $s1 >> $3