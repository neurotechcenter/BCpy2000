////////////////////////////////////////////////////////////////////////////////
// $Id: Scene.cpp 5492 2016-08-04 16:47:13Z mellinger $
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
#include "Scene.h"

#include "GraphDisplay.h"
#include "OpenGLContext.h"
#include "FileUtils.h"
#include "Exception.h"

Scene::Scene( GUI::GraphDisplay& inDisplay )
: GUI::GraphObject( inDisplay, SceneDisplayZOrder ),
  mColor( RGBColor::NullColor ),
  mInitialized( false ),
  mDestroyed( false ),
  mpGLContext( nullptr ),
  mpGLAllocationContext( nullptr ),
  mfpOnCollide( nullptr )
{
}

Scene::~Scene()
{
  DestructorEntered();
  mDestroyed = true;
  DeleteObjects();
  delete mpGLAllocationContext;
}

void
Scene::Add( primObj* p )
{
  if( mInitialized )
    throw std_logic_error <<
      "Cannot add object to scene in initialized state. "
      "Call Scene::Cleanup() to de-initialize the scene.";
  WithLock( this )
    mObjects.insert( p );
}

void
Scene::Remove( primObj* p )
{
  if( mInitialized )
    throw std_logic_error <<
      "Cannot remove object from scene in initialized state. "
      "Call Scene::Cleanup() to de-initialize the scene.";
  WithLock( this )
    mObjects.erase( p );
}

void
Scene::DeleteObjects()
{
  WithLock( this )
  {
    if( mpGLAllocationContext )
      mpGLAllocationContext->MakeCurrent();
    OnCleanupGL();
    while( !mObjects.empty() )
      delete *mObjects.begin();
    if( mpGLAllocationContext )
      mpGLAllocationContext->DoneCurrent();
  }
}

void
Scene::OnPaint( const GUI::DrawContext& inDC )
{
  OnPaintGL();
}

void
Scene::OnChange( GUI::DrawContext& inDC )
{
  if( mpGLContext != inDC.gl )
  {
    if( mpGLAllocationContext )
    {
      WithGLContext( mpGLAllocationContext )
        OnCleanupGL();
      delete mpGLAllocationContext;
      mpGLAllocationContext = nullptr;
    }
    if( inDC.gl )
    {
      mpGLAllocationContext = new GUI::OpenGLContext( GUI::NoWindow, inDC.gl );
      WithGLContext( mpGLAllocationContext )
        OnInitializeGL();
    }
  }
  mpGLContext = inDC.gl;
}

void
Scene::OnInitializeGL()
{
  static const ClockTick nullTick = { 0 };
  mLastTick = nullTick;
  std::string cwd = FileUtils::WorkingDirectory();
  if( !mImagePath.empty() )
    FileUtils::ChangeDirectory( FileUtils::AbsolutePath( mImagePath ) );
  if( !mDestroyed ) WithLock( this )
    for( ObjectIterator i = mObjects.begin(); i != mObjects.end(); ++i )
      ( *i )->initialize();
  FileUtils::ChangeDirectory( cwd.c_str() );
  mInitialized = true;
}

void
Scene::OnCleanupGL()
{
  if( mInitialized && !mDestroyed ) WithLock( this )
    for( ObjectIterator i = mObjects.begin(); i != mObjects.end(); ++i )
      ( *i )->cleanup();
  mInitialized = false;
}

void
Scene::OnPaintGL()
{
  mCameraAndLight.apply();

  if( !mDestroyed ) WithLock( this )
  {
    DrawingOrderedSetOfObjects s;
    for( ObjectIterator i = mObjects.begin(); i != mObjects.end(); ++i )
      s.insert( *i );

    for( DrawingOrderedIterator i = s.begin(); i != s.end(); ++i )
      ( *i )->render();
  }
}

void
Scene::Move( float inDeltaT )
{
  Assert( inDeltaT > -Inf( inDeltaT ) && inDeltaT < Inf( inDeltaT ) );
  typedef std::set<sceneObj*> SetOfSceneObjects;
  typedef SetOfSceneObjects::const_iterator SceneIterator;
  SetOfSceneObjects sceneObjects;
  bool changed = false;
  if( !mDestroyed ) WithLock( this )
  {
    for( ObjectIterator i = mObjects.begin(); i != mObjects.end(); ++i )
    {
      sceneObj* p = dynamic_cast<sceneObj*>( *i );
      if( p )
      {
        sceneObjects.insert( p );
        changed = p->primObj::changed() || changed;
      }
    }
    // Apply movement
    for( SceneIterator i = sceneObjects.begin(); i != sceneObjects.end(); ++i )
      changed = ( *i )->move( inDeltaT ) || changed;

    // Test for collisions
    if( changed && mfpOnCollide )
      for( SceneIterator i = sceneObjects.begin(); i != sceneObjects.end(); ++i )
        for( SceneIterator j = sceneObjects.begin(); j != i; ++j )
          if( sceneObj::VolumeIntersection( **i, **j ) )
            mfpOnCollide( **i, **j );
  }
  if( changed )
    Invalidate();
}

void
Scene::OnTick( const ClockTick& t )
{
  double dt = ( t.time - mLastTick.time ).Seconds();
  mLastTick = t;
  bool ok = ( dt > -Inf(dt) && dt < Inf(dt) );
  if( ok )
    Move( dt );
}
