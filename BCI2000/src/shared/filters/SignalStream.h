////////////////////////////////////////////////////////////////////////////////
//  $Id: IdentityFilter.h 4106 2012-06-13 15:30:47Z mellinger $
//  Author:      juergen.mellinger@uni-tuebingen.de
//  Description: A filter that dumps a copy of its input signal into the recorded
//    file as a signal stream.
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
#ifndef SIGNAL_STREAM_H
#define SIGNAL_STREAM_H

#include "GenericFilter.h"
#include "ClassName.h"
#include <sstream>
#include <vector>
#include <iomanip>

template<int MaxChannels = 10>
class SignalStream : public GenericFilter
{
 public:
  void Publish();
  void Preflight( const SignalProperties&, SignalProperties& ) const;
  void Initialize( const SignalProperties&, const SignalProperties& ) {}
  void StartRun();
  void Process( const GenericSignal&, GenericSignal& );
  bool AllowsVisualization() const { return false; }
 private:
  std::vector<std::string> mChannels;
};


template<int MaxChannels>
void
SignalStream<MaxChannels>::Publish()
{
  std::string prefix = "SGNL_" + ClassName( typeid( *this ) ) + "_";
  int places = static_cast<int>( ::ceil( ::log10( 1.0 * MaxChannels ) ) );
  mChannels.clear();
  for( int i = 0; i < MaxChannels; ++i )
  {
    std::ostringstream oss;
    oss << prefix << std::setw( places ) << std::setfill( '0' ) << i;
    mChannels.push_back( oss.str() );
    oss << " 32 0 0 0";
    States->Add( oss.str() );
  }
}

template<int MaxChannels>
void
SignalStream<MaxChannels>::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  for( size_t i = 0; i < mChannels.size(); ++i )
    State( mChannels[i] );
  if( Input.Channels() > MaxChannels )
    bcierr << "Number of stream channels is " << Input.Channels()
           << ", exceeds stream storage (currently " << MaxChannels
           << "; change the MaxChannels template parameter in PipeDefinition.cpp and recompile)";
  if( Input.Elements() > Parameter( "SampleBlockSize" ) )
    bcierr << "Number of stream elements per block exceeds SampleBlockSize";
  Output = Input;
}

template<int MaxChannels>
void
SignalStream<MaxChannels>::StartRun()
{
  for( size_t i = 0; i < mChannels.size(); ++i )
    State( mChannels[i] ).AsFloat() = NaN<float>();
}

template<int MaxChannels>
void
SignalStream<MaxChannels>::Process( const GenericSignal& Input, GenericSignal& Output )
{
  for( int ch = 0; ch < Input.Channels(); ++ch )
    for( int el = 0; el < Input.Elements(); ++el )
      State( mChannels[ch] )( el ).AsFloat() = static_cast<float>( Input( ch, el ) );
  Output = Input;
}

#endif // SIGNAL_STREAM_H
