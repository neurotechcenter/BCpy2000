////////////////////////////////////////////////////////////////////////////////
// $Id: RootMeanSquare.h 4536 2013-08-05 14:30:13Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: Calculates the Root Mean Square of a series of values
//
// $BEGIN_BCI2000_LICENSE$
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#ifndef ROOTMEANSQUARE_H
#define ROOTMEANSQUARE_H

#include <valarray>
#include <algorithm>
#include <numeric>

template<typename T>
double RootMeanSquare( const std::valarray<T>& );

// Implementation
template<typename T>
double
RootMeanSquare( const std::valarray<T>& inData )
{
  std::valarray<T> square = inData * inData;
  double RMS = square.sum() / ( double )square.size();
  return sqrt( RMS );
}

template<typename T>
double
RootMeanSquare( const T& inData )
{
  double squareSum = 0;
  for( size_t i = 0; i < inData.size(); i++ )
    squareSum += inData[i] * inData[i];
  return sqrt( squareSum / ( double )inData.size() );
}

#endif // ROOTMEANSQUARE_H
