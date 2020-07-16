////////////////////////////////////////////////////////////////////////////////
// $Id: Scene.h 5318 2016-04-20 19:13:13Z mellinger $
// Authors: shzeng, schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A 3D scene viewed through a rectangular region.
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
#ifndef SCENE_H
#define SCENE_H

#include "GraphObject.h"
#include "Uncopyable.h"
#include "Lockable.h"
#include "primObj.h"
#include "cameraNlight.h"

#include <set>

class Scene : public GUI::GraphObject, private Lockable<RecursiveMutex>, Uncopyable
{
 public:
  Scene( GUI::GraphDisplay& );
  virtual ~Scene();

  // Properties
  Scene& SetColor( RGBColor c )
    { mColor = c; return *this; }
  RGBColor Color() const
    { return mColor; }
  Scene& SetImagePath( const std::string& p )
    { mImagePath = p; Change(); return *this; }
  const std::string& ImagePath() const
    { return mImagePath; }

  Scene& SetCameraX( float f )
    { mCameraAndLight.setCamViewPoint( f, CameraY(), CameraZ() ); return *this; }
  float CameraX() const
    { return mCameraAndLight.getCamViewX(); }
  Scene& SetCameraY( float f )
    { mCameraAndLight.setCamViewPoint( CameraX(), f, CameraZ() ); return *this; }
  float CameraY() const
    { return mCameraAndLight.getCamViewY(); }
  Scene& SetCameraZ( float f )
    { mCameraAndLight.setCamViewPoint( CameraX(), CameraY(), f ); return *this; }
  float CameraZ() const
    { return mCameraAndLight.getCamViewZ(); }

  cameraNLight& CameraAndLight()
    { return mCameraAndLight; }

  void Add( primObj* );
  void Remove( primObj* );
  void DeleteObjects();

  typedef std::set<primObj*> SetOfObjects;
  typedef Scene::SetOfObjects::const_iterator ObjectIterator;
  const SetOfObjects& Objects()
    { return mObjects; }

  typedef std::set<primObj*, primObj::compareByDrawingOrder> DrawingOrderedSetOfObjects;
  typedef Scene::DrawingOrderedSetOfObjects::const_iterator DrawingOrderedIterator;

  void Move( float deltaT ); // moves all objects according to their velocities, and tests for
                             // collisions

  typedef void (*OnCollideFunc)( sceneObj&, sceneObj& );
  Scene& SetOnCollide( OnCollideFunc c )
    { mfpOnCollide = c; return *this; }

 protected:
  // GraphObject event handlers
  void OnPaint( const GUI::DrawContext& );
  void OnChange( GUI::DrawContext& );
  void OnTick( const ClockTick& );
  bool IsRenderObject() const
    { return true; }

private:
  // Event handlers executing GL commands
  void OnInitializeGL();
  void OnCleanupGL();
  void OnPaintGL();

 private:
  RGBColor mColor;
  bool  mInitialized;
  GUI::OpenGLContext* mpGLContext, *mpGLAllocationContext;
  // Scene Objects
  SetOfObjects  mObjects;
  Synchronized<bool> mDestroyed;
  cameraNLight  mCameraAndLight;
  std::string   mImagePath; // path to images to load

  OnCollideFunc mfpOnCollide;
  ClockTick mLastTick;
};

#endif // SCENE_H
