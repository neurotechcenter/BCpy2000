////////////////////////////////////////////////////////////////////////////////
// $Id$
// Authors: juergen.mellinger@uni-tuebingen.de
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
#include "Primitives.h"
#include "Numeric.h"
#include "Debugging.h"

Transform::Transform()
{ // identity
  for( size_t i = 0; i < sizeof( m ) / sizeof( *m ); ++i )
    m[ i ] = ( i % 5 ) ? 0 : 1;
}

CVector3 Transform::Apply( const CVector3& in, float c ) const
{
  GLfloat v[] = { in.x, in.y, in.z, c };
  CVector3 out;
  for( int i = 0; i < 3; ++i )
  {
    out.data()[i] = 0;
    for( int j = 0; j < 4; ++j )
      out.data()[i] += v[j] * m[i + 4*j];
  }
  return out;
}

void
Primitive::Apply( const Transform& t )
{
  mPosition = t.Apply( mPosition, 1 );
  mExtent = t.Apply( mExtent, 0 );
}

float
Primitive::MinimumSquaredDistance( const CVector3& p ) const
{
  CVector3 v = ClosestPoint( p ) - p;
  return v * v;
}

CVector3
Line::ClosestPoint( const CVector3& p ) const
{
  float sqNorm = Extent() * Extent();
  if( sqNorm < Eps( sqNorm ) )
    return Position();
  return Position() + ( ( p - Position() ) * Extent() ) * Extent() / sqNorm;
}

CVector3
Plane::ClosestPoint( const CVector3& p ) const
{
  float sqNorm = Extent() * Extent();
  if( sqNorm < Eps( sqNorm ) )
    return Position();
  return p + ( ( Position() - p ) * Extent() ) * Extent() / sqNorm;
}

CVector3
Vertex::ClosestPoint( const CVector3& p ) const
{
  CVector3 p1 = Line::ClosestPoint( p );
  float d = ( p1 - Position() ) * Extent();
  if( d > Extent() * Extent() )
    return Position() + Extent();
  if( d < 0 )
    return Position();
  return p1;
}

bool
Plane::Intersects( const Line& line ) const
{
  return ::fabs( line.Extent() * Extent() ) > Eps<float>();
}

CVector3
Plane::IntersectionPoint( const Line& line ) const
{
  float d = ( ( Position() - line.Position() ) * Extent() ) / ( line.Extent() * Extent() );
  return line.Position() + d * line.Extent();
}

bool
Plane::Intersects( const Vertex& vertex ) const
{
  if( !Intersects( static_cast<const Line&>( vertex ) ) )
    return false;
  CVector3 p1 = vertex.Position(), p2 = vertex.Position() + vertex.Extent();
  return ( ( p1 - Position() ) * Extent() ) * ( ( p2 - Position() ) * Extent() ) < 0;
}

Triangle::Triangle( const CVector3& x1, const CVector3& x2, const CVector3& x3 )
: Plane( ( x1 + x2 + x3 ) / 3, CrossProd( x2 - x1, x3 - x2 ) * 0.5 )
{
  mVertices[0] = struct Vertex( x1, x2 - x1 );
  mVertices[1] = struct Vertex( x2, x3 - x2 );
  mVertices[2] = struct Vertex( x3, x1 - x3 );
}

void
Triangle::Apply( const Transform& t )
{
  for( int i = 0; i < 3; ++i )
    mVertices[i].Apply( t );
  Plane::Apply( t );
}

CVector3
Triangle::ClosestPoint( const CVector3& p ) const
{
  CVector3 p1 = Plane::ClosestPoint( p );
  if( IsPlanePointWithin( p1 ) )
    return p1;
  CVector3 closest[3];
  float min = Inf( min );
  int idx = -1;
  for( int i = 0; i < 3; ++i )
  {
    float d = Vertex( i ).MinimumSquaredDistance( p );
    if( d < min )
      min = d, idx = i;
  }
  return Vertex( idx ).ClosestPoint( p );
}

bool
Triangle::Intersects( const Primitive& other ) const
{
  const Plane* plane = dynamic_cast<const Plane*>( &other );
  if( plane )
  {
    const Triangle* t = dynamic_cast<const Triangle*>( plane );
    if( t )
    {
      for( int i = 0; i < 3; ++i )
      {
        if( Intersects( t->Vertex( i ) ) )
          return true;
        if( t->Intersects( Vertex( i ) ) )
          return true;
      }
    }
    else
      for( int i = 0; i < 3; ++i )
        if( plane->Intersects( Vertex( i ) ) )
          return true;
    return false;
  }

  const Line* line = dynamic_cast<const Line*>( &other );
  if( line )
  {
    if( !Plane::Intersects( *line ) )
      return false;
    CVector3 p = Plane::IntersectionPoint( *line );
    if( !IsPlanePointWithin( p ) )
      return false;
    return line->MinimumSquaredDistance( p ) < Eps<float>();
  }
  return false;
}

bool
Triangle::IsPlanePointWithin( const CVector3& p ) const
{
  float c[3];
  for( int i = 0; i < 3; ++i )
    c[i] = Extent() * CrossProd( Vertex( i ).Extent(), p - Vertex( i ).Position() );
  if( c[0] > 0 && c[1] > 0 && c[2] > 0 )
    return true;
  Assert( !(c[0] < 0 && c[1] < 0 && c[2] < 0) );
  return false;
}
