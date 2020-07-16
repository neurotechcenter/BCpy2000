/////////////////////////////////////////////////////////////////////////////
//
// File: GUIView.cpp
//
// Date: Nov 7, 2001
//
// Author: Juergen Mellinger
//
// Description: The TGUIView class contains GUI specific
//              functionality common to all views.
//
// Changes: Mar 17, 2003, jm:
//          Introduced separate handling of update region for fast updates
//          without flicker.
//          Mar 18, 2003, jm:
//          Introduced VBL synchronization through DirectDraw.
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
//////////////////////////////////////////////////////////////////////////////

#ifdef __BORLANDC__
#include "PCHIncludes.h"
#pragma hdrstop
#endif // __BORLANDC__

#include "OSIncludes.h"

#ifndef VCL
# error This is the VCL implementation of TGUIView.
#endif

#include <cassert>

#include "GUIView.h"
#include "Utils/Util.h"
#include "ViewsRes.h"
#ifdef VBL_SYNC
# include "DirectDraw.h"
#endif // VBL_SYNC

// Initial window properties.
const int   invalidCoord = 32767;

// Static data members.
int                 TGUIView::numInstances = 0;
TGUIView::TFBForm*  TGUIView::FBForm = NULL;
int                 TGUIView::TFBForm::lastWidth = 400;
int                 TGUIView::TFBForm::lastHeight = 300;
int                 TGUIView::TFBForm::lastLeft = invalidCoord;
int                 TGUIView::TFBForm::lastTop = invalidCoord;
TWindowState        TGUIView::TFBForm::lastWindowState = wsMinimized;
// Initial values in the order given by the TGUIElements enumeration in SWGUI.h
TGUIView::TRGBColor TGUIView::ElementColors[ numGUIElements ] =
{
  clNavy,     //  fbBackground
  clGray,     //  fbZeroLine
  clPurple,   //  targetFillNormal
  clGreen,    //  targetFillBlinking
  clBlue,     //  targetFillActive
  clGreen,    //  targetBorderNormal
  clGreen,    //  targetBorderBlinking
  clYellow,   //  targetBorderActive
  clGreen,    //  targetTextNormal
  clGreen,    //  targetTextBlinking
  clYellow,   //  targetTextActive
  clWhite,    //  textFrameBackground
  clYellow    //  scoreViewText
};
int                 TGUIView::ElementWidths[ numGUIElements ] =
{
  0,  //  fbBackground
  0,  //  fbZeroLine
  0,  //  targetFillNormal
  0,  //  targetFillBlinking
  0,  //  targetFillActive
  10, //  targetBorderNormal
  10, //  targetBorderBlinking
  10, //  targetBorderActive
  0,  //  targetTextNormal
  0,  //  targetTextBlinking
  0,  //  targetTextActive
  0,  //  textFrameBackground
  0   //  scoreViewText
};

// Member functions.
TGUIView::TGUIView( TViewZ inZPosition )
: zPosition( inZPosition ),
  viewRect( 0.0, 0.0, 0.0, 0.0 ),
  viewTRect( 0, 0, 0, 0 )
{
  ++numInstances;
  if( FBForm == NULL )
    // Create the feedback form. It is just an empty form.
    // All views draw themselves into its canvas.
    FBForm = new TFBForm;

  FBForm->AttachView( this );
}

TGUIView::~TGUIView()
{
  FBForm->DetachView( this );
  --numInstances;
  if( numInstances < 1 )
  {
    delete FBForm;
    FBForm = NULL;
  }
}

void
TGUIView::Resized()
{
  float   width = ( float )FBForm->ClientWidth,
          height = ( float )FBForm->ClientHeight;
  viewTRect.Left = ROUND( width * viewRect.left );
  viewTRect.Top = ROUND( height * viewRect.top );
  viewTRect.Right = ROUND( width * viewRect.right );
  viewTRect.Bottom = ROUND( height * viewRect.bottom );
  TGUIView::InvalidateRect( viewTRect );
  
  assert( viewTRect.Width() >= 0 && viewTRect.Height() >= 0 );
}

// Class functions.
TPresError
TGUIView::SetStyle( const char  *inStyleName )
{
  // Load the colors and widths of view elements from a bitmap resource or file.
  // The rows of the bitmap represent the elements in the order given by
  // the TGUIElements enumeration in SWGUI.h. An element's width is taken
  // by counting the number of pixels in its row that have the same color
  // as the leftmost one.

  if( inStyleName == NULL || *inStyleName == '\0' )
    return presNoError;

  TPresError  result = presNoError;
  Graphics::TBitmap   *bitmap = new Graphics::TBitmap();

  std::string fileName( inStyleName );
  for( std::string::iterator i = fileName.begin(); i != fileName.end(); ++i )
    *i = toupper( *i );
  fileName += ".BMP";

  // Look for a bitmap resource for the style given.
  try
  {
    bitmap->LoadFromResourceName( ( int )HInstance, fileName.c_str() );
  }
  catch( const EResNotFound& )
  {
    result = presResNotFoundError;
  }

  // No resource found, look for a bitmap file in the application default path.
  if( result == presResNotFoundError )
  {
    result = presNoError;
    try
    {
      bitmap->LoadFromFile( fileName.c_str() );
    }
    catch( ... )
    {
      result = presFileOpeningError;
    }
  }

  // No file found, look in the resource directory.
  if( result == presFileOpeningError )
  {
    result = presNoError;

    Util::TPath curPath;
    fileName = curPath + fileName;

    try
    {
      bitmap->LoadFromFile( fileName.c_str() );
    }
    catch( ... )
    {
      result = presFileOpeningError;
      gPresErrors.AddError( result, fileName.c_str() );
    }
  }

  if( result == presNoError )
  {
    for( int i = 0; i < MIN( int( numGUIElements ), bitmap->Height ); ++i )
    {
      TColor  curColor = bitmap->Canvas->Pixels[ 0 ][ i ];
      ElementColors[ i ].cl = curColor;
      int j = 0;
      while( j < bitmap->Width && bitmap->Canvas->Pixels[ j ][ i ] == curColor )
        ++j;
      ElementWidths[ i ] = j;
    }
  }

  delete bitmap;
  return result;
}

void
TGUIView::InvalidateRect( const TRect& inTRect )
{
#ifndef VCL_DOUBLEBUF
  if( FBForm != NULL )
    FBForm->InvalidateRect( inTRect );
#endif // VCL_DOUBLEBUF
}

void
TGUIView::RefreshWindow()
{
  if( FBForm != NULL )
  {
#ifdef VCL_DOUBLEBUF
    FBForm->Invalidate();
#endif // VCL_DOUBLEBUF
    FBForm->Update();
  }
}

void
TGUIView::ResizeWindow( int inWidth, int inHeight )
{
  if( FBForm != NULL )
  {
    FBForm->Width = inWidth;
    FBForm->Height = inHeight;
  }
}

void
TGUIView::MoveWindow( int inLeft, int inTop )
{
  if( FBForm != NULL )
  {
    FBForm->Left = inLeft;
    FBForm->Top = inTop;
  }
}

void
TGUIView::ShowWindow()
{
  if( FBForm != NULL )
    FBForm->Show();
}

void
TGUIView::HideWindow()
{
  if( FBForm != NULL )
    FBForm->Hide();
}

void
TGUIView::MaximizeWindow()
{
  if( FBForm != NULL )
  {
    FBForm->WindowState = wsMaximized;
    FBForm->Refresh();
  }
}

void
TGUIView::MinimizeWindow()
{
  if( FBForm != NULL )
    FBForm->WindowState = wsMinimized;
}

bool
TGUIView::WindowVisible()
{
  if( FBForm == NULL )
    return false;

  return FBForm->Visible;
}

bool
TGUIView::WindowMaximized()
{
  if( FBForm == NULL )
    return false;

  return ( FBForm->WindowState == wsMaximized );
}

TColor
TGUIView::GUIColorToOSColor( TGUIColor inColor )
{
  TColor  outColor;
  switch( inColor )
  {
    case white:
      outColor = clWhite;
      break;
    case black:
      outColor = clBlack;
      break;
    case red:
      outColor = clRed;
      break;
    case green:
      outColor = clGreen;
      break;
    case blue:
      outColor = clBlue;
      break;
    case yellow:
      outColor = clYellow;
      break;
    case magenta:
      outColor = clPurple;
      break;
    case cyan:
      outColor = clFuchsia;
      break;
    case darkblue:
      outColor = clNavy;
      break;
    default:
      outColor = clBlack;
      break;
  }
  return outColor;
}

TFontStyles
TGUIView::GUIFontStyleToOSFontStyle( TGUIFontStyle inFontStyle )
{
  TFontStyles outStyles;
  switch( inFontStyle )
  {
    case bold:
      outStyles << fsBold;
      break;
    case italic:
      outStyles << fsItalic;
      break;
    case bolditalic:
      outStyles << fsBold;
      outStyles << fsItalic;
      break;
    case plain:
    default:
      ; // nothing
  }
  return outStyles;
}

TAlignment
TGUIView::GUIAlignmentToOSAlignment( TGUIAlignment inAlignment )
{
  TAlignment  outAlignment = taLeftJustify;
  switch( inAlignment )
  {
    case left:
      outAlignment = taLeftJustify;
      break;
    case right:
      outAlignment = taRightJustify;
      break;
    case center:
      outAlignment = taCenter;
      break;
    default:
      ;
  }
  return outAlignment;
}

// The feedback form method definitions.
__fastcall
TGUIView::TFBForm::TFBForm()
: TForm( ( TComponent* )NULL, 1 )
#ifndef VCL_DOUBLEBUF
, offscreenBitmap( NULL ),
  updateRgn( NULL )
#endif // VCL_DOUBLEBUF
{
  if( HICON iconHandle = ::LoadIcon( HInstance, cFbFormTitle ) )
  {
    Icon->ReleaseHandle();
    Icon->Handle = iconHandle;
  }
  AutoScroll = false;
  OnClose = FormCloseHandler;
  OnPaint = FormPaintHandler;
  OnResize = FormResizeHandler;
  Caption = cFbFormTitle;
  DefaultMonitor = dmDesktop;
  Height = lastHeight;
  Width = lastWidth;
  if( lastLeft == invalidCoord || lastTop == invalidCoord )
  {
    Top = ( Screen->Height - Height ) / 2;
    Left = ( Screen->Width - Width ) / 2;
  }
  else
  {
    Top = lastTop;
    Left = lastLeft;
  }
#ifdef BCI2000
  BorderStyle = bsNone;
  WindowState = wsNormal;
#else
  BorderStyle = bsSizeable;
  WindowState = lastWindowState;
#endif // BCI2000

#ifdef VCL_DOUBLEBUF
  DoubleBuffered = true;
#else
  offscreenBitmap = new Graphics::TBitmap;
  updateRgn = ::CreateRectRgn( 0, 0, 0, 0 );
#endif // VCL_DOUBLEBUF
}

__fastcall
TGUIView::TFBForm::~TFBForm()
{
  lastWindowState = WindowState;
  if( lastWindowState != wsMaximized )
  {
    lastHeight = Height;
    lastWidth = Width;
    lastLeft = Left;
    lastTop = Top;
  }
#ifndef VCL_DOUBLEBUF
  delete offscreenBitmap;
  ::DeleteObject( updateRgn );
#endif // !VCL_DOUBLEBUF
}

#ifndef VCL_DOUBLEBUF
void
__fastcall
TGUIView::TFBForm::InvalidateRect( const TRect& inTRect )
{
  RECT invalidRect =
  {
    inTRect.Left,
    inTRect.Top,
    inTRect.Right,
    inTRect.Bottom
  };
  ::InvalidateRect( Handle, &invalidRect, false );
}
#endif // VCL_DOUBLEBUF

void
__fastcall
TGUIView::TFBForm::FormPaintHandler( TObject* )
{
#ifndef VCL_DOUBLEBUF
  offscreenBitmap->Width = ClientWidth;
  offscreenBitmap->Height = ClientHeight;
  ::SelectClipRgn( offscreenBitmap->Canvas->Handle, updateRgn );
#endif // !VCL_DOUBLEBUF

  for( TViewSet::iterator i = views.begin(); i != views.end(); ++i )
    ( *i )->Paint();

#ifndef VCL_DOUBLEBUF
#ifdef VBL_SYNC
  // Do nothing if the window spans across monitors.
  TMonitor* currentMonitor = TScreen::MonitorFromWindow( Handle, mdNull );
  if( currentMonitor )
  {
    HMONITOR monitorHandle = NULL;
    // In case of the
    if( !currentMonitor->Primary )
      monitorHandle = currentMonitor->Handle;
    IDirectDraw7* ddDevice = DDraw[ monitorHandle ];
    if( ddDevice )
      ddDevice->WaitForVerticalBlank( DDWAITVB_BLOCKBEGIN, NULL );
  }
#endif // VBL_SYNC
  Canvas->Draw( 0, 0, offscreenBitmap );
#endif // !VCL_DOUBLEBUF
}

void
__fastcall
TGUIView::TFBForm::FormResizeHandler( TObject* )
{
  for( TViewSet::iterator i = views.begin(); i != views.end(); ++i )
    ( *i )->Resized();
  Refresh();
}

void
__fastcall
TGUIView::TFBForm::FormCloseHandler( TObject*, TCloseAction &outAction )
{
  outAction = caHide;
}

