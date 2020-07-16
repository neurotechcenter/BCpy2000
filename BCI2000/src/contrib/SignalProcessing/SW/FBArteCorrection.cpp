////////////////////////////////////////////////////////////////////////////////
//
// File: FBArteCorrection.cpp
//
// Description: Slow Wave Class Definition
//              written by Dr. Thilo Hinterberger 2000-2001
//              Copyright University of Tuebingen, Germany
//
// Changes:     2003, juergen.mellinger@uni-tuebingen.de: some bugs fixed.
//              Feb 8, 2004, jm: Adaptations to changes in BCI2000 framework,
//              minor reformulations, reformatting.
//              Feb 24, 2004, jm: Moved the TFBArteCorrection class into
//              separate files.
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
#include "FBArteCorrection.h"

#include "BCIStream.h"
#include <cmath>

#define SECTION "Filtering"

using namespace std;

RegisterFilter( TFBArteCorrection, 2.D2 );

TFBArteCorrection::TFBArteCorrection()
: mArteMode( off )
{
  BEGIN_PARAMETER_DEFINITIONS
    SECTION " intlist ArteChList= 2 2 0"
      " 2 0 0 // Association of artefact channels with signal channels, 0 for no artefact channel",
    SECTION " floatlist ArteFactorList= 2 0.15 0"
      " 0 0 0 // Influence of artefact channel on input channel, -1: no artifact channel",
    SECTION " int ArteMode= 0"
      " 1 0 3 // Artefact correction mode: "
        "0 off, "
        "1 linear subtraction, "
        "2 subtraction if supporting, "
        "3 subtraction w/abort "
        "(enumeration)",
  END_PARAMETER_DEFINITIONS

  BEGIN_STATE_DEFINITIONS
    "Artifact 1 0 0 0",
  END_STATE_DEFINITIONS
}

void
TFBArteCorrection::Preflight( const SignalProperties& inSignalProperties,
                                    SignalProperties& outSignalProperties ) const
{
  for( int i = 0; i < Parameter( "ArteChList" )->NumValues(); ++i )
  {
    PreflightCondition( Parameter( "ArteChList" )( i ) >= 0 );
    PreflightCondition( Parameter( "ArteChList" )( i ) <= inSignalProperties.Channels() );
  }
  outSignalProperties = inSignalProperties;
  outSignalProperties.SetName( "Artifact Filtered" );
}

void
TFBArteCorrection::Initialize( const SignalProperties&, const SignalProperties& )
{
  mArteChList.clear();
  for( int i = 0; i < Parameter( "ArteChList" )->NumValues(); ++i )
    mArteChList.push_back( static_cast<int>( Parameter( "ArteChList" )( i ) ) - 1 );

  mArteFactorList.clear();
  for( int i = 0; i < Parameter( "ArteFactorList" )->NumValues(); ++i )
    mArteFactorList.push_back( Parameter( "ArteFactorList" )( i ) );

  mArteMode = static_cast<ArteMode>( ( int )Parameter( "ArteMode" ) );
}

void
TFBArteCorrection::Process( const GenericSignal& inSignal, GenericSignal& outSignal )
{
  outSignal = inSignal;
  GenericSignal& ioSignal = outSignal;

  for( size_t i = 0; i < mArteChList.size(); ++i )
    if( mArteChList[ i ] >= 0 )
      for( int j = 0; j < ioSignal.Elements(); ++j )
      {
        double controlSignal = ioSignal( i, j ),
               arteSignal = ioSignal( mArteChList[ i ], j ) * mArteFactorList[ i ];

        switch( mArteMode )
        {
          case off:
            break;

          case linearSubtraction:
            // Linear subtraction.
            ioSignal( i, j ) = controlSignal - arteSignal;
            break;

          case subtractionIfSupporting:
          case subtractionWithAbort:
            if( arteSignal * controlSignal > 0 )
            { // If they have same signs:
              if( ::fabs( arteSignal ) < ::fabs( controlSignal ) )
              // If artefact is not too big, correct the signal.
                ioSignal( i, j ) = controlSignal - arteSignal;
              else
              { // Artefact is too big.
                ioSignal( i, j ) = 0; // FB is supressed.(?)
                if( mArteMode == subtractionWithAbort )
                  State( "Artifact" ) = 1;
              }
            }
            break;

          default:
            throw std_range_error << "Unknown value of artifact mode";
        }
      }
}

