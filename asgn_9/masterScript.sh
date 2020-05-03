St=8
Fin=1073741824

for (( fileNum=1; fileNum<=4; fileNum=fileNum+1 ))
do
	for (( p=1; p<=16; p=p*2 ))
	do
		for (( i=$St; i<=$Fin; i=i*2 ))
		do  
		    if [ $i -ge 1073741824 ]
		    then
		    	bash runScript.sh $p $p $sz $i iter_${p}_${p}_${sz}_${i}.txt 2
		    elif [ $i -ge 268435456 ]
		    then
		    	bash runScript.sh $p $p $sz $i iter_${p}_${p}_${sz}_${i}.txt 4
		    else
		    	bash runScript.sh $p $p $sz $i iter_${p}_${p}_${sz}_${i}.txt 8
		    fi
		done
	done
done



# cd ..
# git add .
# git commit -m "Asgn_8 all files added!"
# git push
