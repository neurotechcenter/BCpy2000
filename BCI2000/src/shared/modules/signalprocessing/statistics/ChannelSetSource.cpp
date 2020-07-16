////////////////////////////////////////////////////////////////////////////////
// $Id
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A data source that collects its data from a channel set.
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
#include "ChannelSetSource.h"
#include "Debugging.h"

using namespace std;

ChannelSetSource::ChannelSetSource( const std::string& inName, const ChannelSet& inSet )
: DataSource( inName ),
  mChannelSet( inSet ),
  mpSignal( NULL )
{
}

void
ChannelSetSource::OnInitialize( const Context& inContext )
{
  const SignalProperties& Input = inContext.signal->Properties();
  SignalProperties Output = Input;
  if( !mChannelSet.empty() )
  {
    Output.SetChannels( mChannelSet.size() );
    for( size_t i = 0; i < mChannelSet.size(); ++i )
      Output.ChannelLabels()[i] = Input.ChannelLabels()[mChannelSet[i]];
  }
  mProperties = Output;
}

void
ChannelSetSource::OnProcess( const Context& inContext )
{
  mpSignal = inContext.signal;
}

DataSource::Value
ChannelSetSource::OnData( int inIdx )
{
  Assert( mpSignal != nullptr );
  int ch = inIdx / mpSignal->Elements(),
      el = inIdx % mpSignal->Elements();
  return ( *mpSignal )( mChannelSet[ch], el );
}

