////////////////////////////////////////////////////////////////////////////////
// $Id: NeuroscanProtocol.h 4863 2015-06-25 19:21:56Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A std::iostream based implementation of the Neuroscan Acquire protocol
//   intended to be more platform independent than the original headers are.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#ifndef NEUROSCAN_H
#define NEUROSCAN_H

#ifndef DBG_LEVEL
# define DBG_LEVEL 0
#endif

#include <iostream>
#include <cstring>
#include <cfloat>
#include <stdint.h>

enum
{
  HeaderIdCtrl = 0x4354524c, // 'CTRL'
  HeaderIdData = 0x44415441, // 'DATA'
  HeaderIdFile = 0x46494c45, // 'FILE'

  GeneralControlCode = 1,
    RequestForVersion = 1,
    ClosingUp,
    ServerDisconnected,
  ServerControlCode = 2,
    StartAcquisition = 1,
    StopAcquisition,
    StartImpedance,
    ChangeSetup,
    DCCorrection,
  ClientControlCode = 3,
    RequestEDFHeader = 1,
    RequestAstFile,
    RequestStartData,
    RequestStopData,
    RequestBasicInfo,

  SetupFile = 1,
    NeuroscanASTFormat = 1,
    CtfDSFormat = 0x43544644, // 'CTFD', BCI2000-specific extension

  DataType_InfoBlock = 1,
    InfoType_Version = 1,
    InfoType_EdfHeader,
    InfoType_BasicInfo,

  DataType_EegData = 2,
    DataTypeRaw16bit = 1,
    DataTypeRaw32bit,
};

// The neuroscan acquire protocol actually mixes big and little endian data
// arrangement on the network.
namespace BigEndianData
{
  template<typename T>
  static void put( std::ostream& os, T t )
  {
    for( int i = sizeof( T ) - 1; i >= 0; --i )
      os.put( ( t >> ( i * 8 ) ) & 0xff );
  }
  template<>
  inline void put( std::ostream& os, float f )
  {
    union { const float* f; const uint32_t* i; } data = { &f };
    put( os, *data.i );
  }

  template<typename T>
  static void get( std::istream& is, T& t )
  {
    t = 0;
    for( size_t i = 0; i < sizeof( T ); ++i )
    {
      uint8_t c = is.get();
      t <<= 8;
      t |= c;
    }
  }
  template<>
  inline void get( std::istream& is, float& f )
  {
    union { float* f; uint32_t* i; } data = { &f };
    get( is, *data.i );
  }
};

namespace LittleEndianData
{
  template<typename T>
  static void put( std::ostream& os, T t )
  {
    for( size_t i = 0; i < sizeof( T ); ++i )
    {
      os.put( t & 0xff );
      t >>= 8;
    }
  }
  template<>
  inline void put( std::ostream& os, float f )
  {
    union { const float* f; const uint32_t* i; } data = { &f };
    put( os, *data.i );
  }

  template<typename T>
  static void get( std::istream& is, T& t )
  {
    t = 0;
    for( size_t i = 0; i < sizeof( T ); ++i )
    {
      uint8_t c = is.get();
      t |= T( c ) << ( i * 8 );
    }
  }
  template<>
  inline void get( std::istream& is, float& f )
  {
    union { float* f; uint32_t* i; } data = { &f };
    get( is, *data.i );
  }
};

class NscPacketHeader
{
  public:
    NscPacketHeader()
    : mId( 0 ),
      mCode( 0 ),
      mValue( 0 ),
      mDataSize( 0 )
    {
    };
    NscPacketHeader( int id, int code, int value, size_t dataSize = 0 )
    : mId( id ),
      mCode( code ),
      mValue( value ),
      mDataSize( static_cast<int32_t>( dataSize ) )
    {
    };
    bool operator<( const NscPacketHeader& n ) const
    {
      return ::memcmp( this, &n, sizeof( *this ) ) < 0;
    };
    int Id() const { return mId; }
    int Code() const { return mCode; }
    int Value() const { return mValue; }
    size_t DataSize() const { return mDataSize; }

    std::istream& Unserialize( std::istream& is )
    { // The packet header is transmitted in big endian format.
      BigEndianData::get( is, mId );
      BigEndianData::get( is, mCode );
      BigEndianData::get( is, mValue );
      BigEndianData::get( is, mDataSize );
#if( DBG_LEVEL > 0 )
      std::cout << "-> ";
      InsertInto( std::cout );
      std::cout << std::endl;
#endif
      return is;
    }
    std::ostream& Serialize( std::ostream& os ) const
    { // The packet header is transmitted in big endian format.
      BigEndianData::put( os, mId );
      BigEndianData::put( os, mCode );
      BigEndianData::put( os, mValue );
      BigEndianData::put( os, mDataSize );
#if( DBG_LEVEL > 0 )
# if( DBG_LEVEL < 2 )
      if( mId != HeaderIdData && mCode != DataType_EegData )
# endif
      {
        std::cout << "<- ";
        InsertInto( std::cout );
        std::cout << std::endl;
      }
#endif
      return os;
    }
    std::istream& ExtractFrom( std::istream& is )
    {
      BigEndianData::get( is, mId );
      char delim;
      return is >> delim >> mCode >> delim >> mValue >> delim >> mDataSize;
    }

#define CONSIDER(x) case x: match = true; os << #x " : ";
    std::ostream& InsertInto( std::ostream& os ) const
    {
      BigEndianData::put( os, mId );
      os << " : ";
      bool match = false;
      switch( mId )
      {
        case HeaderIdCtrl:
          switch( mCode )
          {
            CONSIDER( GeneralControlCode )
            switch( mValue )
            {
              CONSIDER( RequestForVersion ) break;
              CONSIDER( ClosingUp ) break;
              CONSIDER( ServerDisconnected ) break;
            }
            break;
            CONSIDER( ServerControlCode )
            switch( mValue )
            {
              CONSIDER( StartAcquisition ) break;
              CONSIDER( StopAcquisition ) break;
              CONSIDER( StartImpedance ) break;
              CONSIDER( ChangeSetup ) break;
              CONSIDER( DCCorrection ) break;
            }
            break;
            CONSIDER( ClientControlCode )
            switch( mValue )
            {
              CONSIDER( RequestEDFHeader ) break;
              CONSIDER( RequestAstFile ) break;
              CONSIDER( RequestStartData ) break;
              CONSIDER( RequestStopData ) break;
              CONSIDER( RequestBasicInfo ) break;
            }
            break;
          }
      break;

      case HeaderIdFile:
        switch( mCode )
        {
          CONSIDER( SetupFile )
          switch( mValue )
          {
            CONSIDER( NeuroscanASTFormat ) break;
            CONSIDER( CtfDSFormat ) break;
          }
          break;
        }
        break;

      case HeaderIdData:
        switch( mCode )
        {
          CONSIDER( DataType_InfoBlock )
          switch( mValue )
          {
            CONSIDER( InfoType_Version ) break;
            CONSIDER( InfoType_EdfHeader ) break;
            CONSIDER( InfoType_BasicInfo ) break;
          }
          break;
          CONSIDER( DataType_EegData )
          switch( mValue )
          {
            CONSIDER( DataTypeRaw16bit ) break;
            CONSIDER( DataTypeRaw32bit ) break;
          }
          break;
        }
        break;
      }
      if( !match )
        os << mCode << ':' << mValue << ':';
      return os << mDataSize << " data bytes";
    }
#undef CONSIDER

  private:
    int32_t mId;
    int16_t mCode,
            mValue;
    int32_t mDataSize;
};

inline
std::ostream&
operator<<( std::ostream& os, const NscPacketHeader& h )
{
  return h.InsertInto( os );
}

inline
NscPacketHeader NscCloseRequest()
{
  return NscPacketHeader( HeaderIdCtrl, GeneralControlCode, ClosingUp );
}

inline
NscPacketHeader NscStartAcquisition()
{
  return NscPacketHeader( HeaderIdCtrl, ServerControlCode, StartAcquisition );
}

inline
NscPacketHeader NscStopAcquisition()
{
  return NscPacketHeader( HeaderIdCtrl, ServerControlCode, StopAcquisition );
}

inline
NscPacketHeader NscVersionRequest()
{
  return NscPacketHeader( HeaderIdCtrl, GeneralControlCode, RequestForVersion );
}

inline
NscPacketHeader NscInfoRequest()
{
  return NscPacketHeader( HeaderIdCtrl, ClientControlCode, RequestBasicInfo );
}

inline
NscPacketHeader NscEDFHeaderRequest()
{
  return NscPacketHeader( HeaderIdCtrl, ClientControlCode, RequestEDFHeader );
}

inline
NscPacketHeader NscSetupFileRequest()
{
  return NscPacketHeader( HeaderIdCtrl, ClientControlCode, RequestAstFile );
}

inline
NscPacketHeader NscStartDataRequest()
{
  return NscPacketHeader( HeaderIdCtrl, ClientControlCode, RequestStartData );
}

inline
NscPacketHeader NscStopDataRequest()
{
  return NscPacketHeader( HeaderIdCtrl, ClientControlCode, RequestStopData );
}

// NscBasicInfo class
class NscBasicInfo
{
  static const int cDataSize = 7 * 4;
  public:
    NscBasicInfo()
    : mSizeField( cDataSize ),
      mEEGChannels( 0 ),
      mEventChannels( 0 ),
      mSamplesInBlock( 0 ),
      mSamplingRate( 0 ),
      mDataDepth( 0 ),
      mResolution( 1.0 )
    {
    }
    NscBasicInfo( int EEGChannels, int eventChannels, int samplesInBlock,
                  int samplingRate, int dataDepth, float resolution )
    : mSizeField( cDataSize ),
      mEEGChannels( EEGChannels ),
      mEventChannels( eventChannels ),
      mSamplesInBlock( samplesInBlock ),
      mSamplingRate( samplingRate ),
      mDataDepth( dataDepth ),
      mResolution( resolution )
    {
    }

    int   EEGChannels() const    { return mEEGChannels; }
    int   EventChannels() const  { return mEventChannels; }
    int   SamplesInBlock() const { return mSamplesInBlock; }
    int   SamplingRate() const   { return mSamplingRate; }
    int   DataDepth() const      { return mDataDepth; }
    float Resolution() const     { return mResolution; }

    std::ostream& Serialize( std::ostream& os ) const
    { // Packet data are transmitted in little endian format.
      LittleEndianData::put( os, mSizeField );
      LittleEndianData::put( os, mEEGChannels );
      LittleEndianData::put( os, mEventChannels );
      LittleEndianData::put( os, mSamplesInBlock );
      LittleEndianData::put( os, mSamplingRate );
      LittleEndianData::put( os, mDataDepth );
      LittleEndianData::put( os, mResolution );
      return os;
    }
    std::istream& Unserialize( std::istream& is )
    { // Packet data are transmitted in little endian format.
      LittleEndianData::get( is, mSizeField );
      LittleEndianData::get( is, mEEGChannels );
      LittleEndianData::get( is, mEventChannels );
      LittleEndianData::get( is, mSamplesInBlock );
      LittleEndianData::get( is, mSamplingRate );
      LittleEndianData::get( is, mDataDepth );
      LittleEndianData::get( is, mResolution );
      return is;
    }
    std::ostream& InsertInto( std::ostream& os ) const
    {
      return os << mEEGChannels    << " Signal channels, \t"
                << mEventChannels  << " Event channels, \t"
                << mSamplesInBlock << " Samples per block, \t"
                << mSamplingRate   << " Samples per second, \t"
                << mDataDepth      << " Bytes per sample, \t"
                << mResolution     << " Physical units per LSB\n";
    }
  private:
    int32_t   mSizeField,      // Size of structure, used for version control
              mEEGChannels,    // Number of EEG channels
              mEventChannels,  // Number of event channels
              mSamplesInBlock, // Samples in block
              mSamplingRate,   // Sampling rate (in Hz)
              mDataDepth;      // 2 for "short", 4 for "int" type of data
    float     mResolution;     // Resolution for LSB
};

inline
std::ostream&
operator<<( std::ostream& os, const NscBasicInfo& h )
{
  return h.InsertInto( os );
}

#endif // NEUROSCAN_H

