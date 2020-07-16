////////////////////////////////////////////////////////////////////
// $Id: bci_dat2stream.cpp 5414 2016-07-01 19:45:56Z mellinger $
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
#include "bci_tool.h"
#include "MeasurementUnits.h"
#include "Param.h"
#include "ParamList.h"
#include "State.h"
#include "StateList.h"
#include "StateVector.h"
#include "GenericSignal.h"
#include "MessageChannel.h"
#include <fstream>
#include <cassert>

using namespace std;

string ToolInfo[] =
{
  "bci_dat2stream",
  "Convert a BCI2000 data file into a BCI2000 stream.",
  "Reads a BCI2000 data file (*.dat) compliant stream from "
    "standard input and writes it to the standard output as a BCI2000 "
    "compliant binary stream.",
  "binary",
  "-t,       --transmit={spd}      Select States, Parameters, and/or",
  "                                Data for transmission",
  "-r,       --raw                 Transmit uncalibrated data",
  "-p<file>, --parameters=<file>   Incorporate parameters from named file",
  "-s<time>, --start=<time>        Start at a given offset within the file",
  "-d<time>, --duration=<time>     Transmit only a limited amount of signal",
  " ",
  "Amounts of <time> are expressed in SampleBlocks or (if the unit is",
  "explicitly appended) as a number of seconds or milliseconds that",
  "corresponds to a whole number of SampleBlocks.",
  ""
};

ToolResult ToolInit()
{
  return noError;
}

ToolResult ToolMain( Streambuf& inbuf, Streambuf& outbuf, OptionSet& options )
{
  istream in( &inbuf );

  ToolResult result = noError;
  string transmissionList = options.getopt( "-t|-T|--transmit", "spd" );
  bool transmitStates = ( transmissionList.find_first_of( "sS" ) != string::npos ),
       transmitParameters = ( transmissionList.find_first_of( "pP" ) != string::npos ),
       transmitData = ( transmissionList.find_first_of( "dD" ) != string::npos ),
       calibrateData = !options.findopt( "-r|-R|--raw" );
  string paramFileName = options.getopt( "-p|-P|--parameters", "" );
  string offsetString = options.getopt( "-s|-S|--start", "" );
  string durationString = options.getopt( "-d|-D|--duration", "" );

  // Read the BCI2000 header.
  string token;
  int headerLength,
      sourceCh,
      stateVectorLength;
  SignalType dataFormat;
  StateList states;
  enum { v10, v11 } fileFormatVersion = v10;

  bool legalInput = !!(in >> token);
  if( legalInput && token == "BCI2000V=" )
  {
    legalInput = legalInput &&
      in >> token;
    if( token == "1.1" )
      fileFormatVersion = v11;
    else
      legalInput = false;
    legalInput = legalInput &&
      in >> token;
  }
  legalInput &=
    token == "HeaderLen=" && in >> headerLength &&
    in >> token && token == "SourceCh=" && in >> sourceCh &&
    in >> token && token == "StatevectorLen=" && in >> stateVectorLength;
  switch( fileFormatVersion )
  {
    case v10:
      dataFormat = SignalType::int16;
      break;
    case v11:
      legalInput &=
        in >> token && token == "DataFormat=" && in >> dataFormat;
      break;
    default:
      assert( false );
  }
  legalInput &=
    getline( in >> ws, token, ']' ) >> ws && token == "[ State Vector Definition ";
  while( legalInput && in.peek() != '[' && getline( in, token ) )
  {
    istringstream is( token );
    State state;
    if( is >> state )
    {
      states.Delete( state.Name() );
      states.Add( state );
    }
    legalInput = legalInput && is;
  }

  MessageChannel output( outbuf );
  if( transmitStates )
  { // Transmit states ordered by name, i.e. independently of their order in the file.
    vector<string> stateNames;
    for( const auto& state : states )
      stateNames.push_back( state.Name() );
    sort( stateNames.begin(), stateNames.end(), State::NameCmp() );
    for( const auto& name : stateNames )
      output.Send( states.ByName( name ) );
  }

  legalInput &=
    getline( in >> ws, token, ']' ) >> ws && token == "[ Parameter Definition ";

  ParamList parameters;
  while( legalInput && getline( in, token ) &&  token.length() > 1 )
  {
    istringstream is( token );
    Param param;
    if( is >> param )
      parameters[ param.Name() ] = param;
    legalInput = legalInput && is;
    if( transmitParameters )
      output.Send( param );
  }
  if( paramFileName.size() )
  {
    ifstream pIn( paramFileName.c_str(), ifstream::in );
    while( pIn.good() && getline( pIn, token ) &&  token.length() > 1 )
    {
      istringstream is( token );
      Param param;
      if( is >> param )
        parameters[ param.Name() ] = param;
      if( transmitParameters )
        output.Send( param );
    }
  }
  MeasurementUnits::Initialize( parameters );
  double   offset = (   offsetString.size() ? MeasurementUnits::TimeInSampleBlocks(   offsetString ) :  0.0 );
  double duration = ( durationString.size() ? MeasurementUnits::TimeInSampleBlocks( durationString ) : -1.0 );

  int sampleBlockSize = 0;
  if( legalInput )
    sampleBlockSize = static_cast<int>( PhysicalUnit()
                                        .SetGain( 1.0 ).SetOffset( 0.0 ).SetSymbol( "" )
                                        .PhysicalToRaw( parameters[ "SampleBlockSize" ].Value() )
                                       );
  legalInput &= ( sampleBlockSize > 0 );
  if( !legalInput )
  {
    cerr << "Illegal header format or content" << endl;
    return illegalInput;
  }

  SignalProperties inputProperties( sourceCh, sampleBlockSize, dataFormat );
  if( transmitData || transmitStates )
  {
    StateVector statevector( states, sampleBlockSize );
    if( statevector.Length() != stateVectorLength )
    {
      cerr << "Statevector's length (" << statevector.Length() << ")"
           << " differs from StateVectorLen field (" << stateVectorLength << ")"
           << endl;
      return illegalInput;
    }
    if( calibrateData && parameters.Exists( "SamplingRate" ) )
    {
      double samplingRate = PhysicalUnit()
                           .SetOffset( 0.0 )
                           .SetGain( 1.0 )
                           .SetSymbol( "Hz" )
                           .PhysicalToRaw( parameters[ "SamplingRate" ].Value() );
      inputProperties
        .ElementUnit().SetOffset( 0 )
                      .SetGain( 1.0 / samplingRate )
                      .SetSymbol( "s" );
    }
    vector<double> offsets( sourceCh, 0 ),
                   gains( sourceCh, 1 );
    if( calibrateData && parameters.Exists( "SourceChOffset" ) )
    {
      const Param& sourceChOffset = parameters[ "SourceChOffset" ];
      for( int ch = 0; ch < min( sourceCh, sourceChOffset.NumValues() ); ++ch )
        offsets[ ch ] = ::atof( sourceChOffset.Value( ch ).c_str() );
    }
    if( calibrateData && parameters.Exists( "SourceChGain" ) )
    {
      const Param& sourceChGain = parameters[ "SourceChGain" ];
      for( int ch = 0; ch < min( sourceCh, sourceChGain.NumValues() ); ++ch )
        gains[ ch ] = ::atof( sourceChGain.Value( ch ).c_str() );
      inputProperties
        .ValueUnit().SetOffset( 0 )
        .SetGain( 1e-6 )
        .SetSymbol( "V" );
    }
    if( parameters.Exists( "ChannelNames" ) && parameters[ "ChannelNames" ].NumValues() > 0 )
    {
      LabelIndex& outputLabels = inputProperties.ChannelLabels();
      for( int i = 0; i < min( inputProperties.Channels(), parameters[ "ChannelNames" ].NumValues() ); ++i )
        outputLabels[ i ] = parameters[ "ChannelNames" ].Value( i ).c_str();
    }
    if( transmitData )
    {
      if( calibrateData )
      {
        SignalProperties outputProperties( inputProperties );
        outputProperties.SetType( SignalType::float32 );
        output.Send( outputProperties );
      }
      else
        output.Send( inputProperties );
    }

    int curSample = 0;
    int nBlocksRead = 0, nBlocksTransmitted = 0;
    GenericSignal inputSignal( inputProperties );
    while( in && in.peek() != EOF && ( duration < 0.0 || nBlocksTransmitted < duration ) )
    {
      for( int i = 0; i < sourceCh; ++i )
        inputSignal.ReadValueBinary( in, i, curSample );
      in.read( statevector.Data( curSample ), statevector.Length() );

      if( ++curSample == sampleBlockSize )
      {
        curSample = 0;
        if ( ++nBlocksRead > offset )
        {
          if( transmitStates )
          {
            output.Send( statevector );
          }
          if( transmitData )
          {
            if( calibrateData )
            {
              SignalProperties outputProperties( inputProperties );
              outputProperties.SetType( SignalType::float32 );
              GenericSignal outputSignal( outputProperties );
              for( int i = 0; i < sourceCh; ++i )
                for( int j = 0; j < sampleBlockSize; ++j )
                  outputSignal( i, j )
                    = ( inputSignal( i, j ) - offsets[ i ] ) * gains[ i ];
              // Send the data.
              output.Send( outputSignal );
            }
            else
              output.Send( inputSignal );
          }
          nBlocksTransmitted++;
        }
      }
    }
    if( curSample != 0 )
    {
      cerr << "Non-integer number of data blocks in input" << endl;
      result = illegalInput;
    }
  }
  return result;
}
