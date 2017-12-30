
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <math.h>

//#define NUMT 4
//#define NUMPAD 15
#define NUMTRIES 10
struct s 
{
	float value;
	int pad[NUMPAD];
}Array[4];
int main() 
{
#ifndef _OPENMP
	fprintf(stderr, "OpenMP is not supported here -- sorry. \n");
	return 1;
#endif

	omp_set_num_threads( NUMT );
	fprintf( stderr, "Using %d threads\n", NUMT);
	double maxMegaMults = 0.;
	double sumMegaMults = 0.;
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
		double megaMults = 4 * SomeBigNumber / (time1 - time0) / 1000000.;
		sumMegaMults += megaMults;
		if (megaMults > maxMegaMults)
			maxMegaMults = megaMults;
	}
	double avgMegaMults = sumMegaMults / (double)NUMTRIES;
	fprintf(stderr, "Using %d\n PAD", NUMPAD);
	fprintf(stderr, "   Peak performance = %8.2lf MegaMults/Sec\n", maxMegaMults);
	fprintf(stderr, "Average performance = %8.2lf MegaMults/sec\n", avgMegaMults);
	return 0;
}

	
