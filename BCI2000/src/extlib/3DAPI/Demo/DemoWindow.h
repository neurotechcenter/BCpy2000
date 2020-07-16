////////////////////////////////////////////////////////////////////////////////
// $Id: DemoWindow.h 4808 2015-04-24 22:53:43Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Main window for an application demonstrating the use of
//   the 3D Scene GraphObject class.
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
#ifndef DEMO_WINDOW_H
#define DEMO_WINDOW_H
//---------------------------------------------------------------------------
#include <QWindow>
#include "GraphDisplay.h"

class Scene;
class sceneObj;
class twoDText;
class twoDCursor;
class twoDOverlay;

class DemoWindow : public QWindow
{
  Q_OBJECT
 
 public:
  static const int cFrameUpdateMs = 50;
  DemoWindow();
  ~DemoWindow();

 protected:
  virtual void exposeEvent( QExposeEvent* );
  virtual void keyPressEvent( QKeyEvent* );
  virtual void timerEvent( QTimerEvent* );

 private:
  void Render();
  void StartUpdateTimer();
  void StopUpdateTimer();
  void NewScene();
  void PopulateScene();
  void ResetPositions();
  void StopMovement();
  void RandomTranslation( float speed );
  void RandomRotation( float speed );
  static void RandomTranslation( sceneObj&, float speed );
  static void RandomRotation( sceneObj&, float speed );
  static float Randf();
  static void OnCollide( sceneObj&, sceneObj& );

  sceneObj*    mpBound,
          *    mpEarth,
          *    mpMoon,
          *    mpCube,
          *    mpCuboid,
          *    mp3DText,
          *    mpFace;
  twoDText*    mpHelpText,
          *    mpFPSText;
  twoDCursor*  mpCursor;
  twoDOverlay* mpOverlay;

  int mTimerID;
  float mFieldOfView,
        mSum,
        mCount;

  Scene*            mpScene;
  GUI::GraphDisplay mGraphDisplay;
};

#endif // DEMO_WINDOW_H
