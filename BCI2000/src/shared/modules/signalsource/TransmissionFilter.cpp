////////////////////////////////////////////////////////////////////////////////
// $Id: TransmissionFilter.cpp 5183 2015-12-23 20:00:57Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A filter that returns a subset of input channels in its output
//   signal.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "TransmissionFilter.h"
#include "IndexList.h"
#include "BCIStream.h"

#include <limits>

using namespace std;

RegisterFilter( TransmissionFilter, 1.2 );

TransmissionFilter::TransmissionFilter()
: mCopy( false )
{
  BEGIN_PARAMETER_DEFINITIONS
    "Source:Online%20Processing list TransmitChList= 1 * % % % "
    "// list of transmitted channels: Channel names may contain * and ? wildcards, "
    " and character ranges enclosed in []; wildcard patterns may be negated by prepending an exclamation mark. "
    " Ranges of channels may be specified using : or - to separate begin from end.",
  END_PARAMETER_DEFINITIONS
}

void TransmissionFilter::Preflight( const SignalProperties& Input,
                                          SignalProperties& Output ) const
{
  string TransmitChList = FlatParameter( "TransmitChList" );
  IndexList idx( TransmitChList, Input.ChannelLabels() );
  if( !idx.Errors().empty() )
    bcierr << "Invalid TransmitChList: " << TransmitChList << ": " << idx.Errors();
  else if( idx.Empty() && !Input.IsEmpty() )
    bciwarn << "TransmitChList \"" << TransmitChList << "\" does not match any channel";

  Output = Input;
  Output.SetName( "Transmitted Channels" );
  Output.SetChannels( idx.Size() );
  for( int idxOut = 0; idxOut < idx.Size(); ++idxOut )
    Output.ChannelLabels()[idxOut] = Input.ChannelLabels()[static_cast<int>( idx[idxOut] )];
}

void TransmissionFilter::Initialize( const SignalProperties& Input,
                                     const SignalProperties& Output )
{
  mChannelList.clear();
  IndexList idx( FlatParameter( "TransmitChList" ), Input.ChannelLabels() );
  mCopy = ( Input.Channels() == Output.Channels() );
  for( int i = 0; i < idx.Size(); ++i )
  {
    mCopy = mCopy && idx[i] == i;
    mChannelList.push_back( static_cast<size_t>( idx[i] ) );
  }
}

void TransmissionFilter::Process( const GenericSignal& Input,
                                        GenericSignal& Output )
{
  if( mCopy )
    Output = Input;
  else
    for( size_t i = 0; i < mChannelList.size(); ++i )
      for( int j = 0; j < Input.Elements(); ++j )
        Output( i, j ) = Input( mChannelList[ i ], j );
}

string
TransmissionFilter::FlatParameter( const string& inName ) const
{
  ParamRef p = Parameter( inName );
  string flat = p( 0 );
  for( int i = 1; i < p->NumValues(); ++i )
    flat += " " + string( p( i ) );
  return flat;
}
