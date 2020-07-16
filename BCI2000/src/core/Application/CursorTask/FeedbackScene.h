////////////////////////////////////////////////////////////////////////////////
// $Id: FeedbackScene.h 4924 2015-07-28 12:30:32Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An abstract class interface for feedback scenes.
//   A feedback scene consists of a cursor and a number of targets.
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
#ifndef FEEDBACK_SCENE_H
#define FEEDBACK_SCENE_H

#include "Environment.h"
#include "Color.h"

#include "geomObj.h"
typedef CVector3 Vector3D;

class FeedbackScene : protected Environment
{
  typedef FeedbackScene self;

public:
  FeedbackScene()
    {}
  virtual ~FeedbackScene()
    {}

  void Initialize()
    {
      ::memset( &mPos, 0, sizeof(mPos) );
      ::memset( &mVel, 0, sizeof(mVel) );
      OnInitialize();
    }

  self& SetCursorPosition( const Vector3D& pos )
    {
      mPos = pos;
      OnSetCursorPosition( pos );
      return *this;
    }  
  const Vector3D& CursorPosition() const
    { return mPos; }

  self& SetCursorVelocity( const Vector3D& v )
    {
      mVel = v;
      OnSetCursorVelocity( v );
      return *this;
    }
  const Vector3D& CursorVelocity() const
    { return mVel; }

  virtual float CursorRadius() const = 0;

  virtual void SetCursorVisible( bool ) = 0;
  virtual void SetCursorColor( RGBColor ) = 0;

  virtual int NumTargets() const = 0;
  virtual bool TargetHit( int ) const = 0;
  virtual float CursorTargetDistance( int ) const = 0;
  virtual void SetTargetVisible( bool, int ) = 0;
  virtual void SetTargetColor( RGBColor, int ) = 0;

protected:
  virtual void OnInitialize() = 0;
  virtual void OnSetCursorPosition( const Vector3D& ) = 0;
  virtual void OnSetCursorVelocity( const Vector3D& ) {}

 private:
  Vector3D mPos, mVel;
};

#endif // FEEDBACK_SCENE_H
