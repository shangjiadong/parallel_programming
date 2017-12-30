#!/bin/bash
#number of threads:
for t in 1 2 3 4
do
	echo NUMT =$t
	g++ -DNUMT=$t project3Fix2.cpp -o proj3Fix2 -lm -fopenmp
	./proj3Fix2 >> proj3Fix2.txt
done
