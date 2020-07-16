////////////////////////////////////////////////////////////////////////////////
// $Id: sphere.cpp 4924 2015-07-28 12:30:32Z mellinger $
// Authors: shzeng, schalk@wadsworth.org
// Description: A 3D sphere.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "sphere.h"
#include "Numeric.h"

geomObj::testResult
sphere::contains( const CVector3& inPoint ) const
{
  CVector3 center = getOrigin();
  return ( inPoint - center ) * ( inPoint - center ) <= mRadius * mRadius
         ? true_
         : false_;
}

geomObj::testResult
sphere::intersects( const Primitive& p ) const
{
  return p.MinimumSquaredDistance( getOrigin() ) <= mRadius * mRadius ? true_ : false_;
}

geomObj::testResult
sphere::intersectsSurface( const geomObj& inObj ) const
{
  const sphere* pSphere = dynamic_cast<const sphere*>( &inObj );
  if( pSphere )
  {
    CVector3 center1 = getOrigin(),
             center2 = pSphere->getOrigin();
    float radSum = mRadius + pSphere->mRadius;
    return ( center1 - center2 ) * ( center1 - center2 ) <= radSum * radSum ? true_ : false_;
  }
  return geomObj::intersectsSurface( inObj );
}

void
sphere::onRender() const
{
  GLUquadricObj* q = gluNewQuadric();  // Create A New Quadric
  gluQuadricNormals( q, GL_SMOOTH );   // Generate Smooth Normals For The Quad
  gluQuadricTexture( q, GL_TRUE );     // Enable Texture Coords For The Quad
  gluSphere( q, mRadius, 100, 100 ); // Draw  Sphere
  gluDeleteQuadric( q );               // free the memory
}

