St=512
Fin=1073741824

for (( fileNum=5; fileNum<=5; fileNum=fileNum+1 ))
do
	for (( p=1; p<=1024; p=p*2 ))
	do
		for (( i=$St; i<=$Fin; i=i*2 ))
		do  
		    if [ $i -ge 1073741824 ]
		    then
		    	bash runScript.sh $i $p $fileNum iter_${i}_${p}_${fileNum}.txt 2
		    elif [ $i -ge 268435456 ]
		    then
		    	bash runScript.sh $i $p $fileNum iter_${i}_${p}_${fileNum}.txt 4
		    else
		    	bash runScript.sh $i $p $fileNum iter_${i}_${p}_${fileNum}.txt 8
		    fi
		done
	done
done



# cd ..
# git add .
# git commit -m "Asgn_9 all files added!"
# git push
