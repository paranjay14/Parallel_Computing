St=32
Fin=8192

for (( p=1; p<=16; p=p*2 ))
do
	for (( i=$St; i<=$Fin; i=i*2 ))
	do  
	    if [ $i -ge 2048 ]
	    then
	    	bash runScript.sh $i $p iter_${i}_${p}.txt 2
	    else
	    	bash runScript.sh $i $p iter_${i}_${p}.txt 8
	    fi
	done
done
