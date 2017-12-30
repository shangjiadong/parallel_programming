#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <stdio.h>

int	NowYear;		// 2017 - 2022
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		// number of deer in the current population
int	NowNumWolf; // number of wolf in the current population

const float GRAIN_GROWS_PER_MONTH =		10.0;
const float ONE_DEER_EATS_PER_MONTH =		0.5;

const float AVG_PRECIP_PER_MONTH =		6.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				50.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				40.0;
const float MIDPRECIP =				10.0;

unsigned int seed = 0;  // a thread-private variable
//float x = Ranf( &seed, -1.f, 1.f );

float
  Ranf( unsigned int *seedp,  float low, float high )
  {
	float r = (float) rand_r( seedp );              // 0 - RAND_MAX
    	return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
  }

//int
  //Ranf( unsigned int *seedp, int ilow, int ihigh )
  //{
    	//float low = (float)ilow;
    	//float high = (float)ihigh + 0.9999f;
    	//return (int)(  Ranf(seedp, low,high) );
  //}

float
  SQR( float x )
  {
    	return x*x;
  }

void Grain () 
{
  	float TmpHeight = 0.;
	while (NowYear < 2023) 
	{
		//float TmpHeight = 0.;    
		float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
    		float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );
    		TmpHeight = NowHeight + tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
    		TmpHeight = TmpHeight- (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
    		// DoneComputing barrier:
    		#pragma omp barrier
    		NowHeight = TmpHeight;
		if (NowHeight < 0 )
			NowHeight = 0;
		//return NowHeight;
    		// DoneAssigning barrier:
    		#pragma omp barrier
    
    		// DonePrinting barrier:
    		#pragma omp barrier
	}

}

void GrainDeer()
{
  	int TmpNumDeer;
  	while (NowYear < 2023) 
	{
		//int TmpNumDeer = 0;
    		
    		if(NowNumDeer < NowHeight -5)
		{
      			TmpNumDeer = NowNumDeer + 10;
			TmpNumDeer = TmpNumDeer - NowNumWolf * 0.1;
		}
    		else if(NowNumDeer >= NowHeight - 5) 
		{
      			TmpNumDeer = NowNumDeer - 5;
			TmpNumDeer = TmpNumDeer - NowNumWolf * 0.1;
		}
    		// DoneComputing barrier:
    		#pragma omp barrier
    		NowNumDeer = TmpNumDeer;
		if (NowNumDeer < 0)
			NowNumDeer = 0;
  				
 		// DoneAssigning barrier:
    		#pragma omp barrier
    
    		// DonePrinting barrier:
    		#pragma omp barrier
	}
}

void DeerWolf()
{
	int TmpNumWolf;
 	 while( NowYear < 2023 )
  	{
    		//int TmpNumWolf = 0;
    		if (NowNumWolf <= NowNumDeer-2) 
		{
      			TmpNumWolf = (NowNumWolf + 1);
		}
    		else if(NowNumWolf > NowNumDeer-4) 
		{
      			TmpNumWolf = (NowNumWolf - 1);
		}
   		// DoneComputing barrier:
    		#pragma omp barrier
    		NowNumWolf = TmpNumWolf;
		if(NowNumWolf < 0 )
			NowNumWolf = 0;
    		// DoneAssigning barrier:
    		#pragma omp barrier
    
    		// DonePrinting barrier:
    		#pragma omp barrier
    
  	}
}

void Watcher() 
{
	while (NowYear < 2023) 
    	{
  		// DoneComputing barrier:
    		#pragma omp barrier
  
  		// DoneAssigning barrier:
    		#pragma omp barrier
  
  		// DonePrinting barrier:
    		fprintf(stderr, "%d, %d, %8.2lf, %8.2lf, %8.2lf, %d, %d\n", NowYear, NowMonth, NowTemp, NowPrecip, NowHeight, NowNumDeer, NowNumWolf);
    
    		if (NowMonth == 11) 
		{
      			NowMonth = 0;
      			NowYear += 1;
    		}
    		else {
      			NowMonth += 1;
    		}
    		float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
    		float temp = AVG_TEMP - AMP_TEMP * cos( ang );
    		//unsigned int seed = 0;
    		NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );
    		float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    		NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
    		if( NowPrecip < 0. ) {
      			NowPrecip = 0.;
		}
   	 	#pragma omp barrier
  	}
}

int main(int argc, char *argv[ ]) 
{
  	// starting date and time:
  	NowMonth =    0;
  	NowYear  = 2017;
  
  	// starting state (feel free to change this if you want):
  	NowNumDeer = 20;
  	NowHeight =  20.;
  	NowNumWolf = 1;
  
  	float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );
  	float temp = AVG_TEMP - AMP_TEMP * cos( ang );
  	unsigned int seed = 0;
  	NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );
  	float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
  	NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
  	if( NowPrecip < 0. ) {
    		NowPrecip = 0.;
  	}
  	omp_set_num_threads( 4 );	// same as # of sections
  	#pragma omp parallel sections
  	{
    		#pragma omp section
    		{
      			GrainDeer( );
    		}
    
    		#pragma omp section
    		{
      			Grain( );
    		}
    
    		#pragma omp section
    		{
      			Watcher( );
    		}
    
    		#pragma omp section
    		{
      			DeerWolf( );	// your own
    		}
  	}       // implied barrier -- all functions must return in order
	// to allow any of them to get past here
  	return 0;
}







