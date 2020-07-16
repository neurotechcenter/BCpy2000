/////////////////////////////////////////////////////////////////////////////////
// $Id: Polynomials.h 4748 2014-10-15 20:15:17Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Class templates for univariate polynomials and rational
//              expressions.
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
/////////////////////////////////////////////////////////////////////////////////
#ifndef POLYNOMIALS_H
#define POLYNOMIALS_H

#include <vector>
#include <valarray>
#include <algorithm>
#include <stdexcept>
#include <complex>
#include <limits>

namespace {
template<typename T>
struct ComplexExtension
{
  typedef T Real;
  typedef std::complex<T> Complex;
};
template<typename T>
struct ComplexExtension< std::complex<T> > : ComplexExtension<T>
{
};
} // namespace


template<class T>
class Polynomial
{
 public:
  typedef typename ComplexExtension<T>::Real Real;
  typedef typename ComplexExtension<T>::Complex Complex;
  typedef std::vector<T> Vector;

 private:
  Polynomial( const Vector& roots, const T& factor );
  Polynomial( const Vector& coefficients );

 public:
  Polynomial( const T& );

  static Polynomial FromRoots( const Vector& roots, const T& factor = 1 );
  static Polynomial FromCoefficients( const Vector& coefficients );
  template<typename U> static Polynomial FromCoefficients( const std::valarray<U>& );

  Polynomial& operator*=( const T& );
  Polynomial& operator*=( const Polynomial& );
  template<class U> Polynomial operator*( const U& ) const;

  int Order() const;
  // Return a list of polynomial coefficients.
  const Vector& Coefficients() const;
  // Do we know the polynomial's roots?
  bool RootsKnown() const;
  // Return a list of roots.
  const Vector& Roots() const;
  const T& ConstantFactor() const;
  // Compute the polynomial's value for a given argument.
  T Evaluate( const Real& r, int derivative = 0 ) const { return DoEvaluate<T, Real>( r, derivative ); }
  Complex Evaluate( const Complex& c, int derivative = 0 ) const { return DoEvaluate<Complex, Complex>( c, derivative ); }

 private:
  template<class R, class Z> R DoEvaluate( const Z&, int ) const;
  void FindRoots() const { throw std::logic_error( "Polynomial::FindRoots: Root finding not implemented." ); }

  mutable bool mRootsKnown;     //
  mutable T mConstantFactor;    // These may change during a call to Roots().
  mutable Vector mRoots;        //

  mutable Vector mCoefficients; // This may change during a call to Coefficients().
};

template<class T>
class Ratpoly // A rational expression with a polynomial numerator and denominator.
{
  typedef typename Polynomial<T>::Real Real;
  typedef typename Polynomial<T>::Complex Complex;

 public:
  Ratpoly( const T& = 1 );
  Ratpoly( const Polynomial<T>& numerator, const Polynomial<T>& denominator );

  Ratpoly& operator*=( const T& );
  Ratpoly& operator*=( const Polynomial<T>& );
  Ratpoly& operator*=( const Ratpoly& );
  template<class U> Ratpoly operator*( const U& ) const;

  const Polynomial<T>& Numerator() const;
  const Polynomial<T>& Denominator() const;
  T ConstantFactor() const;
  T Evaluate( const Real& r ) const { return DoEvaluate<T, Real>( r ); }
  Complex Evaluate( const Complex& c ) const { return DoEvaluate<Complex, Complex>( c ); }

 private:
  template<class R, class Z> R DoEvaluate( const Z& ) const;
  void Simplify(); // remove cancelling factors

 private:
  Polynomial<T> mNumerator, mDenominator;
};


// Polynomial definitions

template<class T>
Polynomial<T>::Polynomial( const T& factor )
: mRootsKnown( true ),
  mConstantFactor( factor )
{
}

template<class T>
Polynomial<T>::Polynomial( const Vector& roots, const T& constantFactor )
: mRootsKnown( true ),
  mConstantFactor( constantFactor ),
  mRoots( roots )
{
}

template<class T>
Polynomial<T>::Polynomial( const Vector& coefficients )
: mRootsKnown( false ),
  mConstantFactor( 1 ),
  mCoefficients( coefficients )
{
  if( mCoefficients.size() == 1 )
  {
    mConstantFactor = mCoefficients[ 0 ];
    mRootsKnown = true;
  }
}

template<class T>
Polynomial<T>
Polynomial<T>::FromRoots( const Vector& roots, const T& constantFactor )
{
  return Polynomial<T>( roots, constantFactor );
}

template<class T>
Polynomial<T>
Polynomial<T>::FromCoefficients( const Vector& coefficients )
{
  return Polynomial<T>( coefficients );
}

template<class T> template<class U>
Polynomial<T>
Polynomial<T>::FromCoefficients( const std::valarray<U>& inCoefficients )
{
  Vector coefficients( inCoefficients.size() );
  for( size_t i = 0; i < inCoefficients.size(); ++i )
    coefficients[ i ] = inCoefficients[ i ];
  return Polynomial<T>( coefficients );
}

template<class T> template<class R, class Z>
R
Polynomial<T>::DoEvaluate( const Z& z, int d ) const
{
  R result = 0;
  if( mRootsKnown && d == 0 )
  {
    result = mConstantFactor;
    for( typename Vector::const_iterator i = mRoots.begin(); i != mRoots.end(); ++i )
      result *= ( z - *i );
  }
  else
  {
    Coefficients();
    Z powerOfZ = 1;
    for( size_t i = 0; i < mCoefficients.size() - d; ++i, powerOfZ *= z )
      result += mCoefficients[ i + d ] * powerOfZ;
  }
  return result;
}

template<class T>
int
Polynomial<T>::Order() const
{
  return static_cast<int>( mRootsKnown ? mRoots.size() : mCoefficients.size() - 1 );
}

template<class T>
const typename Polynomial<T>::Vector&
Polynomial<T>::Coefficients() const
{ // Compute coefficients by expanding the product of roots.
  if( mRootsKnown && mCoefficients.empty() )
  {
    mCoefficients = Vector( mRoots.size() + 1, 0 );
    mCoefficients[ 0 ] = 1;
    /* one after one, multiply a factor of (z-mRoots[i]) into coeffs */
    for( typename Vector::const_iterator i = mRoots.begin(); i != mRoots.end(); ++i )
    {
      for( size_t j = mCoefficients.size() - 1; j >= 1; --j )
      {
        mCoefficients[ j ] *= -( *i );
        mCoefficients[ j ] += mCoefficients[ j - 1 ];
      }
      mCoefficients[ 0 ] *= -( *i );
    }
  }
  return mCoefficients;
}

template<class T>
bool Polynomial<T>::RootsKnown() const
{
  return mRootsKnown;
}

template<class T>
const typename Polynomial<T>::Vector&
Polynomial<T>::Roots() const
{
  if( !mRootsKnown )
    FindRoots();
  return mRoots;
}

template<class T>
const T&
Polynomial<T>::ConstantFactor() const
{
  if( !mRootsKnown )
    FindRoots();
  return mConstantFactor;
}

template<class T>
Polynomial<T>&
Polynomial<T>::operator*=( const T& f )
{
  if( mRootsKnown )
    mConstantFactor *= f;
  for( size_t i = 0; i < mCoefficients.size(); ++i )
    mCoefficients[i] *= f;
  return *this;
}

template<class T>
Polynomial<T>&
Polynomial<T>::operator*=( const Polynomial& p )
{
  if( mRootsKnown && p.mRootsKnown )
  {
    mConstantFactor *= p.mConstantFactor;
    mRoots.insert( mRoots.end(), p.mRoots.begin(), p.mRoots.end() );
    mCoefficients = Vector( 0 );
  }
  else
  {
    Vector        coeff1 = this->Coefficients();
    const Vector& coeff2 = p.Coefficients();
    mRootsKnown = false;
    mRoots.clear();
    mCoefficients = Vector( coeff1.size() + coeff2.size() - 1, 0 );
    for( size_t i = 0; i < coeff1.size(); ++i )
      for( size_t j = 0; j < coeff2.size(); ++j )
        mCoefficients[ i + j ] += coeff1[ i ] * coeff2[ j ];
  }
  return *this;
}

template<class T> template<class U>
Polynomial<T>
Polynomial<T>::operator*( const U& u ) const
{
  return Polynomial<T>( *this ) *= u;
}


// Ratpoly definitions
#ifdef __GNUC__
# define TYPENAME typename
#else
# define TYPENAME
#endif // __GNUC__

template<class T>
Ratpoly<T>::Ratpoly( const T& z )
: mNumerator( Polynomial<T>::FromRoots( TYPENAME Polynomial<T>::Vector(), z ) ),
  mDenominator( Polynomial<T>::FromRoots( TYPENAME Polynomial<T>::Vector(), 1 ) )
{
}

#undef TYPENAME

template<class T>
Ratpoly<T>::Ratpoly( const Polynomial<T>& numerator,
                     const Polynomial<T>& denominator )
: mNumerator( numerator ),
  mDenominator( denominator )
{
  Simplify();
}

template<class T>
Ratpoly<T>&
Ratpoly<T>::operator*=( const T& f )
{
  mNumerator *= f;
  return *this;
}

template<class T>
Ratpoly<T>&
Ratpoly<T>::operator*=( const Polynomial<T>& p )
{
  mNumerator *= p;
  Simplify();
  return *this;
}

template<class T>
Ratpoly<T>&
Ratpoly<T>::operator*=( const Ratpoly& r )
{
  mNumerator *= r.mNumerator;
  mDenominator *= r.mDenominator;
  Simplify();
  return *this;
}

template<class T> template<class U>
Ratpoly<T>
Ratpoly<T>::operator*( const U& u ) const
{
  return Ratpoly<T>( *this ) *= u;
}

template<class T>
const Polynomial<T>&
Ratpoly<T>::Numerator() const
{
  return mNumerator;
}

template<class T>
const Polynomial<T>&
Ratpoly<T>::Denominator() const
{
  return mDenominator;
}

template<class T>
T Ratpoly<T>::ConstantFactor() const
{
  return mNumerator.ConstantFactor() / mDenominator.ConstantFactor();
}

namespace {

template<typename T>
bool CloseToZero( const T& t )
{
  return std::fabs( t ) < std::numeric_limits<T>::epsilon();
}

template<typename T>
bool CloseToZero( const std::complex<T>& t )
{ // Avoid an expensive abs() call.
  return CloseToZero( t.real() ) && CloseToZero( t.imag() );
}

} // namespace

template<class T> template<class R, class Z>
R
Ratpoly<T>::DoEvaluate( const Z& z ) const
{
  R num = mNumerator.Evaluate( z ),
    denom = mDenominator.Evaluate( z );
  if( CloseToZero( denom ) )
  {
    int derivative = 0;
    while( CloseToZero( denom ) && derivative <= mDenominator.Order() && CloseToZero( num ) )
    {
      num = mNumerator.Evaluate( z, derivative );
      denom = mDenominator.Evaluate( z, derivative );
      ++derivative;
    }
    if( CloseToZero( denom ) )
    {
      if( CloseToZero( num ) )
      {
        num = 1;
        denom = 1;
      }
      else
      {
        num = 1;
        denom = std::numeric_limits<T>::epsilon();
      }
    }
  }
  return num / denom;
}

template<class T>
void
Ratpoly<T>::Simplify()
{
  if( mNumerator.RootsKnown() && mDenominator.RootsKnown() )
  {
    typename Polynomial<T>::Vector numerRoots = mNumerator.Roots(),
                                   denomRoots = mDenominator.Roots(),
                                   commonRoots;
    typename Polynomial<T>::Vector::const_iterator i;
    for( i = numerRoots.begin(); i != numerRoots.end(); ++i )
    {
      typename Polynomial<T>::Vector::iterator j = find( denomRoots.begin(), denomRoots.end(), *i );
      if( j != denomRoots.end() )
      {
        commonRoots.push_back( *i );
        denomRoots.erase( j );
      }
    }
    for( i = commonRoots.begin(); i != commonRoots.end(); ++i )
      numerRoots.erase( find( numerRoots.begin(), numerRoots.end(), *i ) );

    if( !commonRoots.empty() )
    {
      mNumerator = Polynomial<T>::FromRoots( numerRoots, mNumerator.ConstantFactor() );
      mDenominator = Polynomial<T>::FromRoots( denomRoots, mDenominator.ConstantFactor() );
    }
  }
}

#endif // POLYNOMIALS_H

