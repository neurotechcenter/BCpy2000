////////////////////////////////////////////////////////////////////////////////
// $Id: FeedbackScene3D.h 4924 2015-07-28 12:30:32Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A 3D feedback scene using the OpenGL-based 3D API.
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
#ifndef FEEDBACK_SCENE_3D_H
#define FEEDBACK_SCENE_3D_H

#include "FeedbackScene.h"

#include "Scene.h"
#include "sphere.h"
#include "cuboids.h"
#include "threeDText.h"
#include "DisplayWindow.h"

class FeedbackScene3D : public FeedbackScene
{
 public:
  FeedbackScene3D( GUI::DisplayWindow& );
  virtual ~FeedbackScene3D();

 protected:
  void OnInitialize() override;
  void OnSetCursorPosition( const Vector3D& ) override;

  float CursorRadius() const override;
  void SetCursorVisible( bool ) override;
  void SetCursorColor( RGBColor ) override;

  int NumTargets() const override;
  bool TargetHit( int ) const override;
  float CursorTargetDistance( int ) const override;
  void SetTargetVisible( bool, int ) override;
  void SetTargetColor( RGBColor, int ) override;

 private:
  void ClearObjects();

  GUI::DisplayWindow&  mDisplay;
  Scene*               mpScene;
  sphere*              mpCursor;
  invertedCuboid*      mpBoundary;
  std::vector<cuboid*> mTargets;
};

#endif // FEEDBACK_SCENE_3D_H
