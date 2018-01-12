#include "simd.p7.h"
#define NUMTRIES         10
#define SIZE   32768
float Array[2*SIZE];
float  Sums[1*SIZE];


float
SimdMulSum( float *a, float *b, int len )
{
	float sum[4] = { 0., 0., 0., 0. };
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;

	__asm
	(
		".att_syntax\n\t"
		"movq    -40(%rbp), %rbx\n\t"		// a
		"movq    -48(%rbp), %rcx\n\t"		// b
		"leaq    -32(%rbp), %rdx\n\t"		// &sum[0]
		"movups	 (%rdx), %xmm2\n\t"		// 4 copies of 0. in xmm2
	);

	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		__asm
		(
			".att_syntax\n\t"
			"movups	(%rbx), %xmm0\n\t"	// load the first sse register
			"movups	(%rcx), %xmm1\n\t"	// load the second sse register
			"mulps	%xmm1, %xmm0\n\t"	// do the multiply
			"addps	%xmm0, %xmm2\n\t"	// do the add
			"addq $16, %rbx\n\t"
			"addq $16, %rcx\n\t"
		);
	}

	__asm
	(
		".att_syntax\n\t"
		"movups	 %xmm2, (%rdx)\n\t"	// copy the sums back to sum[ ]
	);

	for( int i = limit; i < len; i++ )
	{
		sum[i-limit] += a[i] * b[i];
	}

	return sum[0] + sum[1] + sum[2] + sum[3];
}

int
main (int argc, char *argv[ ])
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
                Array[i+Size] = Array[i];               // duplicate the array
        }
	fclose( fp );

        double maxMegaMults = 0.;
        double sumMegaMults = 0.;

        for( int t = 0; t < NUMTRIES; t++ )
        {               
 		
                double time0 = omp_get_wtime( );
                for( int shift = 0; shift < Size; shift++ )
		{
			float sum = 0.;
			Sums[shift] = SimdMulSum(&Array[0], &Array[0+shift], Size);
		}
                double time1 = omp_get_wtime( );
                double megaMults = (double)Size * (double)Size / (time1-time0)/1000000.;
                sumMegaMults += megaMults;
                if( megaMults > maxMegaMults )
                        maxMegaMults = megaMults;
        }

	double avgMegaMults = sumMegaMults/(double)NUMTRIES;
        printf(" %8.2lf %8.2lf\n", maxMegaMults, avgMegaMults);
        fprintf(stderr, " %8.2lf %8.2lf\n", maxMegaMults, avgMegaMults);
	return 0;
}

