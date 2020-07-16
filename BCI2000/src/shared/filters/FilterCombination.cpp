////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
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
#include "FilterCombination.h"

#include "ClassName.h"

#define FOREACH_FILTER(p) for( size_t i = 0; i < mFilters.size(); ++i ) for( GenericFilter* p = mFilters[i].Filter; p; p = 0 )
#define FOREACH_FILTER_ENTRY(p) for( size_t i = 0; i < mFilters.size(); ++i ) for( const FilterEntry* p = &mFilters[i]; p; p = 0 )

using namespace std;

FilterCombination::~FilterCombination()
{
  FOREACH_FILTER( p )
    delete p;
}

void
FilterCombination::Add( GenericFilter::Registrar* Registrar, GenericSignal* Input, GenericSignal* Output )
{
  FilterEntry entry = { Registrar, 0, Input, Output };
  mFilters.push_back( entry );
}

string
FilterCombination::FilterName( const GenericFilter* p )
{
  string result;
  std::string name = ClassName( typeid( *p ) );
  for( size_t i = 0; i < name.length(); ++i )
    if( ::isalnum( name[i] ) )
      result += name[i];
  return result;
}

FilterCombination::IndexVector::IndexVector( const IndexList& list )
{
  for( IndexList::const_iterator i = list.begin(); i != list.end(); ++i )
    push_back( Floor( *i ) );
}

void
FilterCombination::ApplySubset( const IndexVector& Idx, const GenericSignal& Input, GenericSignal& Output ) const
{
  for( size_t i = 0; i < Idx.size(); ++i )
    for( int j = 0; j < Input.Elements(); ++j )
      Output( i, j ) = Input( Idx[i], j );
}

void
FilterCombination::Publish()
{
  // reverse order of instantiation matters
  for( size_t i = mFilters.size(); i > 0; --i )
  {
    FilterEntry& entry = mFilters[i-1];
    entry.Filter = entry.Registrar->NewInstance();
    entry.Filter->CallPublish();
  }
}

void
FilterCombination::StartRun()
{
  FOREACH_FILTER( p )
    p->CallStartRun();
}

void
FilterCombination::StopRun()
{
  FOREACH_FILTER( p )
    p->CallStopRun();
}

void
FilterCombination::Resting()
{
  FOREACH_FILTER( p )
    p->CallResting();
}

void
FilterCombination::Halt()
{
  FOREACH_FILTER( p )
    p->CallHalt();
}

void
FilterCombination::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  FOREACH_FILTER_ENTRY( pEntry )
  {
    const SignalProperties& input = pEntry->Input ? pEntry->Input->Properties() : Input;
    SignalProperties temp = input;
    SignalProperties& output = pEntry->Output ? temp : Output;
    pEntry->Filter->CallAutoConfig( input );
    pEntry->Filter->CallPreflight( input, output );
    if( pEntry->Output )
      *pEntry->Output = GenericSignal( output );
  }
}

void
FilterCombination::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
  FOREACH_FILTER_ENTRY( pEntry )
  {
    const SignalProperties& input = pEntry->Input ? pEntry->Input->Properties() : Input,
                          & output = pEntry->Output ? pEntry->Output->Properties() : Output;
    pEntry->Filter->CallInitialize( input, output );
  }
}

void
FilterCombination::Process( const GenericSignal& Input, GenericSignal& Output )
{
  FOREACH_FILTER_ENTRY( pEntry )
  {
    const GenericSignal& input = pEntry->Input ? *pEntry->Input : Input;
    GenericSignal& output = pEntry->Output ? *pEntry->Output : Output;
    pEntry->Filter->CallProcess( input, output );
  }
}

// ParallelCombination implementation

void
ParallelCombinationBase::Publish()
{
  FilterCombination::Publish();
  mParameterName = FilterName( this ) + "Channels";
  std::string p =
    "Filtering matrix " + mParameterName + "= 0 1 % % % "
    "// list of channels transmitted to separate chains: Channel names may contain * and ? wildcards, "
    " and character ranges enclosed in []; wildcard patterns may be negated by prepending an exclamation mark. "
    " Ranges of channels may be specified using : or - to separate begin from end.";
  BEGIN_PARAMETER_DEFINITIONS
    p.c_str()
  END_PARAMETER_DEFINITIONS

  MutableParamRef subsets = Parameter( mParameterName );
  subsets->SetNumRows( mFilters.size() );
  for( size_t i = 0; i < mFilters.size(); ++i )
  {
    subsets->RowLabels()[i] = FilterName( mFilters[i].Filter );
    subsets( i, 0 ) = "*";
  }
}


void
ParallelCombinationBase::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  mInput1 = GenericSignal( SubsetProperties( 0, Input ) );
  mInput2 = GenericSignal( SubsetProperties( 1, Input ) );

  FilterCombination::Preflight( Input, Output );

  const SignalProperties& output1 = mOutput1.Properties(),
                        & output2 = mOutput2.Properties();
  Output = SignalProperties( output1.Channels() + output2.Channels(), std::max( output1.Elements(), output2.Elements() ) );

  Output.ChannelUnit() = PhysicalUnit();
  Output.ElementUnit() = PhysicalUnit().SetRawMax( Output.Elements() - 1 );
  Output.ValueUnit() = PhysicalUnit();
  double min = std::min( output1.ValueUnit().RawMin(), output2.ValueUnit().RawMin() ),
         max = std::max( output1.ValueUnit().RawMax(), output2.ValueUnit().RawMax() );
  Output.ValueUnit().SetRawMin( min ).SetRawMax( max );

  if( !output1.ChannelLabels().IsTrivial() )
    for( int ch = 0; ch < output1.Channels(); ++ch )
      Output.ChannelLabels()[ch] = output1.ChannelLabels()[ch];
  if( !output2.ChannelLabels().IsTrivial() )
    for( int ch = 0; ch < output2.Channels(); ++ch )
      Output.ChannelLabels()[ch + output1.Channels()] = output2.ChannelLabels()[ch];
}


void
ParallelCombinationBase::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
  mIdx1 = IndexList( Subset( 0 ), Input.ChannelLabels() );
  mIdx2 = IndexList( Subset( 1 ), Input.ChannelLabels() );
  FilterCombination::Initialize( Input, Output );
}


void
ParallelCombinationBase::Process( const GenericSignal& Input, GenericSignal& Output )
{
  ApplySubset( mIdx1, Input, mInput1 );
  ApplySubset( mIdx2, Input, mInput2 );

  FilterCombination::Process( Input, Output );

  for( int ch = 0; ch < mOutput1.Channels(); ++ch )
    for( int el = 0; el < mOutput1.Elements(); ++el )
      Output( ch, el ) = mOutput1( ch, el );

  for( int ch = 0; ch < mOutput2.Channels(); ++ch )
    for( int el = 0; el < mOutput2.Elements(); ++el )
      Output( ch + mOutput1.Channels(), el ) = mOutput2( ch, el );
}


std::string
ParallelCombinationBase::Subset( int idx ) const
{
  std::string result;
  ParamRef p = Parameter( mParameterName );
  for( int i = 0; i < p->NumColumns(); ++i )
  {
    result += p( idx, i );
    result += " ";
  }
  return result;
}


SignalProperties
ParallelCombinationBase::SubsetProperties( int idx, const SignalProperties& Input ) const
{
  std::string subset = Subset( idx );
  IndexList list = IndexList( subset, Input.ChannelLabels() );
  if( !list.Errors().empty() )
    bcierr << "Invalid channel subset: " << subset << ": " << list.Errors();
  else if( list.Empty() )
    bciwarn << "Channel subset \"" << subset << "\" does not match any channel";

  const GenericFilter* p = mFilters[idx].Filter;
  SignalProperties result = Input;
  result.SetName( ClassName( typeid( *p ) ) + " Subset" );
  result.SetChannels( list.Size() );
  for( int idxOut = 0; idxOut < list.Size(); ++idxOut )
    result.ChannelLabels()[idxOut] = Input.ChannelLabels()[Floor( list[idxOut] )];
  return result;
}
