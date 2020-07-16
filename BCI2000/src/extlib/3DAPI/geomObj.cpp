////////////////////////////////////////////////////////////////////////////////
// $Id: geomObj.cpp 5492 2016-08-04 16:47:13Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: geomObj holds common geometric properties of 3D scene objects,
//   and provides an interface for hit testing (volume intersection).
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
#include "geomObj.h"
#include <limits>

// Transforms
enum // Indices into the transformation matrix
{
  xx =  0, xy =  1, xz =  2,
  yx =  4, yy =  5, yz =  6,
  zx =  8, zy =  9, zz = 10,
  cx = 12, cy = 13, cz = 14,
};

static const float eps = std::numeric_limits<float>::epsilon();
static const float pi = 4.f * ::atan( 1.f );

geomObj::geomObj()
{ // Initialize the world transform to the identity matrix.
  CVector3 nullVector = { 0, 0, 0 };
  mVelocity = nullVector;
  mAngVelocity = nullVector;
}

void
geomObj::setOrigin( const CVector3& c )
{
  movedIf( (getOrigin() - c).sqLength() > eps );
  setOriginPrivate( c );
}

void
geomObj::setOriginPrivate( const CVector3& c )
{
  mWorldTransform.m[ cx ] = c.x;
  mWorldTransform.m[ cy ] = c.y;
  mWorldTransform.m[ cz ] = c.z;
}

CVector3
geomObj::getOrigin() const
{
  CVector3 c =
  {
    mWorldTransform.m[ cx ],
    mWorldTransform.m[ cy ],
    mWorldTransform.m[ cz ]
  };
  return c;
}

void
geomObj::setXAxis( const CVector3& c )
{
  movedIf( (getXAxis() - c).sqLength() > eps );
  setXAxisPrivate( c );
}

void
geomObj::setXAxisPrivate( const CVector3& c )
{
  mWorldTransform.m[ xx ] = c.x;
  mWorldTransform.m[ xy ] = c.y;
  mWorldTransform.m[ xz ] = c.z;
}

CVector3
geomObj::getXAxis() const
{
  CVector3 c =
  {
    mWorldTransform.m[ xx ],
    mWorldTransform.m[ xy ],
    mWorldTransform.m[ xz ]
  };
  return c;
}

void
geomObj::setYAxis( const CVector3& c )
{
  movedIf( (getYAxis() - c).sqLength() > eps );
  setYAxisPrivate( c );
}

void
geomObj::setYAxisPrivate( const CVector3& c )
{
  mWorldTransform.m[ yx ] = c.x;
  mWorldTransform.m[ yy ] = c.y;
  mWorldTransform.m[ yz ] = c.z;
}

CVector3
geomObj::getYAxis() const
{
  CVector3 c =
  {
    mWorldTransform.m[ yx ],
    mWorldTransform.m[ yy ],
    mWorldTransform.m[ yz ]
  };
  return c;
}

void
geomObj::setZAxis( const CVector3& c )
{
  movedIf( (getZAxis() - c).sqLength() > eps );
  setZAxisPrivate( c );
}

void
geomObj::setZAxisPrivate( const CVector3& c )
{
  mWorldTransform.m[ zx ] = c.x;
  mWorldTransform.m[ zy ] = c.y;
  mWorldTransform.m[ zz ] = c.z;
}

CVector3
geomObj::getZAxis() const
{
  CVector3 c =
  {
    mWorldTransform.m[ zx ],
    mWorldTransform.m[ zy ],
    mWorldTransform.m[ zz ]
  };
  return c;
}

void
geomObj::translate( const CVector3& v )
{
  setOrigin( getOrigin() + v );
}

void
geomObj::rotate( const CVector3& inVecAngle )
{
  rotate( Length( inVecAngle ), inVecAngle );
}

void
geomObj::rotate( GLfloat inAngle, const CVector3& inAxis )
{ // Rotate around an axis that contains the global origin.
  double length = Length( inAxis );
  if( length > 0 )
  {
    double sinAng = ::sin( inAngle * pi / 180 ),
           cosAng = ::cos( inAngle * pi / 180 );
    CVector3 a = 1 / length * inAxis,
             xPrime = ( 1 - cosAng ) * a.x * a,
             yPrime = ( 1 - cosAng ) * a.y * a,
             zPrime = ( 1 - cosAng ) * a.z * a;

    xPrime.x += cosAng;
    xPrime.y -= sinAng * a.z;
    xPrime.z += sinAng * a.y;

    yPrime.x += sinAng * a.z;
    yPrime.y += cosAng;
    yPrime.z -= sinAng * a.x;

    zPrime.x -= sinAng * a.y;
    zPrime.y += sinAng * a.x;
    zPrime.z += cosAng;

    CVector3 v = getOrigin();
    setOrigin( v.x * xPrime + v.y * yPrime + v.z * zPrime );
    v = getXAxis();
    setXAxis( v.x * xPrime + v.y * yPrime + v.z * zPrime );
    v = getYAxis();
    setYAxis( v.x * xPrime + v.y * yPrime + v.z * zPrime );
    v = getZAxis();
    setZAxis( v.x * xPrime + v.y * yPrime + v.z * zPrime );
  }
}

void
geomObj::spin( const CVector3& inVecAngle )
{
  spin( Length( inVecAngle ), inVecAngle );
}

void
geomObj::spin( GLfloat inAngle, const CVector3& inAxis )
{ // Rotate around an axis that contains the object's center.
  static CVector3 nullVector = { 0, 0, 0 };
  CVector3 origin = getOrigin();
  setOriginPrivate( nullVector );
  rotate( inAngle, inAxis );
  setOriginPrivate( origin );
}

void
geomObj::scale( GLfloat x, GLfloat y, GLfloat z )
{ // Scale without affecting the object's position.
  setXAxis( x * getXAxis() );
  setYAxis( y * getYAxis() );
  setZAxis( z * getZAxis() );
}

geomObj::testResult
geomObj::intersects( const Primitive& p ) const
{
  if( mTriangles.empty() )
    return undetermined;
  for( const Triangle& t : mTriangles )
  {
    Triangle tprime = t;
    tprime.Apply( mWorldTransform );
    if( tprime.Intersects( p ) )
      return true_;
  }
  return false_;
}

geomObj::testResult
geomObj::intersectsVolume( const geomObj& inObj ) const
{
  if( isConvex() == true_ )
  {
    if( inObj.contains( getOrigin() ) == true_ )
      return true_;
    return intersectsSurface( inObj );
  }
  if( intersectsSurface( inObj ) == true_ )
    return true_;
  return undetermined;
}

geomObj::testResult
geomObj::intersectsSurface( const geomObj& inObj ) const
{
  if( mTriangles.empty() )
    return undetermined;
  testResult s = false_;
  for( const Triangle& t : mTriangles )
  {
    Triangle tprime = t;
    tprime.Apply( mWorldTransform );
    testResult r = inObj.intersects( tprime );
    if( r == undetermined )
      s = undetermined;
    else if( r == true_ )
      return true_;
  }
  return s;
};

bool
geomObj::VolumeIntersection( const geomObj& p1, const geomObj& p2 )
{
  testResult s = p1.intersectsVolume( p2 );
  if( s == undetermined )
    s = p2.intersectsVolume( p1 );
  return s == true_;
}

