////////////////////////////////////////////////////////////////////
// $Id: bci_stream2mat.cpp 5414 2016-07-01 19:45:56Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: See the ToolInfo definition below.
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
////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cstdio>
#include <ctime>
#include <set>

#include "bci_tool.h"
#include "Param.h"
#include "ParamList.h"
#include "State.h"
#include "StateList.h"
#include "StateVector.h"
#include "GenericVisualization.h"
#include "MessageChannel.h"
#include "BCIStream.h"
#include "Files.h"

using namespace std;

string ToolInfo[] =
{
  "bci_stream2mat",
  "Convert a binary BCI2000 stream into a matlab .mat file",
  "Reads a BCI2000 compliant binary stream from standard input, "
    "and writes it to stdout in matlab level 5 MAT-file format.",
  "binary",
  ""
};

class StreamToMat : public MessageChannel
{
  // Matlab file format related constants.
  enum
  {
    matTextHeaderLength = 116,
    matVersionInfoOffset = 124,
    matPadding = 8, // padding to 64 bit boundaries

    miMATRIX = 14,

    miINT8 = 1,
    miINT32 = 5,
    miUINT8 = 2,
    miUINT32 = 6,
    miSINGLE = 7,
    miUTF16 = 17,

    mxCELL_CLASS   = 1,
    mxSTRUCT_CLASS = 2,
    mxCHAR_CLASS   = 4,
    mxDOUBLE_CLASS = 6,
    mxSINGLE_CLASS = 7,
    mxUINT32_CLASS = 13,
  };

 public:
  StreamToMat( Streambuf& in, Streambuf& out )
  : MessageChannel( in ), mOutput( out ),
    mpStatevector( NULL ), mSignalProperties( 0, 0 ),
    mDataElementSizePos( 0 ), mDataColsPos( 0 ), mDataSizePos( 0 ), mDataCols( 0 ),
    mParamsDumped( false ) {}
  ~StreamToMat() { delete mpStatevector; }
  void FinishHeader();

 private:
  OStream mOutput;

  stringstream        mParamsOut;
  bool                mParamsDumped;
  StateList           mStatelist;
  StateVector*        mpStatevector;
  SignalProperties    mSignalProperties;
  typedef set<string> StringSet; // A set is a sorted container of unique values.
  StringSet           mStateNames;
  size_t              mDataCols;
  streamoff           mDataElementSizePos,
                      mDataColsPos,
                      mDataSizePos;

  ostream& Output() { return mOutput; }
  streamoff BeginVar(uint32_t flags);
  void FinishVar(streamoff sizePos);
  void WriteDims(uint32_t nRows, uint32_t nCols);
  void WriteName(const string& name);
  void WriteString(const string& name, const string& str);

  void WriteHeader();
  void WriteData( const GenericSignal& );
  void Write16( uint16_t value )
  { Output().write( reinterpret_cast<const char*>( &value ), sizeof( value ) ); }
  void Write32( uint32_t value )
  { Output().write( reinterpret_cast<const char*>( &value ), sizeof( value ) ); }
  void WriteFloat32( float32_t value )
  { Output().write( reinterpret_cast<const char*>( &value ), sizeof( value ) ); }
  void Pad();

  virtual bool OnState( istream& ) override;
  virtual bool OnVisSignal( istream& ) override;
  virtual bool OnVisSignalProperties( istream& ) override;
  virtual bool OnStateVector( istream& ) override;
  virtual bool OnParam( istream& ) override;
};

ToolResult
ToolInit()
{
  return noError;
}

ToolResult
ToolMain( Streambuf& in, Streambuf& out, OptionSet& arOptions )
{
  if( arOptions.size() > 1 )
    return illegalOption;
  StreamToMat converter( in, out );
  while( converter.HandleMessage() )
    ;
  converter.FinishHeader();
  if( in.Input()->Failed() )
    return illegalInput;
  return noError;
}

void
StreamToMat::Pad()
{
  for( streamoff i = Output().tellp(); i % matPadding; ++i )
    Output().put( 0 );
}


streamoff
StreamToMat::BeginVar(uint32_t flags)
{
  Write32( miMATRIX );
  streamoff sizePos = Output().tellp();
  Write32( 0 ); // placeholder for size
  Write32( miUINT32 ); Write32( 8 ); // flags are coming
  Write32( flags ); Write32( 0 ); // here are the flags
  return sizePos;
}

void
StreamToMat::FinishVar(streamoff sizePos)
{ // bounce back to sizePos, write the number of bytes between here and where you just came from - 4, bounce back to where you came from
  streamoff endPos = Output().tellp();
  Output().seekp( sizePos );
  Write32( static_cast<uint32_t>( endPos - sizePos - 4 ) );
  Output().seekp( endPos );
}

void
StreamToMat::WriteDims(uint32_t nRows, uint32_t nCols)
{
  Write32( miINT32 ); Write32( 8 );
  Write32( nRows ); Write32( nCols );
}

void
StreamToMat::WriteName(const string& name)
{
  Write32( static_cast<uint32_t>( miINT8 ) ); Write32( static_cast<uint32_t>( name.size() ) );
  if( name.size() )
  {
    Output() << name;
    Pad();
  }
}

void
StreamToMat::WriteString(const string& name, const string& str)
{
  streamoff sizePos = BeginVar( mxCHAR_CLASS );
  WriteDims( 1, static_cast<int>( str.size() ) );
  WriteName( name );
  Write32( miUTF16 );
  Write32( static_cast<uint32_t>( 2 * str.size() ) );
  for( size_t j = 0; j < str.size(); j++ ) Write16( str[j] );
  Pad();
  FinishVar( sizePos );
}

void
StreamToMat::WriteHeader()
{
  time_t timer = ::time( NULL );
  Output() << "MATLAB 5.0 MAT-file created "
        << ::ctime( &timer )
        << " by "
        << ToolInfo[ name ];
  for( streamoff i = Output().tellp(); i < matTextHeaderLength; ++i )
    Output() << ' ';
  Output().seekp( matTextHeaderLength );
  for( streamoff i = Output().tellp(); i < matVersionInfoOffset; ++i )
    Output() << ' ';
  Write16( 0x0100 ); Write16( 'MI' );
  // Write a matlab structure containing arrays with state names
  // pointing to the associated columns.
  streamoff indexSizePos = BeginVar( mxSTRUCT_CLASS );
  WriteDims( 1, 1 );
  WriteName( "Index" );

  // Field name length
  const char signalName[] = "Signal";
  size_t fieldNameLength = sizeof( signalName ) - 1;
  for( StringSet::const_iterator i = mStateNames.begin(); i != mStateNames.end(); ++i )
    if( i->length() > fieldNameLength )
      fieldNameLength = i->length();
  fieldNameLength = ( fieldNameLength / matPadding + 1 ) * matPadding;
  Write32( static_cast<uint32_t>( 4 << 16 | miINT32 ) );
  Write32( static_cast<uint32_t>( fieldNameLength ) );
  // Field names
  Write32( static_cast<uint32_t>( miINT8 ) );
  Write32( static_cast<uint32_t>( fieldNameLength * ( mStateNames.size() + 1 ) ) );
  for( StringSet::const_iterator i = mStateNames.begin(); i != mStateNames.end(); ++i )
  {
    Output() << *i;
    for( size_t j = i->length(); j < fieldNameLength; ++j )
      Output().put( '\0' );
  }
  Output() << signalName;
  for( size_t j = sizeof( signalName ) - 1; j < fieldNameLength; ++j )
    Output().put( '\0' );
  // Fields
  // 1x1 arrays holding the states' column indices
  for( size_t i = 1; i <= mStateNames.size(); ++i )
  {
    streamoff sizePos = BeginVar( mxUINT32_CLASS );
    WriteDims( 1, 1 );
    WriteName( "" );
    // Array data
    Write32( static_cast<uint32_t>( 4 << 16 | miUINT32 ) );
    Write32( static_cast<uint32_t>( i ) );

    FinishVar( sizePos );
  }
  // An array with the signal's dimensions holding the signal entries' row indices
  long numSignalEntries = mSignalProperties.Channels() * mSignalProperties.Elements();
  {
    streamoff sizePos = BeginVar( mxUINT32_CLASS );
    WriteDims( mSignalProperties.Channels(), mSignalProperties.Elements() );
    WriteName( "" );
    // Array data
    Write32( miUINT32 ); Write32( 4 * numSignalEntries );
    for( int j = 0; j < mSignalProperties.Elements(); ++j )
      for( int i = 0; i < mSignalProperties.Channels(); ++i )
        Write32( static_cast<unsigned int>( mStateNames.size() + 1 + i * mSignalProperties.Elements() + j ) );
    FinishVar( sizePos );
    Pad();
  }
  FinishVar( indexSizePos );

  // A cell array of channel labels
  streamoff channelLabelsSize = BeginVar( mxCELL_CLASS );
  WriteDims( mSignalProperties.Channels(), 1 );
  WriteName( "ChannelLabels" );
  for( int i = 0; i < mSignalProperties.Channels(); i++ )
    WriteString( "", mSignalProperties.ChannelLabels()[i] );
  FinishVar( channelLabelsSize );

  // A cell array of element labels
  streamoff elementLabelsSize = BeginVar( mxCELL_CLASS );
  WriteDims( mSignalProperties.Elements(), 1 );
  WriteName( "ElementLabels" );
  for( int i = 0; i < mSignalProperties.Elements(); i++ )
    WriteString( "", mSignalProperties.ElementLabels()[i] );
  FinishVar( elementLabelsSize );

  // A single-precision numeric array of element values
  streamoff elementValuesSize = BeginVar( mxSINGLE_CLASS );
  WriteDims( mSignalProperties.Elements(), 1 );
  WriteName( "ElementValues" );
  Write32( miSINGLE ); Write32( 4 * mSignalProperties.Elements() );
  for( int i = 0; i < mSignalProperties.Elements(); i++ )
  {
    float val = static_cast<float>( ( i - mSignalProperties.ElementUnit().Offset() ) * mSignalProperties.ElementUnit().Gain() );
    WriteFloat32(val);
  }
  Pad();
  FinishVar( elementValuesSize );

  // A string denoting the element unit
  WriteString( "ElementUnit", mSignalProperties.ElementUnit().Symbol());

  // A long string containing the prm text
  WriteString( "Parms", mParamsOut.str() );
  mParamsDumped = true;

  // A single-precision numeric array that holds the signal.
  mDataElementSizePos = BeginVar( mxSINGLE_CLASS );
  WriteDims( static_cast<uint32_t>( mStateNames.size() + numSignalEntries ), 0 );
  mDataColsPos = Output().tellp(); mDataColsPos -= 4;
  WriteName( "Data" );
  Write32( miSINGLE );
  mDataSizePos = Output().tellp();
  Write32( 0 );
}

void
StreamToMat::WriteData( const GenericSignal& s )
{
  if( mpStatevector == NULL )
    for( size_t i = 0; i < mStateNames.size(); ++i )
      WriteFloat32( 0 );
  else
    for( StringSet::const_iterator i = mStateNames.begin(); i != mStateNames.end(); ++i )
      WriteFloat32( static_cast<float32_t>( mpStatevector->StateValue( *i ) ) );

  for( int i = 0; i < s.Channels(); ++i )
    for( int j = 0; j < s.Elements(); ++j )
      WriteFloat32( static_cast<float32_t>( s( i, j ) ) );
  ++mDataCols;
}

void
StreamToMat::FinishHeader()
{
  FinishVar( mDataSizePos );
  Pad();
  FinishVar( mDataElementSizePos );

  streamoff endPos = Output().tellp();
  Output().seekp( mDataColsPos );
  Write32( static_cast<unsigned int>( mDataCols ) );
  Output().seekp( endPos );
}

bool
StreamToMat::OnState( istream& arIn )
{
  State s;
  s.Unserialize( arIn );
  if( arIn )
  {
    mStatelist.Delete( s.Name() );
    mStatelist.Add( s );
    if( mpStatevector )
    {
      delete mpStatevector;
      mpStatevector = nullptr;
    }
  }
  return true;
}

bool
StreamToMat::OnVisSignal( istream& arIn )
{
  VisSignal v;
  v.Unserialize( arIn );
  const GenericSignal& s = v;
  if( mSignalProperties.IsEmpty() )
    bcierr << "Internal error: HandleVisSignalProperties should have written the header already, but has not" << endl;
  if( s.Properties() != mSignalProperties )
    bcierr << "Ignored signal with inconsistent properties" << endl;
  else
    WriteData( s );
  return true;
}


bool
StreamToMat::OnVisSignalProperties( istream& arIn )
{
  VisSignalProperties vsp;
  vsp.Unserialize( arIn );
  mSignalProperties = vsp.SignalProperties();

  // Write a header first.
  mStateNames.clear();
  for( const auto& s : mStatelist )
    if( s.Kind() != State::Padding && !s.Name().empty() && ::isalpha( s.Name().front() ) )
      mStateNames.insert( s.Name() );
  WriteHeader();

  return true;
}


bool
StreamToMat::OnStateVector( istream& arIn )
{
  if( mpStatevector == NULL )
    mpStatevector = new StateVector( mStatelist );
  mpStatevector->Unserialize( arIn );
  return true;
}

bool
StreamToMat::OnParam( istream& arIn )
{
  Param p;
  if( p.Unserialize( arIn ) )
  {
    if( mParamsDumped )
      bciout << "additional parameter arrived after parameters were written to mat-file";
    else
      mParamsOut << p << "\n";
  }
  return true;
}
