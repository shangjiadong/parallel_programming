#!/bin/csh

# number of threads:
foreach t (1 2 4 6 8 10 20)
	echo NUMT = $t
	#number of nodes
	foreach s (5 10 20 50 100 200 500 1000 2000 5000 10000)
		echo NUMNODES = $s
		g++ -DNUMNODES=$s -DNUMT=$t project1.cpp -o project1 -lm -fopenmp
		./project1 >> res.txt
		
	end
end
