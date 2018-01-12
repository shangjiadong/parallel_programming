#!/bin/bash

g++ simd.p7.cpp -o simdMulSum -lm -fopenmp
./simdMulSum >> simdMulSum.txt
