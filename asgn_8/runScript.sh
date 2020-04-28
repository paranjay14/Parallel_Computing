#!/bin/bash
echo "Value of Prod : " $1
echo "Value of Cons : " $2
echo "Value of FULLSIZE : " $3
echo "Value of TOTAL_OPS : " $4
echo "File : " $5
Iter=$6
# Power2=$((2**$1))
# echo $Power2

g++ conc_queue.cpp prod_cons.cpp -lpthread -latomic -o ProdCons

s0=0
# s1=0

> $5

for (( i=1; i<=$Iter; i++ ))
do  
    echo "Iteration No. : " $i

    t0=`./ProdCons $1 $2 $3 $4`
    echo $t0 >> $5
    s0=`echo $s0 + $t0 | bc -l`

    # t1=`./pmk $1 $2`
    # echo $t1 >> $5
    # s1=`echo $s1 + $t1 | bc -l`
    
done

echo " " >> $5
echo "Mean Values :- " >> $5
s0=`echo $s0 / $Iter | bc -l  | grep -o '.*[1-9]'`
# s1=`echo $s1 / $Iter | bc -l  | grep -o '.*[1-9]'`

echo $s0 >> $5
# echo $s1 >> $5