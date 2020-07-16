////////////////////////////////////////////////////////////////////////////////
// $Id: FeedbackScene.h 2182 2008-11-05 16:52:05Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An abstract class interface for feedback scenes.
//   A feedback scene consists of a cursor and a number of targets.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef FEEDBACK_SCENE_H
#define FEEDBACK_SCENE_H

#include "Environment.h"
#include "Color.h"

class FeedbackScene : protected Environment
{
 typedef FeedbackScene self;

 public:
  FeedbackScene()
    {}
  virtual ~FeedbackScene()
    {}

  virtual self& Initialize() = 0;

  virtual float CursorRadius() const = 0;
  virtual float CursorXPosition() const = 0;
  virtual float CursorYPosition() const = 0;
  virtual float CursorZPosition() const = 0;
  virtual self& SetCursorPosition( float x, float y, float z ) = 0;
  virtual self& SetCursorVisible( bool ) = 0;
  virtual self& SetCursorColor( RGBColor ) = 0;

  virtual int   NumTargets() const = 0;
  virtual bool  TargetHit( int ) const = 0;
  virtual float CursorTargetDistance( int ) const = 0;
  virtual self& SetTargetVisible( bool, int ) = 0;
  virtual self& SetTargetColor( RGBColor, int ) = 0;
};

#endif // FEEDBACK_SCENE_H
