////////////////////////////////////////////////////////////////////////////////
//  $Id: ViewSource.cpp 4970 2015-08-21 16:46:07Z mellinger $
//  Author: juergen.mellinger@uni-tuebingen.de
//  Description: A data source that combines multiple data sources into a
//    single one, and that provides its output if form of a GenericSignal,
//    suitable for visualization, or as a filter output.
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
#include "ViewSource.h"
#include "Debugging.h"

using namespace std;

ViewSource::ViewSource( const std::string& inName )
: DataSource( inName )
{
}

void
ViewSource::OnInitialize( const Context& )
{
  mElements.clear();
  SignalProperties output;
  if( !Sources().empty() )
  {
    output = Sources()[0]->Properties();
    mElements.push_back( output.Elements() );
  }
  for( size_t i = 1; i < Sources().size(); ++i )
  {
    SignalProperties s( Sources()[i]->Properties() );
    if( s.Channels() != output.Channels() )
    {
      bcierr << Name() << " View: Cannot concatenate data with differing number of channels"
             << " when adding \"" << Sources()[i]->Name() << "\", which has "
             << s.Channels() << " channels"
             << endl;
    }
    else
    {
      AppendElements( output, s );
      mElements.push_back( s.Elements() );
    }
  }
  if( output.Elements() == 1 )
  {
    output.ElementUnit() = PhysicalUnit()
                           .SetGain( MeasurementUnits::SampleBlockDuration() )
                           .SetOffset( 0 )
                           .SetSymbol( "s" )
                           .SetRawMin( 0 )
                           .SetRawMax( MeasurementUnits::TimeInSampleBlocks( "10s" ) - 1 );
    output.ElementLabels()[0] = LabelIndex::TrivialLabel( 0 );
    output.SetIsStream( true );
  }
  mProperties = output;
  mSignal.SetProperties( output );
}

void
ViewSource::OnProcess( const Context& )
{
  for( int ch = 0; ch < mSignal.Channels(); ++ch )
  {
    int el = 0;
    for( size_t i = 0; i < Sources().size(); ++i )
      for( int j = 0; j < mElements[i]; ++j )
        mSignal( ch, el++ ) = Sources()[i]->Data( ch * mElements[i] + j );
  }
}

DataSource::Value
ViewSource::OnData( int inIdx )
{
  int ch = inIdx / mSignal.Elements(),
      el = inIdx % mSignal.Elements();
  return mSignal( ch, el );
}

void
ViewSource::AppendElements( SignalProperties& ioSp, const SignalProperties& inSp )
{
  Assert( inSp.Channels() == ioSp.Channels() );
  if( ioSp.ChannelLabels().IsTrivial() )
    ioSp.ChannelLabels() = inSp.ChannelLabels();

  for( int i = 0; i < ioSp.Channels(); ++i )
    ioSp.ValueUnit( i ).Combine( inSp.ValueUnit( i ) );

  if( ioSp.ElementUnit() == inSp.ElementUnit() && ioSp.ElementUnit().RawMax() == inSp.ElementUnit().RawMin() - 1 )
    ioSp.ElementUnit().SetRawMax( inSp.ElementUnit().RawMax() );
  else
    ioSp.ElementUnit() = PhysicalUnit();

  int numEl1 = ioSp.Elements(),
      numEl2 = inSp.Elements();
  ioSp.SetElements( numEl1 + numEl2 );
  if( !inSp.ElementLabels().IsTrivial() )
    for( int i = 0; i < numEl2; ++i )
      ioSp.ElementLabels()[numEl1 + i] = inSp.ElementLabels()[i];
}

