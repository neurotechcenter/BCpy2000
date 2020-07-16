#include <cstdio>
#include <iostream>
using namespace std;


#include "Quantiles.h"
//typedef Quantilizer<double> MyQuantilizer;

#include <utility> // for pair<>
typedef Quantilizer< pair<double,double> > MyQuantilizer;

#include <cstdlib> // for rand,srand
#include <ctime> // for time


#if _WIN32
# include <Windows.h> // for usleep
void GoToSleep(int inMs) { ::Sleep( inMs ); }
#else
# include <unistd.h> // for usleep
void GoToSleep(int inMs) { ::usleep( inMs * 1000 ); }
#endif // !_WIN32

#ifndef PRECISION_TIME_H
#define PRECISION_TIME_H

class PrecisionTime
{
 public:
  typedef unsigned short NumType;

  PrecisionTime()
    : mValue( 0 ) {}
  PrecisionTime( NumType value )
    : mValue( value ) {}

  NumType operator-( PrecisionTime subtrahend ) const
    { return TimeDiff( subtrahend, *this ); }
  operator NumType() const
    { return mValue; }

  static PrecisionTime Now();
  static NumType TimeDiff( NumType, NumType );
  static int     SignedDiff( NumType, NumType );

 private:
  NumType mValue;
};

#endif // PRECISION_TIME_H

// **************************************************************************
// Function:   Now()
// Purpose:    gets the current time from the system's high-performance timers
// Parameters:
// Returns:    an unsigned sixteen bit value for the current time in ms
//             (-> wrap-around occurs every 65536ms, or about 65 seconds)
// **************************************************************************
#if defined ( _WIN32 )
// **************************************************************************

#include <windows.h>
PrecisionTime
PrecisionTime::Now()
{
  // Get the current time from the Windows precision timer.
  LARGE_INTEGER prectime, prectimebase;
  ::QueryPerformanceCounter( &prectime );
  ::QueryPerformanceFrequency( &prectimebase );
  return static_cast<PrecisionTime::NumType>( ( prectime.QuadPart * 1000 ) / prectimebase.QuadPart );
}

// **************************************************************************
#elif defined ( __APPLE__ )
// **************************************************************************

#include <mach/mach_time.h>
PrecisionTime
PrecisionTime::Now()
{
  static int64_t mt0;
  static double  multiplier = 0.0;  
  if(!multiplier) {
  	mach_timebase_info_data_t mtbinfo;
    mach_timebase_info( &mtbinfo );
    multiplier = 1.0e-6 * (double(mtbinfo.numer) / double(mtbinfo.denom));
    mt0 = mach_absolute_time();
  }
  return multiplier * double(mach_absolute_time() - mt0);
}

// **************************************************************************
#else // neither _WIN32 nor __APPLE__
// **************************************************************************

#include <time.h>
PrecisionTime
PrecisionTime::Now()
{
  // Use clock_gettime() on non-Windows systems with librt.
  struct timespec t;
  ::clock_gettime( CLOCK_REALTIME, &t );
  return ( t.tv_sec * 1000 ) + t.tv_nsec / 1000000;
}

// **************************************************************************
#endif // _WIN32, __APPLE__
// **************************************************************************



// **************************************************************************
// Function:   TimeDiff
// Purpose:    calculates the difference between two times (i.e., time2-time1)
//             takes roll-over into account (in case time2 < time1)
// Parameters: time1, time2 - two 16 bit integers
// Returns:    time2-time1, if time2-time1 >= 0
//             or time2-time1+65536, if time2-time1 < 0
// **************************************************************************
PrecisionTime::NumType
PrecisionTime::TimeDiff( NumType time1, NumType time2 )
{
  const int maxdiffPlusOne = 1 << ( 8 * sizeof( time1 ) );
  return ( ( time2 + maxdiffPlusOne ) - time1 ) % maxdiffPlusOne;
}

// **************************************************************************
// Function:   SignedDiff
// Purpose:    calculates the signed difference between two times,
//             taking roll-over into account.
// Parameters: time1, time2 - two 16 bit integers
// Returns:    signed difference
// **************************************************************************
int
PrecisionTime::SignedDiff( NumType time1, NumType time2 )
{
  const int wraparound = 1 << ( 8 * sizeof( time1 ) );
  int diff = time1 - time2;
  if( diff >= wraparound / 2 )
    diff -= wraparound;
  else if( diff < -wraparound / 2 )
    diff += wraparound;
  return diff;
}


int main( int argc, const char *argv[] )
{
	/**/
	
	double millisecondsPerBuffer = 60*1000;
	double millisecondsPerPacket = 50.;
	MyQuantilizer qq( (unsigned int)(0.5 + millisecondsPerBuffer / millisecondsPerPacket) );
	const unsigned int N = 100;
	MyQuantilizer qqv[N];
	for ( unsigned int iq = 0; iq < N; iq++ ) qqv[iq].Reset( (unsigned int)(0.5 + millisecondsPerBuffer / millisecondsPerPacket) );
	cout << qq.Capacity() << endl;
	bool full = false;
	double x = 0.0;
	PrecisionTime::NumType lastTime = PrecisionTime::Now();
	srand(time(NULL));
	for( unsigned int ipacket = 0; ; ipacket++)
	{
		x = (double(rand()) / double(RAND_MAX));
		qq << pair<double,double>(x,-x);
		for ( unsigned int iq = 0; iq < N; iq++ ) qqv[iq] << pair<double,double>(x,-x);
		if( !full && qq.Full() ) { full = true; cout << "Full!\n"; }
		if( qq.Full() ) while( PrecisionTime::TimeDiff( lastTime, PrecisionTime::Now() ) < millisecondsPerPacket ) GoToSleep(1);
		if( ipacket % 20 == 0 ) printf("% 6d % 10.3f % 10.3f % 10.3f\n", qq.Membership(), x, qq.Quantile(0.025).first, qq.Quantile(0.975).first); 
		lastTime = PrecisionTime::Now();
	}
	/**/
	
		
	double v;
	MyQuantilizer q(9);
	
	while(1)
	{
		cout << endl;
		for(MyQuantilizer::iterator i = q.begin(); i != q.end(); i++ ) printf("%5.3f  %g %g\n", i.where(), i->first, i->second);
		cout << "\n\nValue? "; cin >> v;
		q << pair<double,double>(v,v*v);
	}
	return 0;
}
