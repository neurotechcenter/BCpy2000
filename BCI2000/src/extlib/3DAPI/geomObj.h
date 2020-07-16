////////////////////////////////////////////////////////////////////////////////
// $Id: geomObj.h 5492 2016-08-04 16:47:13Z mellinger $
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
#ifndef GEOM_OBJ_H
#define GEOM_OBJ_H

#include "Primitives.h"

class geomObj
{
 private:
  Transform mWorldTransform;  // Matrix representing the object's position,
                              // orientation, and scale in space.
  CVector3 mVelocity,         // Velocities of movement
           mAngVelocity;
  bool mMoved;
  TriangulatedSurface mTriangles;

 public:
  geomObj();
  virtual ~geomObj() {}
  const GLfloat* getWorldTransform() const
        { return mWorldTransform.m; }

  // Set/get the current proper system's origin and axes as stored in the
  // world transform matrix.
  //  Typically, the proper system's origin is the object's center of mass.
  //  For convex objects, it is assumed that the origin belongs to it
  //  (in the sense that it is part of the object's solid volume).
  void setOrigin( const CVector3& );
  void setOrigin( GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; setOrigin( c ); }
  CVector3 getOrigin() const;
  float getOriginX() const
        { return getOrigin().x; }
  float getOriginY() const
        { return getOrigin().y; }
  float getOriginZ() const
        { return getOrigin().z; }

  void setXAxis( const CVector3& );
  void setXAxis( GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; setXAxis( c ); }
  CVector3 getXAxis() const;
  float getXAxisX() const
        { return getXAxis().x; }
  float getXAxisY() const
        { return getXAxis().y; }
  float getXAxisZ() const
        { return getXAxis().z; }

  void setYAxis( const CVector3& );
  void setYAxis( GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; setYAxis( c ); }
  CVector3 getYAxis() const;
  float getYAxisX() const
        { return getYAxis().x; }
  float getYAxisY() const
        { return getYAxis().y; }
  float getYAxisZ() const
        { return getYAxis().z; }

  void setZAxis( const CVector3& );
  void setZAxis( GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; setZAxis( c ); }
  CVector3 getZAxis() const;
  float getZAxisX() const
        { return getZAxis().x; }
  float getZAxisY() const
        { return getZAxis().y; }
  float getZAxisZ() const
        { return getZAxis().z; }

  // Movement parameters
  void setVelocity( const CVector3& v )
        { mVelocity = v; }
  void setVelocity( GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; setVelocity( c ); }
  const CVector3& getVelocity() const
        { return mVelocity; }
  void setAngVelocity( const CVector3& v )
        { mAngVelocity = v; }
  void setAngVelocity( GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; setAngVelocity( c ); }
  const CVector3& getAngVelocity() const
        { return mAngVelocity; }

  // Transformations of the object's coordinate system
  // Translate
  void translate( const CVector3& );
  void translate( GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; translate( c ); }
  // Rotate around an axis through the global origin
  void rotate( const CVector3& vecAngle );
  void rotate( GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; rotate( c ); }
  void rotate( GLfloat angle, const CVector3& axis );
  void rotate( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; rotate( angle, c ); }
  // Rotate around an axis through the object's center
  void spin( const CVector3& vecAngle );
  void spin( GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; spin( c ); }
  void spin( GLfloat angle, const CVector3& axis );
  void spin( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
        { CVector3 c = { x, y, z }; spin( angle, c ); }
  // Scale without affecting the object's position
  void scale( const CVector3& v )
        { scale( v.x, v.y, v.z ); }
  void scale( GLfloat x, GLfloat y, GLfloat z );

  // Geometric properties related to hit testing
  enum testResult { false_ = 0, true_ = 1, undetermined };
  virtual testResult isConvex() const
        { return undetermined; };
  virtual testResult contains( const CVector3& ) const
        { return undetermined; };
  virtual testResult intersects( const Primitive& ) const;
  virtual testResult intersectsSurface( const geomObj& ) const;
  virtual testResult intersectsVolume( const geomObj& ) const;
  static bool VolumeIntersection( const geomObj&, const geomObj& );

  const TriangulatedSurface& Triangles() const
        { return mTriangles; }

  // Events for geomObjs:
  bool move( float dt )
        { onMove( dt ); return moved(); }

 protected:
  TriangulatedSurface& Triangles()
        { return mTriangles; }
  virtual void onMove( float dt )
        { translate( dt*mVelocity ); spin( dt*mAngVelocity ); }
  bool movedIf( bool b )
        { mMoved = mMoved || b; return b; }
  bool moved()
        { bool b = mMoved; mMoved = false; return b; }
 private:
  void setOriginPrivate( const CVector3& );
  void setXAxisPrivate( const CVector3& );
  void setYAxisPrivate( const CVector3& );
  void setZAxisPrivate( const CVector3& );
};//geomObj



#endif // GEOM_OBJ_H


