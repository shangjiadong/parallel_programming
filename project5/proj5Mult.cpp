#include <stdio.h>
#include <omp.h>
#include <math.h>

//#define ARRAYSIZE 10000
#define NUMTRIES 10

float A[ARRAYSIZE];
float B[ARRAYSIZE];
float C[ARRAYSIZE];

int main (int argc, char *argv[ ])
{
	double maxMegaMults = 0.;
	double sumMegaMults = 0.;
	for (int t = 0; t < NUMTRIES; t++)
	{
		double time0 = omp_get_wtime();
		for (int i = 0; i < ARRAYSIZE; i++)
		{
			C[i] = A[i] * B[i];
		}
		double time1 = omp_get_wtime();
		double megaMults = (double) ARRAYSIZE / (time1 - time0) / 1000000.;
		sumMegaMults += megaMults;
		if (megaMults > maxMegaMults)
			maxMegaMults = megaMults;
	}
	double avgMegaMults = sumMegaMults /(double)NUMTRIES;
	printf ("%8.2lf, %8.2lf\n", maxMegaMults, avgMegaMults);
	return 0;

}
