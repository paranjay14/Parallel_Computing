St=256
Fin=33554432
# Fin=8192

for (( p=1; p<=32; p=p*2 ))
do
	for (( i=$St; i<=$Fin; i=i*2 ))
	do  
	    if [ $i -ge 16777216 ]
	    then
	    	bash runScript.sh $p $i iterFFT_${i}_${p}.txt 1
	    elif [ $i -ge 524288 ]
	    then
	    	bash runScript.sh $p $i iterFFT_${i}_${p}.txt 2
	    else
	    	bash runScript.sh $p $i iterFFT_${i}_${p}.txt 8
	    fi
	done
done
