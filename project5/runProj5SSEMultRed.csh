#!/bin/bash

#arrysize:
for s in 1000 5000 10000 50000 100000 500000 1000000 5000000 10000000 16000000 20000000 32000000
do
  	echo ARRAYSIZE=$s
        g++ -DARRAYSIZE=$s proj5SSEMultRed.cpp -o proj5SSEMultRed -lm -fopenmp
        ./proj5SSEMultRed >> proj5SSEMultRed.txt
done
