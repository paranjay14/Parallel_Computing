Fin=268435456
pMax=16

for (( p=2; p<=$pMax; p=p*2 ))
do
	for (( i=2; i<=$Fin; i=i*2 ))
	do  
	    echo "Size : " $i
	    if [ $i -ge 8388608 ]
	    then
	    	bash runScript.sh $i $p oes_iter_${i}_${p}.txt 3
	    else
	    	bash runScript.sh $i $p oes_iter_${i}_${p}.txt 10
	    fi
	done
done

# bash runScript.sh 536870912 4 iter.txt 1