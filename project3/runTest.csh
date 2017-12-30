#!/bin/bash
#number of threads:
for t in 2 4 
do
  echo NUMT = $t
  #number of pad
  for s in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
  do
    echo NUMPAD =$s
    g++ -DNUMPAD=$s -DNUMT=$t test.cpp -o test -lm -fopenmp
    ./test >> test.txt
  done
done

