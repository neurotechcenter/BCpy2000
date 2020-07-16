//////////////////////////////////////////////////////////////////////////////////////////////
//
//  File:        ctfneurod.cpp
//
//  Author:      juergen.mellinger@uni-tuebingen.de
//
//  Date:        Jul 27, 2004
//
//  Description: The main() function and signal handling for the CTF to Neuroscan forwarding
//               daemon.
//
///////////////////////////////////////////////////////////////////////////////////////////////
#include "./CtfNeuroSrv.h"
#include <string>
#include <fstream>
#include <exception>
#include <csignal>
#include <regex.h>

using namespace std;

static NeuroSrv* pNeuroSrv = NULL;

void
Terminate( int )
{
  delete pNeuroSrv;
  ::exit( 0 );
}

int
main( int argc, const char* argv[] )
{
  cout << argv[0] << ", " __DATE__ " " __TIME__ << endl;

  int rem_argc = 1;
  const char** rem_argv = new const char* [ argc ];
  rem_argv[ 0 ] = argv[ 0 ];
  ofstream elocFile, sourceParamFile;
  const char* pattern = NULL;
  double freqCorrectionFactor = 1.0;
  int outputFormat = DataTypeRaw16bit;
  bool disableHP = false;

  int i = 1;
  while( i < argc )
  {
#ifdef APPREVISION
    if( string( argv[ i ] ) == "--version" || string( argv[ i ] ) == "-v" )
    {
      cout << APPREVISION << '\n';
# ifdef COPYRIGHT
      cout << COPYRIGHT << '\n';
# endif // COPYRIGHT
      return 0;
    }
#endif // APPREVISION
    if( string( argv[ i ] ) == "--elocFile" || string( argv[ i ] ) == "-e" )
    {
      if( argc >= i )
        elocFile.open( argv[ ++i ] );
    }
    if( string( argv[ i ] ) == "--sourceParamFile" || string( argv[ i ] ) == "-s" )
    {
      if( argc >= i )
        sourceParamFile.open( argv[ ++i ] );
    }
    if( string( argv[ i ] ) == "--pattern" || string( argv[ i ] ) == "-p" )
    {
      if( argc >= i )
	pattern = argv[ ++i ];
    }
    else if( string( argv[ i ] ) == "--freqCorrection" || string( argv[ i ] ) == "-f" )
    {
      if( argc >= i )
        freqCorrectionFactor = ::atof( argv[ ++i ] );
    }
    else if( string( argv[ i ] ) == "--outputFormat" || string( argv[ i ] ) == "-o" )
    {
      if( argc >= i )
      {
        string outputFormatString = argv[ ++i ];
        if( outputFormatString.find( "16" ) != string::npos )
          outputFormat = DataTypeRaw16bit;
        else if( outputFormatString.find( "32" ) != string::npos )
          outputFormat = DataTypeRaw32bit;
        else
	{
          cerr << "Unknown output format: " << outputFormatString << ",\n"
               << "  use \"16\" or \"32\" as output format specifiers."
               << endl;
          return -1;
	}
      } 
    }
    else if( string( argv[ i ] ) == "--disableHP" || string( argv[ i ] ) == "-d" )
    {
      disableHP = true;
    }
    else
    {
      rem_argv[ rem_argc++ ] = argv[ i ];
    }
    ++i;
  }
  
  regex_t compiledPattern, *pCompiledPattern = NULL;
  if( pattern )
  {
    pCompiledPattern = &compiledPattern;
    int err = ::regcomp( pCompiledPattern, pattern, REG_EXTENDED );
    if( err )
    {
      const int bufLen = 256;
      char messageBuffer[ 256 ];
      ::regerror( err, pCompiledPattern, messageBuffer, bufLen );
      cerr << "Error compiling regular expression: " << messageBuffer << endl;
      return -1;
    }
  }

  if( outputFormat == DataTypeRaw16bit && disableHP )
    cout << "Warning: High-pass filtering disabled for 16 bit output." << endl;

  const int terminatingSignals[] =
  {
    SIGINT, SIGKILL, SIGTERM,
  };
  for( size_t i = 0; i < sizeof( terminatingSignals ) / sizeof( *terminatingSignals ); ++i )
    ::signal( terminatingSignals[ i ], Terminate );

  int result = 0;
  try
  {
    pNeuroSrv = new CtfNeuroSrv( pCompiledPattern, freqCorrectionFactor, outputFormat, !disableHP, elocFile, sourceParamFile );
    result = pNeuroSrv->Run( rem_argc, rem_argv );
  }
  catch( const exception& e )
  {
    cerr << "Exception caught: " << e.what() << endl;
    result = -1;
  }
  catch( const char* s )
  {
    cerr << "Exception caught: " << s << endl;
    result = -1;
  }
  catch( ... )
  {
    cerr << "Unknown exception caught" << endl;
    result = -1;
  }
  if( pCompiledPattern )
    ::regfree( pCompiledPattern );
  delete pNeuroSrv;
  delete[] rem_argv;
  return result;
}
