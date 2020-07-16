////////////////////////////////////////////////////////////////////////////////
// $Id: UDemoWindow.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Main form for an application demonstrating the use of
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
#ifndef UDemoWindowH
#define UDemoWindowH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "GraphDisplay.h"
#include <ExtCtrls.hpp>

class Scene;
class sceneObj;
class twoDText;
class twoDCursor;
class twoDOverlay;

//---------------------------------------------------------------------------
class TDemoWindow : public TForm
{
__published:    // IDE-managed Components
    TTimer *FrameUpdate;
    void __fastcall FormPaint(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall FrameUpdateTimer(TObject *Sender);

private:    // User declarations
    void __fastcall WMEraseBkgnd( TWMEraseBkgnd& ) {}
    BEGIN_MESSAGE_MAP
      VCL_MESSAGE_HANDLER( WM_ERASEBKGND, TWMEraseBkgnd, WMEraseBkgnd )
    END_MESSAGE_MAP( TForm )

  void SetupScene();
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

  float mFieldOfView;

  Scene*            mpScene;
  GUI::GraphDisplay mGraphDisplay;
  
public:     // User declarations
    __fastcall TDemoWindow(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDemoWindow *DemoWindow;
//---------------------------------------------------------------------------
#endif
