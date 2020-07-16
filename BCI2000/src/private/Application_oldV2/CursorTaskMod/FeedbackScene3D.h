////////////////////////////////////////////////////////////////////////////////
// $Id: FeedbackScene3D.h 2182 2008-11-05 16:52:05Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A 3D feedback scene using the OpenGL-based 3D API.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef FEEDBACK_SCENE_3D_H
#define FEEDBACK_SCENE_3D_H

#include "FeedbackScene.h"

#include "Scene.h"
#include "Sphere.h"
#include "Cuboids.h"
#include "threeDText.h"
#include "DisplayWindow.h"

class FeedbackScene3D : public FeedbackScene
{
  typedef FeedbackScene3D self;

 public:
  FeedbackScene3D( GUI::DisplayWindow& );
  virtual ~FeedbackScene3D();

  virtual self& Initialize();

  virtual float CursorRadius() const;
  virtual float CursorXPosition() const;
  virtual float CursorYPosition() const;
  virtual float CursorZPosition() const;
  virtual self& SetCursorPosition( float x, float y, float z );
  virtual self& SetCursorVisible( bool );
  virtual self& SetCursorColor( RGBColor );

  virtual int   NumTargets() const;
  virtual bool  TargetHit( int ) const;
  virtual float CursorTargetDistance( int ) const;
  virtual self& SetTargetVisible( bool, int );
  virtual self& SetTargetColor( RGBColor, int );

  // Feedback Start -->
  void ShowFixation( bool show );
  // <-- Feedback End

 private:
  void ClearObjects();

  GUI::DisplayWindow&  mDisplay;
  Scene*               mpScene;
  sphere*              mpCursor;
  invertedCuboid*      mpBoundary;
  std::vector<cuboid*> mTargets;

  // Feedback Start -->
  threeDText* mpFixationCross;
  int mFixationCrossSize;
  // <-- Feedback End
};

#endif // FEEDBACK_SCENE_3D_H
