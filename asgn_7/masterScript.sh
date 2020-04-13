St=1024
Fin=2048

for (( p=1; p<=8; p=p*2 ))
do
	for (( i=$St; i<=$Fin; i=i*2 ))
	do  
	    if [ $i -ge 65536 ]
	    then
	    	bash runScript.sh $i $p iter_${i}_${p}.txt 1
	    else
	    	bash runScript.sh $i $p iter_${i}_${p}.txt 10
	    fi
	done
done
