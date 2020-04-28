St=128
Fin=8388608
# Fin=8192

# for (( p=1; p<=16; p=p*2 ))
# do
# 	for (( sz=64; sz<=16384; sz=sz*4 ))
# 	do
# 		for (( i=$St; i<=$Fin; i=i*16 ))
# 		do  
# 		    if [ $i -ge 524288 ]
# 		    then
# 		    	bash runScript.sh $p $p $sz $i iter_${p}_${p}_${sz}_${i}.txt 1
# 		    # elif [ $i -ge 524288 ]
# 		    # then
# 		    # 	bash runScript.sh $p $p $sz $i iter_${p}_${p}_${sz}_${i}.txt 2
# 		    else
# 		    	bash runScript.sh $p $p $sz $i iter_${p}_${p}_${sz}_${i}.txt 8
# 		    fi
# 		done
# 	done
# done

constsz=4

for (( p=1; p<=16; p=p*2 ))
do
	for (( sz=64; sz<=16384; sz=sz*4 ))
	do
		for (( i=$St; i<=$Fin; i=i*16 ))
		do  
		    if [ $i -ge 524288 ]
		    then
		    	bash runScript.sh $p $constsz $sz $i iterP_${p}_${constsz}_${sz}_${i}.txt 1
		    # elif [ $i -ge 524288 ]
		    # then
		    # 	bash runScript.sh $p $p $sz $i iter_${p}_${p}_${sz}_${i}.txt 2
		    else
		    	bash runScript.sh $p $constsz $sz $i iterP_${p}_${constsz}_${sz}_${i}.txt 8
		    fi
		done
	done
done


for (( p=1; p<=16; p=p*2 ))
do
	for (( sz=64; sz<=16384; sz=sz*4 ))
	do
		for (( i=$St; i<=$Fin; i=i*16 ))
		do  
		    if [ $i -ge 524288 ]
		    then
		    	bash runScript.sh $constsz $p $sz $i iterC_${constsz}_${p}_${sz}_${i}.txt 1
		    # elif [ $i -ge 524288 ]
		    # then
		    # 	bash runScript.sh $p $p $sz $i iter_${p}_${p}_${sz}_${i}.txt 2
		    else
		    	bash runScript.sh $constsz $p $sz $i iterC_${constsz}_${p}_${sz}_${i}.txt 8
		    fi
		done
	done
done


cd ..
git add .
git commit -m "Asgn_8 all files added!"
git push
