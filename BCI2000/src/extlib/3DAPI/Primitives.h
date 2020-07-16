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
#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "component.h"

struct Transform
{
  Transform();
  CVector3 Apply( const CVector3&, float ) const;
  GLfloat m[16];
};

class Primitive
{
public:
  Primitive() {}
  Primitive( const CVector3& pos, const CVector3& extent ) : mPosition( pos ), mExtent( extent ) {}
  const CVector3& Position() const { return mPosition; }
  const CVector3& Extent() const { return mExtent; }

  float MinimumSquaredDistance( const CVector3& point ) const;
  virtual CVector3 ClosestPoint( const CVector3& ) const = 0;
  virtual void Apply( const Transform& );

private:
  CVector3 mPosition, mExtent;
};

struct Line : Primitive
{
  Line() {}
  Line( const CVector3& pos, const CVector3& dir ) : Primitive( pos, dir ) {}
  CVector3 ClosestPoint( const CVector3& ) const override;

};

struct Vertex : Line
{
  Vertex() {}
  Vertex( const CVector3& point, const CVector3& length ) : Line( point, length ) {}
  CVector3 ClosestPoint( const CVector3& ) const override;
};

struct Plane : Primitive
{
  Plane() {}
  Plane( const CVector3& pos, const CVector3& normal ) : Primitive( pos, normal ) {}
  CVector3 ClosestPoint( const CVector3& ) const override;

  bool Intersects( const Line& ) const;
  CVector3 IntersectionPoint( const Line& ) const;
  bool Intersects( const Vertex& ) const;
};

class Triangle : public Plane
{
public:
  Triangle() {}
  Triangle( const CVector3& x1, const CVector3& x2, const CVector3& x3 );
  const Vertex& Vertex( size_t i ) const { return mVertices[i]; }
  CVector3 ClosestPoint( const CVector3& ) const override;
  bool Intersects( const Primitive& ) const;
  virtual void Apply( const Transform& ) override;

private:
  bool IsPlanePointWithin( const CVector3& ) const;
  struct Vertex mVertices[3];
};

typedef std::vector<Triangle> TriangulatedSurface;

#endif // PRIMITIVES_H
