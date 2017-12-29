
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <cstdlib>

float
Ranf(unsigned int *seedp, float low, float high)
{
	float r = (float)rand_r(seedp);		// 0 - RAND_MAX

	return(low + r * (high - low) / (float)RAND_MAX);
}

//#define CHUNKSIZE 4096
#define ARRAYSIZE 50000
//#define NUMT 2
#define NUMTRIES 10
float Array[ARRAYSIZE];



int main ()
{
#ifndef _OPENMP
	fprintf(stderr, "OpenMP is not supported here -- sorry.\n");
	return 1;
#endif 
	//int maxNum = omp_get_max_threads();
	//fprintf(stderr, "maximum %d threads\n", maxNum);
	omp_set_num_threads(NUMT);
	fprintf(stderr, "Using %d threads\n", NUMT);
	double maxMegaMults = 0.;
	double sumMegaMults = 0.;
	
	for (int h=0; h < ARRAYSIZE; h++)
	{
		unsigned int seed = 0;
		Array[h] = Ranf (&seed, -1.f, 1.f);
	}
	for (int t=0; t<NUMTRIES; t++)
	{
		//float prod = 0.;
		double time0 = omp_get_wtime();
		#pragma omp parallel for default(none),shared(Array), schedule(dynamic, CHUNKSIZE)

		//#pragma omp parallel for default(none),shared(Array), schedule(static, CHUNKSIZE)
		for (int i = 0; i < ARRAYSIZE; i++)
		{
				
    			float prod = 1;
			for (int j = 0; j < i; j++)
			{
				prod *= Array[j];
			}
		}
		double time1 = omp_get_wtime();


		// count of how many multiplications were done:
		long int numMuled = (long int)ARRAYSIZE * (long int)(ARRAYSIZE + 1) / 2;
		double megaMults = (double)numMuled / (time1 - time0) / 1000000.;
		sumMegaMults += megaMults;
		if (megaMults > maxMegaMults)
			maxMegaMults = megaMults;
	
		//fprintf(stderr, "time diff = %10.10lf\n", (time1 - time0));
		//fprintf(stderr, "Threads = %2d; ChunkSize = %5d; Scheduling=static ; MegaMults/sec = %10.2lf\n", NUMT, CHUNKSIZE, (double)numMuled / (time1 - time0) / 1000000.);
	}
	double avgMegaMults = sumMegaMults / (double)NUMTRIES;
	//fprintf(stderr, "time diff = %10.2lf\n", time1 - time0);	
	fprintf(stderr, "Threads = %2d \n", NUMT);
	fprintf(stderr, "Chunksize = %5d \n", CHUNKSIZE);
	fprintf(stderr, "  Peak Performance = %10.2lf MegaMults/Sec\n", maxMegaMults);
	fprintf(stderr, "Average Performance = %10.2lf MegaMults/Sec\n", avgMegaMults);
	return 0;
}
