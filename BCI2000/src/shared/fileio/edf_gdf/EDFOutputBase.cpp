////////////////////////////////////////////////////////////////////////////////
// $Id: EDFOutputBase.cpp 5384 2016-05-31 13:31:54Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class for EDF/GDF type output formats.
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
#include "EDFOutputBase.h"

#include "GDF.h"
#include "BCIStream.h"

#include <iostream>
#include <sstream>
#include <limits>
#include <cstdlib>

using namespace std;


EDFOutputBase::EDFOutputBase()
: mNumRecords( 0 )
{
}


EDFOutputBase::~EDFOutputBase()
{
}


void
EDFOutputBase::Publish() const
{
  BEGIN_PARAMETER_DEFINITIONS
    "Storage string SubjectYearOfBirth= % 1970 % % "
      "// year the subject was born",
    "Storage int SubjectSex= 0 0 0 2 "
      "// 0: not specfied, 1: male, 2: female (enumeration)",
    "Storage string EquipmentID= BCI2000 % % % "
      "// equipment provider identification",
    "Storage string TransducerType= EEG % % % "
      "// e.g. \"EEG: Ag/AgCl\"",
    "Storage string SignalUnit= uV % % % "
      "// physical unit of calibrated signal",
    "Storage stringlist ChannelNames= 0 % % % % "
      "// channel names",
  END_PARAMETER_DEFINITIONS
}


void
EDFOutputBase::Preflight( const SignalProperties&,
                          const StateVector& ) const
{
  Parameter( "TransducerType" );
  Parameter( "SignalUnit" );
  Parameter( "ChannelNames" );
  if( OptionalParameter( "FilterEnabled", 0 ) == 1 )
  {
    Parameter( "FilterHighPass" );
    Parameter( "FilterLowPass" );
  }
  if( OptionalParameter( "NotchEnabled", 0 ) == 1 )
  {
    Parameter( "NotchHighPass" );
    Parameter( "NotchLowPass" );
  }
  Parameter( "SourceChGain" );
  Parameter( "SourceChOffset" );
}


void
EDFOutputBase::Initialize( const SignalProperties& inProperties,
                           const StateVector& )
{
  mChannels.clear();
  // Enter brain signal channels into the channel list.
  int typeCode = GDF::float64::Code;
  switch( inProperties.Type() )
  {
    case SignalType::int16:
      typeCode = GDF::int16::Code;
      break;
    case SignalType::int32:
      typeCode = GDF::int32::Code;
      break;
    case SignalType::float24:
    case SignalType::float32:
      typeCode = GDF::float32::Code;
      break;
    default:
      break;
  }
  double digitalMin = inProperties.Type().Min(),
         digitalMax = inProperties.Type().Max();
  ChannelInfo channel;
  channel.TransducerType = string( Parameter( "TransducerType" ) );
  channel.PhysicalDimension = string( Parameter( "SignalUnit" ) );
  channel.PhysicalDimensionCode = GDF::PhysicalUnitToGDFUnit( channel.PhysicalDimension );
  channel.SamplesPerRecord = inProperties.Elements();
  channel.DataType = typeCode;
  channel.DigitalMinimum = digitalMin;
  channel.DigitalMaximum = digitalMax;
  const float cNaN = static_cast<float>( ::strtod( "+NAN", NULL ) );
  for( size_t i = 0; i < sizeof( channel.ElectrodePosition ) / sizeof( *channel.ElectrodePosition ); ++i )
    channel.ElectrodePosition[ i ] = cNaN;
  channel.ElectrodeImpedance = 255;
  channel.LowPass = cNaN;
  channel.HighPass = cNaN;
  channel.Notch = cNaN;
  ostringstream filtering;
  if( OptionalParameter( "FilterEnabled", 0 ) == 1 )
  {
    channel.LowPass = Parameter( "FilterLowPass" );
    channel.HighPass = Parameter( "FilterHighPass" );
    filtering << "HP:" << channel.HighPass
              << "LP:" << channel.LowPass;
  }
  else if( OptionalParameter( "FilterEnabled", 1 ) == 0 )
  {
    channel.LowPass = -1;
    channel.HighPass = -1;
  }
  if( OptionalParameter( "NotchEnabled", 0 ) == 1 )
  {
    channel.Notch = static_cast<float>( ( Parameter( "NotchHighPass" ) + Parameter( "NotchLowPass" ) ) / 2.0 );
    filtering << "N:" << channel.Notch;
  }
  else if( OptionalParameter( "NotchEnabled", 1 ) == 0 )
  {
    channel.Notch = -1;
  }
  channel.Filtering = filtering.str();
  for( int i = 0; i < inProperties.Channels(); ++i )
  {
    if( i < Parameter( "ChannelNames" )->NumValues() )
      channel.Label = string( Parameter( "ChannelNames" )( i ) );
    else
    {
      ostringstream oss;
      oss << "Ch" << i + 1;
      channel.Label = oss.str();
    }
    channel.PhysicalMinimum = Parameter( "SourceChGain" )( i )
                               * ( digitalMin + Parameter( "SourceChOffset" )( i ) );
    channel.PhysicalMaximum = Parameter( "SourceChGain" )( i )
                               * ( digitalMax + Parameter( "SourceChOffset" )( i ) );
    mChannels.push_back( channel );
  }

  // Marker channels to represent states.
  mStateNames.clear();
  ChannelInfo markerChannel;
  markerChannel.TransducerType = "Marker";
  markerChannel.PhysicalDimension = "";
  markerChannel.PhysicalDimensionCode = GDF::PhysicalUnitToGDFUnit( "" );
  markerChannel.SamplesPerRecord = inProperties.Elements();
  markerChannel.DataType = GDF::int16::Code;
  for( size_t i = 0; i < sizeof( channel.ElectrodePosition ) / sizeof( *channel.ElectrodePosition ); ++i )
    markerChannel.ElectrodePosition[ i ] = static_cast<float>( cNaN );
  markerChannel.ElectrodeImpedance = 255;
  markerChannel.LowPass = cNaN;
  markerChannel.HighPass = cNaN;
  markerChannel.Notch = cNaN;
  for( const auto& state : *States )
  {
    static string statesToIgnore[] = { "Running", "Recording" };
    bool ignoreCurrentState = false;
    for( size_t i = 0; i < sizeof( statesToIgnore ) / sizeof( *statesToIgnore ) && !ignoreCurrentState; ++i )
      ignoreCurrentState |= ( statesToIgnore[ i ] == state.Name() );
    if( !ignoreCurrentState )
    {
      double digitalMinimum = -1,
             digitalMaximum = 1 << state.Length();
      markerChannel.Label = state.Name();
      // DigitalMinimum and DigitalMaximum should be outside the range of actually
      // occurring values.
      markerChannel.DigitalMinimum = digitalMinimum;
      markerChannel.DigitalMaximum = digitalMaximum;
      markerChannel.PhysicalMinimum = digitalMinimum;
      markerChannel.PhysicalMaximum = digitalMaximum;
      Channels().push_back( markerChannel );
      mStateNames.push_back( state.Name() );
    }
  }
}


void
EDFOutputBase::StartRun( ostream&, const string& )
{
  mNumRecords = 0;
}


void
EDFOutputBase::StopRun( ostream& )
{
}


template<typename T>
void
EDFOutputBase::PutBlock( ostream& os, const GenericSignal& inSignal, const StateVector& inStatevector )
{
  for( int i = 0; i < inSignal.Channels(); ++i )
    for( int j = 0; j < inSignal.Elements(); ++j )
      GDF::Num<T>( static_cast<typename T::ValueType>( inSignal( i, j ) ) ).InsertInto( os );
  for( size_t i = 0; i < mStateNames.size(); ++i )
    for( int j = 0; j < inSignal.Elements(); ++j )
      GDF::PutField< GDF::Num<GDF::int16> >(
        os, static_cast<GDF::int16::ValueType>( inStatevector.StateValue( mStateNames[ i ], min( j, inStatevector.Samples() - 1 ) ) )
      );
}


void
EDFOutputBase::Write( ostream& os, const GenericSignal& inSignal, const StateVector& inStatevector )
{
  ++mNumRecords;
  switch( inSignal.Type() )
  {
    case SignalType::int16:
      PutBlock<GDF::int16>( os, inSignal, inStatevector );
      break;

    case SignalType::int32:
      PutBlock<GDF::int32>( os, inSignal, inStatevector );
      break;

    case SignalType::float24:
    case SignalType::float32:
      PutBlock<GDF::float32>( os, inSignal, inStatevector );
      break;

    default:
      bcierr << "Unsupported signal data type" << endl;
  }
}
