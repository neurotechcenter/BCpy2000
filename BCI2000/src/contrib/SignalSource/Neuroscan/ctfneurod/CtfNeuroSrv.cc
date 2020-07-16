//////////////////////////////////////////////////////////////////////////////////////////////
//
//  File:        CtfNeuroSrv.cpp
//
//  Author:      juergen.mellinger@uni-tuebingen.de
//
//  Date:        Jul 27, 2004
//
//  Description: A class implementing a Neuroscan Acquire forwarding daemon for CTF's
//               shared memory interface.
//
///////////////////////////////////////////////////////////////////////////////////////////////
#include "CtfNeuroSrv.h"

#include "ACQ_MessagePacket.h"
#include "PhysicalSet.h"
#include "MEGDefs.h"

#include <iostream>
#include <string>
#include <sstream>

#include <sys/shm.h>
#include <sys/time.h>
#include <errno.h>
#include <regex.h>

#undef BIG_ENDIAN_INPUT

using namespace std;

CtfNeuroSrv::CtfNeuroSrv(
  const regex_t* inChannelNameRegex,
  double         inFreqCorrectionFactor,
  int            inDataOutputFormat,
  bool           inDoHPFiltering,
  ostream&       ioElocFile,
  ostream&       ioSourceParamFile  )
: mShmHandle( -1 ),
  mPacketIndex( 0 ),
  mpMessageQueue( NULL ),
  mDecayFactor( 0.0 ),
  mpChannelNameRegex( inChannelNameRegex ),
  mFreqCorrectionFactor( inFreqCorrectionFactor ),
  mDataOutputFormat( inDataOutputFormat ),
  mBytesPerSample( 0 ),
  mDoHPFiltering( inDoHPFiltering ),
  mrElocFile( ioElocFile ),
  mrSourceParamFile( ioSourceParamFile )
{
  size_t shmSize = sizeof( ACQ_MessagePacket ) * ACQ_MSGQ_SIZE;
  mShmHandle = ::shmget( ACQ_MESQ_SHMKEY, shmSize, 0666 | IPC_CREAT );
  if( mShmHandle == -1 )
  {
    cerr << "Could not create shared memory block of size "
         << shmSize / ( 1 << 10 ) << "kB"
         << " (" << ::strerror( errno ) << ")"
         << endl;
    return;
  }
  void* shmPtr = ::shmat( mShmHandle, 0, 0 );
  if( shmPtr == reinterpret_cast<void*>( -1 ) )
  {
    cerr << "Could not attach to shared memory" << endl;
    return;
  }
  mpMessageQueue = reinterpret_cast<ACQ_MessagePacket*>( shmPtr );
  for( int i = ACQ_MSGQ_SIZE - 1; i >= 0; --i )
    mpMessageQueue[ i ].message_type = ACQ_MSGQ_INVALID;
  // Block until the ctf program wrote the setup info.
  cout << "Waiting for the CTF Acq program to send configuration data... " << flush;
  while( mpMessageQueue[ 0 ].message_type != ACQ_MSGQ_SETUP_COLLECTION
         || mpMessageQueue[ 1 ].message_type != ACQ_MSGQ_DATA )
      Sleep( 100 );
  cout << "received." << endl;
  try
  {
    mDsPath = reinterpret_cast<const char*>( mpMessageQueue[ 0 ].data );
    PhysicalSet ds( mDsPath.c_str() );
    const ACQ_MessagePacket& firstPacket = mpMessageQueue[ 1 ];
    if( firstPacket.numChannels != ds.getNumberOfChannels() )
      cerr << "Received inconsistent data" << endl;

    double bitIgnoringFactor = 1.0;
    switch( mDataOutputFormat )
    {
      case DataTypeRaw16bit:
        bitIgnoringFactor = 1.0 / ( 1 << cBitsToIgnore );
        break;
      case DataTypeRaw32bit:
        bitIgnoringFactor = 1.0;
        break;
    }
    mChannelIndices.clear();
    mChannelGains.clear();
    mHPChannelOffsets.clear();
    vector<int> channelTypes;
    vector<string> channelNames;
    vector<Point> megPos;

    for( Channel_t i = 0; i < ds.getNumberOfChannels(); ++i )
    {
      const Channel& ch = ds.getChannel( i );
      bool useChannel = false;
      if( mpChannelNameRegex )
      useChannel = !::regexec( mpChannelNameRegex, ch.getName(), 0, NULL, 0 );
      else switch( ch.getSensorClass() )
      {
        case MEGSensor:
        case badMEGSensor:
          useChannel = true;
          break;
      }
      if( useChannel )
      {
        mChannelIndices.push_back( i );
        channelTypes.push_back( ch.getSensorClass() );
        switch( ch.getSensorClass() )
        {
          case EEGSensor:
          case badEEGSensor:
            mChannelGains.push_back( cEEGRoughGain / ch.getProperGain() / ch.getQGain() * bitIgnoringFactor );
            break;
          case MEGSensor:
          case badMEGSensor:
                  mChannelGains.push_back( cMEGRoughGain / ch.getProperGain() / ch.getQGain() * bitIgnoringFactor );
            break;
          default:
            mChannelGains.push_back( 1.0 );
        }
        mHPChannelOffsets.push_back( 0.0 );
        channelNames.push_back( string( ch.getName() ) );
        switch( ch.getSensorClass() )
        {
          case MEGSensor:
          case badMEGSensor:
            megPos.push_back( ch.getDewarCoilPosition( 0 ) );
            break;
          default:
            ;
        }
      }
    }

    Point centerPos;
    for( size_t i = 0; i < megPos.size(); ++i )
      centerPos += megPos[ i ];
    centerPos /= megPos.size();

    double maxRadius = 0;
    vector<double> megAngles, megRadii;
    for( size_t i = 0; i < megPos.size(); ++i )
    {
      Point p = megPos[ i ] - centerPos;
      megAngles.push_back( p.getTheta() );
      double r = p.getPhi();
      maxRadius = max( r, maxRadius );
      megRadii.push_back( r );
    }

    mDecayFactor = ::exp( -1.0 / ds.getSampleRate() / mFreqCorrectionFactor / cHPTimeConstant );
    double resolution = 1.0;
    switch( mDataOutputFormat )
    {
      case DataTypeRaw16bit:
        resolution = cEEGUnitGain;
        mBytesPerSample = 2;
        break;
      case DataTypeRaw32bit:
        resolution = cEEGUnitGain / ( 1 << cBitsToIgnore );
        mBytesPerSample = 4;
        break;
      default:
        cerr << "Unknown output data type: " << mDataOutputFormat << endl;
    }
    size_t numChannels = mChannelIndices.size();
    int samplingRate = ::floor( ds.getSampleRate() * mFreqCorrectionFactor + 0.5 );
    ostringstream parmChannelNames, parmSourceCh, parmSourceChGain, parmSourceChOffset,
                  parmSamplingRate, parmSampleBlockSize;
    parmChannelNames << "Source stringlist ChannelNames= " << numChannels << " ";
    parmSourceCh << "Source int SourceCh= " << numChannels;
    parmSourceChGain << "Source floatlist SourceChGain= " << numChannels << " ";
    parmSourceChOffset << "Source floatlist SourceChOffset= " << numChannels << " ";
    parmSamplingRate << "Source float SamplingRate= " << samplingRate << "Hz";
    parmSampleBlockSize << "Source int SampleBlockSize= " << firstPacket.numSamples;
    mChannelInfo.resize( numChannels );
    double pi = 4 * ::atan( 1 );
    size_t megIdx = 0;
    for( size_t i = 0; i < numChannels; ++i )
    {
      mChannelInfo[i].Label = channelNames[i];
      string elocName = channelNames[ i ];
      double elocAngle = 0,
             elocRadius = 1;
      switch( channelTypes[ i ] )
      {
        case EEGSensor:
          mChannelInfo[i].TransducerType = "EEG";
          break;
        case MEGSensor:
        case badMEGSensor:
          mChannelInfo[i].TransducerType = "MEG";
          mChannelInfo[i].PhysicalDimension = "fT";
          elocName = channelNames[ i ].substr( 0, 4 );
          elocRadius = megRadii[ megIdx ] / 2 / maxRadius;
          elocAngle = - megAngles[ megIdx++ ] * 180 / pi + 45;
          break;
        default:
          ;
      }
      for( size_t j = elocName.length(); j < 4; ++j )
        elocName += '.';
      mrElocFile << i + 1 << ' ' << elocAngle << ' ' << elocRadius << ' ' << elocName << "\r\n";

      parmChannelNames << ' ' << channelNames[ i ];
      parmSourceChGain << ' ' << resolution;
      parmSourceChOffset << ' ' << int( 0 );
    }
    mrElocFile.flush();
    time_t t = ::time( NULL );
    string comment = " // Created by ctfneurod, ";
    comment += ::ctime( &t );
    comment = comment.substr( 0, comment.length() - 1 );
    mrSourceParamFile << parmSourceCh.str() << comment << "\r\n"
                      << parmSourceChGain.str() << comment << "\r\n"
                      << parmSourceChOffset.str() << comment << "\r\n"
                      << parmChannelNames.str() << comment << "\r\n"
                      << parmSamplingRate.str() << comment << "\r\n"
                      << parmSampleBlockSize.str() << comment << "\r\n"
                      << flush;

    mBasicInfo = NscBasicInfo(
      mChannelIndices.size(), // signal channels
      0,                      // event channels
      firstPacket.numSamples, // samples per block
      samplingRate, // sampling rate
      mBytesPerSample,        // bytes per sample
      resolution              // resolution
    );
    cout << mBasicInfo << endl;
  }
  catch( const Error& err )
  {
    cerr << "Could not retrieve channel setup information" << endl
         << err.getMessage() << endl;
    TerminateConnection();
  }
  mPacketIndex = 1;
}

CtfNeuroSrv::~CtfNeuroSrv()
{
  if( ::shmdt( mpMessageQueue ) == -1 )
    cerr << "Could not detach from shared memory block"
         << " (" << ::strerror( errno ) << ")"
         << endl;
  if( ::shmctl( mShmHandle, IPC_RMID, 0 ) == -1 )
    cerr << "Could not mark shared memory block for removal"
         << " (" << ::strerror( errno ) << ")"
         << endl;
}

void
CtfNeuroSrv::SendASTSetupFile( ostream& os )
{
  char* pBuf = 0;
  size_t length = 0;

  char* pTemp = ::tempnam( 0, "ast" );
  string command = "tar -Pczf \"";
  command += pTemp;
  command += "\" \"" + mDsPath + "\"";
  if( ::system( command.c_str() ) )
    cerr << "Could not execute command: " << command << endl;
  else
  {
    ifstream file( pTemp );
    if( file.is_open() )
    {
      file.seekg( 0, file.end );
      length = file.tellg();
      file.seekg( 0, file.beg );
      pBuf = new char[length];
      file.read( pBuf, length );
    }
    if( !file )
      cerr << "Could not read file: " << pTemp << endl;
  }
  NscPacketHeader( 'FILE', SetupFile, 'CTFD', length ).WriteBinary( os );
  os.write( pBuf, length );
  os.flush();
  delete[] pBuf;
  ::unlink( pTemp );
  ::free( pTemp );
}

void
CtfNeuroSrv::StartSendingData( ostream& os )
{
  // Release all packets and await data at the beginning of the queue buffer.
  for( int i = 0; i < ACQ_MSGQ_SIZE; ++i )
    mpMessageQueue[ i ].message_type = ACQ_MSGQ_INVALID;
  mPacketIndex = 0;

  NeuroSrv::StartSendingData( os );
}

void
CtfNeuroSrv::StopSendingData( ostream& os )
{
  // Release all packets and have the packet index point to a termination message.
  for( int i = 1; i < ACQ_MSGQ_SIZE; ++i )
    mpMessageQueue[ i ].message_type = ACQ_MSGQ_INVALID;
  mpMessageQueue[ 0 ].message_type = ACQ_MSGQ_CLOSE_COLLECTION;
  mPacketIndex = 0;

  NeuroSrv::StopSendingData( os );
}

int
CtfNeuroSrv::SendData( ostream& os )
{
  ACQ_MessagePacket* curPacket = mpMessageQueue + mPacketIndex;
  while( curPacket->message_type == ACQ_MSGQ_INVALID )
  {
    ++mPacketIndex %= ACQ_MSGQ_SIZE;
    curPacket = mpMessageQueue + mPacketIndex;
  }
  while( curPacket->message_type != ACQ_MSGQ_INVALID )
  {
    switch( curPacket->message_type )
    {
      case ACQ_MSGQ_DATA:
      {
        size_t dataLength = mChannelIndices.size() * curPacket->numSamples * mBytesPerSample;
        NscPacketHeader( 'DATA', DataType_EegData, mDataOutputFormat, dataLength ).WriteBinary( os );

        for( size_t sample = 0; sample < curPacket->numSamples; ++sample )
          for( size_t i = 0; i < mChannelIndices.size(); ++i )
        {
      #ifdef BIG_ENDIAN_INPUT // The old system provided data in big endian format:
          // We get the data in 32 bit big endian format -- convert it to machine format.
          const unsigned char* inBytes = reinterpret_cast<const unsigned char*>(
                   &curPacket->data[ sample * curPacket->numChannels + mChannelIndices[ i ] ] );
                signed int intValue = 0;
          for( int j = 0; j < 4; ++j )
          {
                  intValue <<= 8;
                  intValue |= inBytes[ j ];
          }
      #else // BIG_ENDIAN_INPUT The new system provides data in little endian format:
          signed int intValue = curPacket->data[ sample * curPacket->numChannels + mChannelIndices[ i ] ];
      #endif // BIG_ENDIAN_INPUT
          // Apply the individual gain factor for the channel (should be in the order of 1.0).
          double floatValue = intValue * mChannelGains[ i ];
          if( mDoHPFiltering )
                  ApplyHP( i, floatValue );

          switch( mDataOutputFormat )
          {
                  case DataTypeRaw16bit:
                  {
                    const double maxShort = 1 << 15 - 1;
                    if( floatValue > maxShort )
                floatValue = maxShort;
                    else if( floatValue < -maxShort )
                floatValue = -maxShort;
                    signed short shortValue = floatValue;
                    os.write( reinterpret_cast<const char*>( &shortValue ), 2 );
                  } break;

                  case DataTypeRaw32bit:
                    // Write the scaled data value into the output stream.
                    intValue = ::floor( floatValue );
                    os.write( reinterpret_cast<const char*>( &intValue ), 4 );
                    break;

                  default:
                    cerr << "Unknown data output format: " << mDataOutputFormat << endl;
                    TerminateConnection();
          }
        }
        os.flush();
      } break;
      case ACQ_MSGQ_CLOSE_COLLECTION: // We don't know how to correctly handle this message because
                                      // we can't notify a neuroscan client of a changing hardware setup.
                                      // So the best we can do is close the connection to enforce the
                                      // client to re-establish it, thereby getting its settings updated.
        TerminateConnection();
        break;
      default:
        cerr << "Unexpected value in ACQ_MessagePacket's message_type field" << endl;
        TerminateConnection();
    }
    curPacket->message_type = ACQ_MSGQ_INVALID;
    ++mPacketIndex %= ACQ_MSGQ_SIZE;
    curPacket = mpMessageQueue + mPacketIndex;
  }
  return 5;
}

void
CtfNeuroSrv::ApplyHP( int inChannel, double& ioValue )
{
  // Update the HighPass offset.
  mHPChannelOffsets[ inChannel ] *= mDecayFactor;
  mHPChannelOffsets[ inChannel ] += ( 1.0 - mDecayFactor ) * ioValue;
  // Remove the HighPass offset.
  ioValue -= mHPChannelOffsets[ inChannel ];
}
