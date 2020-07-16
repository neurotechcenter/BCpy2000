////////////////////////////////////////////////////////////////////////////////
// $Id
// Author: juergen.mellinger@uni-tuebingen.de
//  Description: A base class for objects that provide signal data to other
//    objects, and that depend on such data.
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
#include "DataSource.h"
#include "BCIStream.h"
#include "Debugging.h"
#include <algorithm>

using namespace std;

bool DataSource::sAbortFlag = false;

DataSource*
DataSource::SourceList::Find( const std::string& inName ) const
{
  DataSource* p = NULL;
  const_iterator i = begin();
  while( p == NULL && i != end() )
  {
    if( ( *i )->Name() == inName )
      p = *i;
    ++i;
  }
  return p;
}

bool
DataSource::SourceList::Contains( DataSource* p )
{
  return find( begin(), end(), p ) != end();
}

DataSource::SourceList&
DataSource::SourceList::Add( DataSource* p )
{
  Assert( p != NULL );
  push_back( p );
  return *this;
}

DataSource::SourceList&
DataSource::SourceList::AddUnique( DataSource* p )
{
  if( !Contains( p ) )
    Add( p );
  return *this;
}

bool
DataSource::Dimension::operator==( const Dimension& d ) const
{
  return name == d.name
      && continuous == d.continuous
      && streaming == d.streaming
      && unit == d.unit
      && labels == d.labels;
}

DataSource::DataProperties::DataProperties( const SignalProperties& s )
: mDimensions( 2 ),
  mUnit( s.ValueUnit() )
{
  Dimension& ch = mDimensions[0];
  ch.unit = s.ChannelUnit();
  ch.labels = s.ChannelLabels();

  Dimension& el = mDimensions[1];
  el.unit = s.ElementUnit();
  el.labels = s.ElementLabels();
  el.continuous = s.ElementLabels().IsTrivial();
  el.streaming = s.IsStream();

  if( !s.IsStream() && s.UpdateRate() > 0 )
  {
    mDimensions.resize( 3 );
    Dimension& st = mDimensions[2];
    st.labels.Resize( 1 );
    st.unit.SetGain( 1.0 / s.UpdateRate() )
           .SetSymbol( "s" );
    st.continuous = true;
    st.streaming = true;
  }
}

DataSource::DataProperties::operator SignalProperties() const
{
  IndexList nonSingletonDims;
  int streamingDim = -1;
  for( size_t i = 0; i < mDimensions.size(); ++i )
  {
    Assert( mDimensions[i].Size() > 0 );
    if( mDimensions[i].Size() > 1 )
      nonSingletonDims.push_back( static_cast<int>( i ) );
    if( mDimensions[i].streaming )
    {
      if( streamingDim == -1 )
        streamingDim = static_cast<int>( i );
      else if( mDimensions[i].unit != mDimensions[streamingDim].unit )
        throw std_runtime_error << "Incompatible streaming dimensions";
    }
  }
  SignalProperties result( 0, 0 );
  result.SetName( mName );
  result.ValueUnit() = mUnit;
  if( !mDimensions.empty() )
  {
    if( nonSingletonDims.size() == 0 )
    {
      result.SetChannels( 1 )
            .SetElements( 1 );
      result.ChannelLabels()[0] = mName;
      result.ElementLabels()[0] = mName;
    }
    else if( nonSingletonDims.size() == 1 )
    {
      size_t idx = nonSingletonDims[0];
      const Dimension& dim = mDimensions[idx];
      if( dim.continuous )
      {
        result.SetChannels( 1 );
        result.ChannelLabels()[0] = mName;
        result.ElementLabels() = dim.labels;
        result.ElementUnit() = dim.unit;
        if( dim.streaming )
          result.SetIsStream( true );
      }
      else
      {
        result.ChannelLabels() = dim.labels;
        result.SetElements( 1 );
        result.ElementLabels()[0] = mName;
      }
    }
    else if( nonSingletonDims.size() == 2 )
    {
      size_t idx1 = nonSingletonDims[0];
      result.ChannelLabels() = mDimensions[idx1].labels;
      result.ChannelUnit() = mDimensions[idx1].unit;
      size_t idx2 = nonSingletonDims[1];
      result.ElementLabels() = mDimensions[idx2].labels;
      result.ElementUnit() = mDimensions[idx2].unit;
      if( !mDimensions[idx1].streaming && mDimensions[idx2].streaming )
        result.SetIsStream( true );
    }
    else
    {
      size_t numElements = 1;
      for( size_t i = 1; i < nonSingletonDims.size(); ++i )
        numElements *= mDimensions[nonSingletonDims[i]].Size();
      for( size_t i = 1; i < nonSingletonDims.size(); ++i )
      {
        const Dimension& dim = mDimensions[nonSingletonDims[i]];
        Assert( dim.Size() == dim.unit.RawRange() );
        const LabelIndex& labels = dim.labels.IsTrivial() ?
                                   LabelIndex( dim.unit ) :
                                   dim.labels;
        if( i == 1 )
          result.ElementLabels() = labels;
        else
          result.ElementLabels() *= labels;
      }
    }
  }
  if( streamingDim >= 0 )
    result.SetUpdateRate( 1.0 / mDimensions[streamingDim].unit.Gain() / mDimensions[streamingDim].Size() );
  return result;
}

int
DataSource::DataProperties::DataSize() const
{
  int result = 1;
  for( size_t i = 0; i < mDimensions.size(); ++i )
    result *= mDimensions[i].Size();
  return result;
}

int
DataSource::DataProperties::ToSingleIndex( const IndexList& inIdx ) const
{
  Assert( inIdx.size() == mDimensions.size() );
  int result = 0;
  for( size_t i = 0; i < inIdx.size(); ++i )
  {
    Assert( inIdx[i] > 0 );
    Assert( inIdx[i] < mDimensions[i].Size() );
    result *= mDimensions[i].Size();
    result += inIdx[i];
  }
  return result;
}

DataSource::IndexList
DataSource::DataProperties::ToIndexList( int inIdx ) const
{
  size_t dims = mDimensions.size();
  IndexList result( dims );
  int idx = inIdx;
  for( size_t i = 0; i < dims; ++i )
  {
    int size = mDimensions[dims - i].Size();
    result[dims - i] = idx % size;
    idx /= size;
  }
  return result;
}

bool
DataSource::DataProperties::Compatible( const DataSource::DataProperties& inP ) const
{
  SignalProperties s1( *this ),
                   s2( inP );
  bool result = true;
  result &= ( s1.Channels() == s2.Channels() );
  result &= ( s1.Elements() == s2.Elements() );
  result &= ( s1.ChannelLabels() == s2.ChannelLabels() );
  result &= ( s1.ElementLabels() == s2.ElementLabels() );
  return result;
}

DataSource::DataProperties&
DataSource::DataProperties::OuterProduct( const DataProperties& inP )
{
  mDimensions.insert( mDimensions.end(), inP.mDimensions.begin(), inP.mDimensions.end() );
  mUnit *= inP.mUnit;
  return *this;
}

void
DataSource::Depends( const Context& inContext, SourceList& outList, SourceList inList )
{
  BCIStream::ContextFrame frame( Name() );
  if( inList.Contains( this ) )
  {
    bcierr << "Circular dependency on \"" << mName << "\"" << endl;
  }
  else
  {
    OnDepends( inContext );
    inList.Add( this );
    for( size_t i = 0; i < mSources.size(); ++i )
      mSources[i]->Depends( inContext, outList, inList );
    outList.AddUnique( this );
  }
}

void
DataSource::Initialize( const Context& inContext )
{
  BCIStream::ContextFrame frame( Name() );
  OnInitialize( inContext );
}

void
DataSource::Process( const Context& inContext )
{
  BCIStream::ContextFrame frame( Name() );
  OnProcess( inContext );
}

DataSource::Value
DataSource::Data( int inIdx )
{
  return sAbortFlag ? 0 : OnData( inIdx );
}

DataSource::Value
DataSource::Data( const IndexList& inIdx )
{
  return Data( mProperties.ToSingleIndex( inIdx ) );
}

