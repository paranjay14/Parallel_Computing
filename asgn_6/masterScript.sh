St=5
Fin=11

for (( p=1; p<=16; p=p*2 ))
do
	for (( i=$St; i<=$Fin; i=i+1 ))
	do  
	    Size=$((10**$i))
	    # echo "Size : " $Size
	    if [ $i -ge 8 ]
	    then
	    	bash runScript.sh $Size $p iter_${i}_${p}.txt 4
	    else
	    	bash runScript.sh $Size $p iter_${i}_${p}.txt 8
	    fi
	done
done
