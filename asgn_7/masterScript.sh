St=1024
Fin=131072

for (( p=1; p<=8; p=p*2 ))
do
	for (( i=$St; i<=$Fin; i=i*2 ))
	do  
	    if [ $i -ge 2048 ]
	    then
	    	bash runScript.sh $i $p iter_${i}_${p}.txt 1
	    else
	    	bash runScript.sh $i $p iter_${i}_${p}.txt 1
	    fi
	done
done
