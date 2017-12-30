//#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <math.h>

#define NUMTRIES 10

struct s 
{
	float value;
	int pad[NUMPAD];
} Array[4];

int main(int argc, char *argv[ ] )
{
#ifndef _OPENMP
	fprintf(stderr, "OpenMP is not supported here -- sorry. \n");
	return 1;
#endif

	omp_set_num_threads( NUMT );
	printf("Using %d threads\n", NUMT);
	double maxMegaSums = 0.;
	double sumMegaSums = 0.;
	const int SomeBigNumber = 100000000;	// keep < 2B
	
	for(int t = 0; t < NUMTRIES; t++) 
	{
		double time0 = omp_get_wtime();	
		#pragma omp parallel for
		for( int i = 0; i < 4; i++ )
		{	
			unsigned int seed = 0;		// automatically private
			for( unsigned int j = 0; j < SomeBigNumber; j++ )
			{
				Array[ i ].value = Array[ i ].value + (float)rand_r( &seed );
			}
		}
		double time1 = omp_get_wtime();
		double megaSums = 4 * SomeBigNumber / (time1 - time0) / 1000000.;
		sumMegaSums += megaSums;
		if (megaSums > maxMegaSums)
			maxMegaSums = megaSums;
	}
	double avgMegaSums = sumMegaSums / (double)NUMTRIES;
	printf ("%d %d %8.2lf %8.2lf\n", NUMT, NUMPAD, maxMegaSums, avgMegaSums);
	//printf("          Using PAD =    %d \n", NUMPAD);
	//printf("   Peak performance = %8.2lf MegaSums/Sec\n", maxMegaSums);
	
	return 0;
}

	

