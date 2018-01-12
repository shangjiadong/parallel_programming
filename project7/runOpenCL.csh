#!/bin/bash

g++ -o autoCor autoCor.cpp libOpenCL.so -Wno-write-strings -lm -fopenmp
./autoCor >> autoCor.txt
