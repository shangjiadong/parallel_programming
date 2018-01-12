#!/bin/bash

#NMB:
for s in 1 4 8 16 32 64 128 256 512 1024 2048 3072 4096 5120 6144 7168 8192 9216 10240
do
	echo NMB=$s
	# LOCAL_SIZE
        for t in 8 16 32 64 128	256 512
        do
                echo LOCAL_SIZE=$t
		g++ -DLOCAL_SIZE=$t -DNMB=$s -o firstSum firstSum.cpp /scratch/cuda-7.0/lib64/libOpenCL.so -w -lm -fopenmp
		./firstSum >> proj6MultSum.txt
	done
done

