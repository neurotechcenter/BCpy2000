////////////////////////////////////////////////////////////////////////////////
// $Id: Rectify.h 4536 2013-08-05 14:30:13Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: Rectification functions
//
// $BEGIN_BCI2000_LICENSE$
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#ifndef RECTIFY_H
#define RECTIFY_H

#include <valarray>
#include <algorithm>
#include <numeric>

namespace Rectify
{
  template<typename T>
   T HalfWaveRectify( const T &val );
  template<typename T>
   void HalfWaveRectify( const std::valarray<T>&, std::valarray<T>& );
  template<typename T>
   void HalfWaveRectify( const T&, T& );
  template<typename T>
   T FullWaveRectify( const T &val );
  template<typename T>
   void FullWaveRectify( const std::valarray<T>&, std::valarray<T>& );
  template<typename T>
   void FullWaveRectify( const T&, T& );
}

// Implementation
template<typename T> 
T Rectify::HalfWaveRectify( const T &val ) { return ( val < 0.0 ) ? 0.0 : val; }

template<typename T>
void
Rectify::HalfWaveRectify( const std::valarray<T>& inData, std::valarray<T>& outResult )
{
  if( outResult.size() != inData.size() )
    outResult.resize( inData.size(), 0.0 );
  for( size_t i = 0; i < inData.size(); i++ )
    outResult = inData.apply( Rectify::HalfWaveRectify );
}

template<typename T>
void
Rectify::HalfWaveRectify( const T& inData, T& outResult )
{
  for( int chan = 0; chan < inData.Channels(); chan++ )
    for( int elem = 0; elem < inData.Elements(); elem++ )
      outResult( chan, elem ) = HalfWaveRectify( inData( chan, elem ) );
}

template<typename T> 
T Rectify::FullWaveRectify( const T &val ) { return fabs( val ); }

template<typename T>
void
Rectify::FullWaveRectify( const std::valarray<T>& inData, std::valarray<T>& outResult )
{
  if( outResult.size() != inData.size() )
    outResult.resize( inData.size(), 0.0 );
  if( inData.size() > 0 )
    outResult = inData.apply( Rectify::FullWaveRectify );
}

template<typename T>
void
Rectify::FullWaveRectify( const T& inData, T& outResult )
{
  for( int chan = 0; chan < inData.Channels(); chan++ )
    for( int elem = 0; elem < inData.Elements(); elem++ )
      outResult( chan, elem ) = FullWaveRectify( inData( chan, elem ) );
}

#endif // RECTIFY_H
