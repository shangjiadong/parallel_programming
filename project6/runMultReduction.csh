#!/bin/bash

#NMB:
for s in 1 4 8 16 32 64 128 256 512 1024 2048 3072 4096 5120 6144 7168 8192 9216 10240
do
	echo NMB=$s
	# LOCAL_SIZE
	for t in 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70
	do
		echo LOCAL_SIZE=$t
		g++ -DLOCAL_SIZE=$t -DNMB=$s -o thirdReduction thirdReduction.cpp libOpenCL.so -w -lm -fopenmp
		./thirdReduction >> proj6MultReduction.txt
	done
done

