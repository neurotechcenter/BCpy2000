//////////////////////////////////////////////////////////////////////////////////////////////
//
//  File:        demoneurod.cpp
//
//  Author:      juergen.mellinger@uni-tuebingen.de
//
//  Date:        Sept 21, 2012
//
//  Description: A test program for the Neuroscan demon framework.
//
///////////////////////////////////////////////////////////////////////////////////////////////
#include "./NeuroSrv.h"
#include <stdexcept>
#include <sstream>
#include <cmath>

using namespace std;

class DemoNeuroSrv : public NeuroSrv
{
 public:
  DemoNeuroSrv( int, char** );
  int Run();
  
 protected:
  virtual int SendData( std::ostream& );
  virtual void SendASTSetupFile( std::ostream& );

 private:
  int mRemArgc;
  char** mRemArgv;
};

DemoNeuroSrv::DemoNeuroSrv( int argc, char** argv )
: mRemArgc( argc ),
  mRemArgv( argv )
{
  int eegChannels = 13,
      eventChannels = 2,
      samplesInBlock = 17,
      samplingRate = 129,
      dataDepth = 4;
  float resolution = 1.f/30;
  
  mBasicInfo = NscBasicInfo( eegChannels, eventChannels, samplesInBlock, samplingRate, dataDepth, resolution );
  mChannelInfo.resize( eegChannels );
  for( size_t i = 0; i < mChannelInfo.size(); ++i )
  {
    EDFHeader::ChannelInfo& ch = mChannelInfo[i];
    ostringstream oss;
    oss << "Demo Ch" << i + 1;
    ch.Label = oss.str();
    ch.PhysicalDimension = "uV";
    ch.TransducerType = "EEG";
    ch.PhysicalMaximum = 5000;
    ch.PhysicalMinimum = -5000;
  }
}

int
DemoNeuroSrv::Run()
{
  return NeuroSrv::Run( mRemArgc, const_cast<const char**>( mRemArgv ) );
}

void
DemoNeuroSrv::SendASTSetupFile( std::ostream& os )
{
  char data[2049];
  for( size_t i = 0; i < sizeof( data ) / sizeof( *data ); ++i )
    data[i] = static_cast<char>( i );
  NscPacketHeader( 'FILE', SetupFile, CtfDSFormat, sizeof( data ) ).Serialize( os );
  os.write( data, sizeof( data ) );
  os.flush();
}

int
DemoNeuroSrv::SendData( std::ostream& os )
{
  int numBytes = ( mBasicInfo.EEGChannels() + mBasicInfo.EventChannels() )
                 * mBasicInfo.SamplesInBlock() * mBasicInfo.DataDepth(),
      dataType;
  switch( mBasicInfo.DataDepth() )
  {
    case 2:
      dataType = DataTypeRaw16bit;
      break;
    case 4:
      dataType = DataTypeRaw32bit;
      break;
    default:
      throw runtime_error( "unsupported value in mBasicInfo.DataDepth()" );
  }
  NscPacketHeader( HeaderIdData, DataType_EegData, dataType, numBytes ).Serialize( os );
  int numNumbers = numBytes / mBasicInfo.DataDepth(),
      amplitude = static_cast<int>( 100 / mBasicInfo.Resolution() );
  switch( dataType )
  {
    case DataTypeRaw16bit:
      for( int i = 0; i < numNumbers; ++i )
        LittleEndianData::put<int16_t>( os, static_cast<int16_t>( ( i * amplitude ) / numNumbers ) );
      break;
    case DataTypeRaw32bit:
      for( int i = 0; i < numNumbers; ++i )
        LittleEndianData::put<int32_t>( os, ( i * amplitude ) / numNumbers );
      break;
  }
  os.flush();
  return ( 1000 * mBasicInfo.SamplesInBlock() ) / mBasicInfo.SamplingRate();
}


int
main( int argc, char* argv[] )
{
  int result = 0;
  try
  {
    result = DemoNeuroSrv( argc, argv ).Run();
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
  return result;
}
