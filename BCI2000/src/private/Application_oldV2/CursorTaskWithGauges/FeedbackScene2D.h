////////////////////////////////////////////////////////////////////////////////
// $Id: FeedbackScene2D.h 2182 2008-11-05 16:52:05Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A 2D feedback scene implemented using 2D GraphObjects.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#ifndef FEEDBACK_SCENE_2D_H
#define FEEDBACK_SCENE_2D_H

#include "FeedbackScene.h"
#include "Shapes.h"
#include "DisplayWindow.h"

class FeedbackScene2D : public FeedbackScene
{
 typedef FeedbackScene2D self;
 
 public:
  FeedbackScene2D( GUI::DisplayWindow& );
  virtual ~FeedbackScene2D();

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

 private:
  void ClearObjects();
  enum { point, vector };
  void SceneToObjectCoords( GUI::Point&, int kind ) const;
  void ObjectToSceneCoords( GUI::Point&, int kind ) const;

  GUI::DisplayWindow&            mDisplay;
  float                          mScalingX,
                                 mScalingY,
                                 mCursorZ;
  EllipticShape*                 mpCursor;
  RectangularShape*              mpBoundary;
  std::vector<RectangularShape*> mTargets;
};

#endif // FEEDBACK_SCENE_2D_H
