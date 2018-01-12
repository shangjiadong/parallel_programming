#!/bin/bash
# number of threads:
for t in 1 4 8 16 32 64
do
	echo NUMT = $t
	g++ -DNUMT=$t autoOpenMPOneThread.cpp -o autoOpenMP -lm -fopenmp
	./autoOpenMP >> autoOpenMP.txt
done
