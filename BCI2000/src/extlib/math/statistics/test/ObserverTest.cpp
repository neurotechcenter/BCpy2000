#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <csignal>
#include "StatisticalObserver.h"
#include "WindowObserver.h"
#include "PrecisionTime.h"
#include "ExceptionCatcher.h"

using namespace std;
using namespace StatisticalObserver;

typedef vector<ObserverBase*> ObserverContainer;

#define COLWIDTH 7
#define SEPARATOR "====================================================================================================================="
#define OPTION( x, f )  else if( !::stricmp( argv[i], "--" #x ) ) x = f( ++i<argc ? argv[i] : "" )
#define PRINT( x )      cout << #x ":\t" << x << endl
#define SHOW( x )       cout << setw( COLWIDTH ) << (*i)->x << '\t'

double normpdf( double x )
{
  return exp( - x*x / 2 ) / sqrt( 2 * M_PI );
}

double inverf( double x )
{ // Approximation by Winitzki, S., 2008
  if( x >= 1 - eps )
    return 1 / eps;
  if( x <= -1 + eps )
    return -1 / eps;
  const double a = 8 * ( M_PI - 3 ) / 3 / M_PI / ( 4 - M_PI );
  double signx = ( x > 0 ) - ( x < 0 ),
         b = log( 1 - x * x ),
         c = 2 / M_PI / a + b / 2,
         result = 0;
  result = c * c - b / a;
  result = sqrt( result );
  result -= c;
  result = signx * sqrt( result );
  return result;
}

ObserverBase*
CreateObserver( const char* inName )
{
  string name( inName );
  if( name.find( "Window" ) == 0 )
    return new WindowObserver;
  if( name.find( "Histogram" ) == 0 )
    return new HistogramObserver;
  if( name.find( "Power" ) == 0 )
    return new PowerSumObserver;
  if( name.find( "Combined" ) == 0 )
    return new CombinedObserver;
  if( name.find( "Full" ) == 0 )
    return new CombinedObserver( AllFunctions );
  cerr << "Unknown observer name: " << inName << ", creating a fully configured Observer" << endl;
  return new CombinedObserver( AllFunctions );
}

void
PrintMatrix( const Matrix& inM )
{
  for( size_t i = 0; i < inM.size(); ++i )
  {
    cout << "[\t";
    for( size_t j = 0; j < inM[i].size(); ++j )
      cout << inM[i][j] << '\t';
    cout << "]\n";
  }
}

bool gStop = false;

void
Sighandler( int )
{
  gStop = true;
}

int
main_( int argc, char** argv )
{
  signal( SIGINT, &Sighandler );

  bool help = false;
  int randseed = 0,
      numsamples = -1,
      metabins = 10,
      showskewkurtosis = 0;
  double mean = 0,
         sinefreq = 0,
         sinevar = 0,
         noisevar = 0,
         gaussvar = 0,
         windowlength = -1,
         accuracy = -2,
         leftaccuracy = -1,
         quantile = 0.5;
  const char* left = "FullyConfigured",
            * right = "WindowObserver";

  for( int i = 1; i < argc; ++i )
  {
    if( !stricmp( argv[i], "--help" ) )
    {
      help = true;
    }
    OPTION( randseed, atoi );
    OPTION( numsamples, atoi );
    OPTION( mean, atof );
    OPTION( sinefreq, atof );
    OPTION( sinevar, atof );
    OPTION( gaussvar, atof );
    OPTION( noisevar, atof );
    OPTION( windowlength, atof );
    OPTION( accuracy, atof );
    OPTION( leftaccuracy, atof );
    OPTION( quantile, atof );
    OPTION( left, (char*) );
    OPTION( right, (char*) );
    OPTION( metabins, atoi );
    OPTION( showskewkurtosis, atoi );
    else
      cerr << "Unknown option: " << argv[i] << endl;
  }

  double sineamp = sqrt( 2 * sinevar ),
         noiseamp = 2 * sqrt( 3 * noisevar ),
         gaussamp = sqrt( gaussvar );
  if( accuracy != -2 )
    leftaccuracy = accuracy;
  if( randseed == 0 )
    randseed = static_cast<unsigned int>( time( NULL ) );
  srand( randseed );
  if( quantile < 0 )
    metabins = 0;

  cout << "Parameters may be adjusted from the command line using --<name> <value>:" << endl;
  if( !help )
    cout << SEPARATOR << endl;

  PRINT( randseed );
  PRINT( numsamples );
  PRINT( mean );
  PRINT( sinefreq );
  PRINT( sinevar );
  PRINT( noisevar );
  PRINT( gaussvar );
  PRINT( windowlength );
  PRINT( quantile );
  PRINT( metabins );
  PRINT( showskewkurtosis );
  PRINT( left );
  PRINT( right );

  if( help )
  {
    cout << "\nUse output redirection to create a protocol file." << endl;
    return 0;
  }

  try
  {
    ObserverBase* pLeft = CreateObserver( left ),
                * pRight = CreateObserver( right );
    pLeft->SetQuantileAccuracy( leftaccuracy );
    pRight->SetQuantileAccuracy( -1 );

    Observer p( AllFunctions ); // This is used to observe the quantile output of the two observers.
    p.SetWindowLength( 1000 );
    Observer h( AllFunctions ); // This is used to observe the histogram output of p.
    h.SetWindowLength( 1000 );

    ObserverContainer observers;
    observers.push_back( pLeft );
    observers.push_back( pRight );
    for( ObserverContainer::iterator i = observers.begin(); i != observers.end(); ++i )
      ( *i )->SetWindowLength( windowlength );

    leftaccuracy = pLeft->QuantileAccuracy();
    PRINT( leftaccuracy );

    cout << SEPARATOR << endl;
    cout << "Displaying: Sample | " << left << " observer | " << right << " observer (unlimited accuracy)" << endl;
    cout << "For each observer: Count, Mean, Variance";
    if( showskewkurtosis )
      cout << ", Skewness, Kurtosis";
    if( quantile >= 0 )
      cout << ", Quantile(" << quantile << ")";
    cout << endl;
    cout << SEPARATOR << endl;
    cout << fixed << setprecision( 3 );
    PrecisionTime t = PrecisionTime::Now();

    if( numsamples < 0 )
      cerr << "Observing random data until ctrl-c is pressed." << endl;
    for( int sample = 1; ( sample <= numsamples || numsamples < 0 ) && !gStop; ++sample )
    {
      double value = 0;
      value += mean;
      value += sineamp * cos( ( sample - 1 ) * sinefreq * 2 * M_PI );
      value += noiseamp * ( 1.0 * rand() / RAND_MAX - 0.5 );
      value += gaussamp * sqrt( 2.0 ) * inverf( 2.0 * rand() / RAND_MAX - 1.0 );
      cout << setw( 5 ) << sample << ' ' << setw( COLWIDTH ) << value;

      Vector quantiles( 2 );
      for( ObserverContainer::iterator i = observers.begin(); i != observers.end(); ++i )
      {
        ( *i )->AgeBy( 1 ).Observe( value );
        cout << " | ";
        SHOW( Count() );
        SHOW( Mean()()[0] );
        SHOW( Variance()()[0] );
        if( showskewkurtosis )
        {
          SHOW( Skewness()()[0] );
          SHOW( Kurtosis()()[0] );
        }
        if( quantile >= 0 )
        {
          SHOW( Quantile( quantile )()[0] );
          quantiles[i-observers.begin()] = ( *i )->Quantile( quantile )()[0];
        }
      }
      cout << '\n';

      if( quantile >= 0 )
      {
        p.AgeBy( 1 ).Observe( quantiles );

        Number meandiff = p.Mean()()[1] - p.Mean()()[0],
               dev = sqrt( p.Variance()()[0] + meandiff * meandiff );
        if( dev > eps && metabins > 0 )
        {
          Number center = p.Mean()()[0] + meandiff / 2,
                 resolution = 3 * dev / metabins;
          Vector edges = BinEdges( center, resolution, metabins + 2 );
          Matrix histograms = *p.Histogram( edges );
          histograms /= p.Count();
          h.AgeBy( 1 );
          for( int i = 0; i < metabins; ++i )
          {
            Number value = edges[i] + resolution / 2;
            for( size_t j = 0; j < histograms.size(); ++j )
            {
              Vector buffer( histograms.size() );
              buffer[j] = value;
              h.Observe( buffer, histograms[j][i + 1] );
            }
          }
        }
      }

      if( sample % 1000 == 0 )
      {
        static bool showLegend = true;
        if( showLegend )
        {
          cerr << "<Sample#>:<ms> ";
          showLegend = false;
        }
        PrecisionTime now = PrecisionTime::Now();
        cerr << sample << ":" << PrecisionTime::UnsignedDiff( now, t ) << " " << flush;
        t = now;
      }
    }
    cerr << endl;
    // Statistics over data
    if( metabins > 0 )
    {
      cout << SEPARATOR << endl;
      Vector edges = BinEdges( mean, 3 * sqrt( pLeft->Variance()()[0] ) / metabins, metabins );
      Vector histogram1 = pLeft->Histogram( edges )()[0];
      Vector histogram2 = pRight->Histogram( edges )()[0];
      cout << "Histograms\n\tEdges:";
      for( size_t i = 0; i < edges.size(); ++i )
        cout << "\t" << edges[i];
      cout << endl;
      for( size_t i = 0; i < histogram1.size(); ++i )
        cout << "\t" << histogram1[i];
      cout << endl;
      for( size_t i = 0; i < histogram2.size(); ++i )
        cout << "\t" << histogram2[i];
      cout << endl;
    }
    if( quantile >= 0 )
    {
      cout << SEPARATOR << endl;
      // Statistics over quantiles
      cout << "Quantiles: mean1, mean2, variance1, variance2, covariance, correlation, rsquared" << endl;
      double mean1 = p.Mean()()[0],
             mean2 = p.Mean()()[1],
             meandiff = mean1 - mean2,
             var1 = p.Variance()()[0],
             var2 = p.Variance()()[1],
             cov = p.Covariance()()[0][1];
      cout << "\t\t"
           << '\t' << mean1
           << ",\t" << mean2
           << ",\t" << var1
           << ",\t" << var2
           << ",\t" << cov
           << ",\t" << p.Correlation()()[0][1]
           << ",\t" << RSquared( p, p )()[0][1]
           << endl;
      cout << "Full covariance: " << endl;
      PrintMatrix( *p.Covariance() );
      cout << "Full correlation: " << endl;
      PrintMatrix( *p.Correlation() );
      cout << "Full rsquared: " << endl;
      PrintMatrix( *RSquared( p, p ) );
      cout << "Quantiles: requested accuracies: ";
      for( size_t i = 0; i < observers.size(); ++i )
        cout << '\t' << observers[i]->QuantileAccuracy();
      cout << endl;
      double dev1 = sqrt( var1 + ( meandiff * meandiff ) ), // Standard deviation relative to mean2, accounts for mean difference.
             dev2 = sqrt( var2 ),
             // To convert from deviation into quantile accuracy, we divide deviation by the slope of the inverse CDF,
             // which is just the inverse value of the distribution itself.
             invslope = -1;
      if( fabs( sineamp ) < eps && fabs( noiseamp ) > eps && fabs( gaussamp ) < eps ) // Pure white noise
        invslope = 1 / noiseamp;
      else if( fabs( sineamp ) < eps && fabs( noiseamp ) < eps && fabs( gaussamp ) > eps ) // Pure gaussian noise
        invslope = normpdf( ( mean1 - mean ) / gaussamp ) / gaussamp;
      if( invslope != -1 )
      {
        cout << "Quantiles: empirical accuracies:"
             << '\t' << dev1 * invslope
             << '\t' << dev2 * invslope
             << endl;
      }
    }
    // Statistics over quantiles, from observing resampled histograms
    if( metabins > 0 )
    {
      cout << SEPARATOR << endl;
      cout << "Quantiles meta: mean1, mean2, variance1, variance2, covariance, correlation, rsquared" << endl;
      double mean1 = h.Mean()()[0],
             mean2 = h.Mean()()[1],
             meandiff = mean2 - mean1,
             var1 = h.Variance()()[0],
             var2 = h.Variance()()[1],
             cov = h.Covariance()()[0][1];
      cout << "\t\t"
           << '\t' << mean1
           << ",\t" << mean2
           << ",\t" << var1
           << ",\t" << var2
           << ",\t" << cov
           << ",\t" << h.Correlation()()[0][1]
           << ",\t" << RSquared( h, h )()[0][1]
           << endl;

      Vector edges = BinEdges( ( mean1 + mean2 ) / 2, 3 * sqrt( var2 + meandiff * meandiff ) / metabins, metabins );
      Matrix histograms = *h.Histogram( edges );
      cout << "Histograms:\n\tEdges:";
      for( size_t i = 0; i < edges.size(); ++i )
        cout << "\t" << edges[i];
      cout << endl;
      for( size_t i = 0; i < edges.size() + 1; ++i )
        cout << "\t" << histograms[0][i];
      cout << endl;
      for( size_t i = 0; i < edges.size() + 1; ++i )
        cout << "\t" << histograms[1][i];
      cout << endl;
    }

    for( size_t i = 0; i < observers.size(); ++i )
      delete observers[i];
  }
  catch( exception& e )
  {
    cerr << "Exception caught: " << e.what() << endl;
    return -1;
  }
  return 0;
}

int
main( int argc, char** argv )
{
  FunctionCall< int(int,char**) > call( main_, argc, argv );
  bool finished = ExceptionCatcher()
                 .SetMessage( "Terminating" )
                 .Run( call );
  return finished ? call.Result() : -1;
}

