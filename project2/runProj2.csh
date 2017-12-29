#!/bin/bash

#number of threads
for t in 1 2 4 6 8 10 12 14 16 24
do
	echo NUMT=$t
	#Chunk size
	for s in 1 4096
	do
		echo CHUNKSIZE = $s
		g++ -DCHUNKSIZE=$s -DNUMT=$t project2.cpp -o project2 -lm -fopenmp
		./project2 >> proj2res.txt
	done
done
