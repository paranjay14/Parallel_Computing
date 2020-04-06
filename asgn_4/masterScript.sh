Fin=16384

for (( p=64; p<=64; p=p*4 ))
do
	for (( i=2; i<=$Fin; i=i*2 ))
	do  
	    echo "Size : " $i
	    if [ $i -ge 16384 ]
	    then
	    	bash runScript.sh $i $p iter_${i}_${p}.txt 4
	    else
	    	bash runScript.sh $i $p iter_${i}_${p}.txt 10
	    fi
	done
done

# bash runScript.sh 536870912 4 iter2.txt 6