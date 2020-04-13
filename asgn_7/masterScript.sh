St=256
Fin=524288

for (( p=0; p<=5; p=p+1 ))
do
	for (( i=$St; i<=$Fin; i=i*2 ))
	do  
	    if [ $i -ge 65536 ]
	    then
	    	bash runScript.sh $p $i iter_${i}_${p}.txt 2
	    else
	    	bash runScript.sh $p $i iter_${i}_${p}.txt 8
	    fi
	done
done
