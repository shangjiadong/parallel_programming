#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
//#define NUMT	         1
#define NUMTRIES         10
#define SIZE   32768
float Array[2*SIZE];
float  Sums[1*SIZE];

int
main()
{

	FILE *fp = fopen( "signal.txt", "r" );
	if( fp == NULL )
	{
		fprintf( stderr, "Cannot open file 'signal.txt'\n" );
		exit( 1 );
	}

	int Size;
	fscanf( fp, "%d", &Size );
	Size = SIZE;

	for( int i = 0; i < Size; i++ )
	{
		fscanf( fp, "%f", &Array[i] );
		Array[i+Size] = Array[i];		// duplicate the array
	}
	fclose( fp );

#ifndef _OPENMP
        fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
        return 1;
#endif

        omp_set_num_threads( NUMT );
        fprintf( stderr, "Using %d threads\n", NUMT );

        double maxMegaMults = 0.;
        double sumMegaMults = 0.;

        for( int t = 0; t < NUMTRIES; t++ )
        {
                double time0 = omp_get_wtime( );

                #pragma omp parallel for
                for( int shift = 0; shift < Size; shift++ )
		{
			float sum = 0.;
			for( int i = 0; i < Size; i++ )
			{
				sum += Array[i] * Array[i + shift];
			}
			Sums[shift] = sum;	// note the "fix #2" from false sharing if you are using OpenMP
		}
                double time1 = omp_get_wtime( );
                double megaMults = (double)Size * (double)Size/(time1-time0)/1000000.;
                sumMegaMults += megaMults;
                if( megaMults > maxMegaMults )
                        maxMegaMults = megaMults;
        }

        double avgMegaMults = sumMegaMults/(double)NUMTRIES;
        //printf( " %8.2lf %8.2lf\n", maxMegaMults, avgMegaMults );
	fprintf (stderr, " %8.2lf %8.2lf\n ", maxMegaMults, avgMegaMults);
	for(int i = 0; i < Size; i++)
	{
		printf("%8.2lf\t", Sums[i]);
	}
	printf( "%8.2lf\t", Sums);
	//fprintf (stderr, " %8.2lf %8.2lf\n ", maxMegaMults, avgMegaMults);
        return 0;
}

