/////////////////////////////////////////////////////////////////////////////
//
// File: GUIView.h
//
// Date: Nov 7, 2001
//
// Author: Juergen Mellinger
//
// Description: The TGUIView class contains GUI specific
//              functionality common to all views.
//              The TViewZ enumeration constants define the order
//              in which views are drawn. The first view in the list
//              is on top of all others.
//
// Changes: Mar 17, 2003, jm:
//          Introduced separate handling of update region for fast updates
//          without flicker.
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

#ifndef GUI_VIEW_H
#define GUI_VIEW_H

#include "OSIncludes.h"
#include "SWGUI.h"

#include <set>

// The TViewZ enumeration constants define the order
// in which views are drawn. The first view in the list
// is on top of all others.
typedef enum TViewZ
{
  textFrameViewZ,
  stimulusViewZ,
  reinforcementViewZ,
  artifactViewZ,
  scoreViewZ,
  feedbackViewZ,
  markerViewZ,
  targetViewZ,
  nullTargetViewZ,
  backgroundViewZ
} TViewZ;

class TGUIView
{
  friend class TGUITextProperties;
  protected:
    // Only derived classes may instantiate this class.
                    TGUIView( TViewZ inZPosition );
                    ~TGUIView();

  public:
    // Use these functions to update graphics output.
    static  void    RefreshWindow();

    // Use these functions for window manipulation.
    static void     ResizeWindow( int inWidth, int inHeight );
    static void     MoveWindow( int inLeft, int inTop );
    static void     ShowWindow();
    static void     HideWindow();
    static void     MaximizeWindow();
    static void     MinimizeWindow();
    static bool     WindowVisible();
    static bool     WindowMaximized();

    // This function is called whenever actual graphics output is needed.
    virtual void    Paint() = 0;
    // This function is called when the output window is resized.
    virtual void    Resized();

            bool    isBefore( const TGUIView    &inView ) const;
            bool    isBehind( const TGUIView    &inView ) const;

    // A comparator class for use with stl algorithms.
    // With this class as comparator,
    // iterating through a set will follow the views' z positions.
    class   CmpViews
    {
      public:
            bool    operator()( const TGUIView  *inView1,
                                const TGUIView  *inView2 ) const;
    };

  protected:
    // This typedef is just a shortcut.
    typedef std::set< TGUIView*, TGUIView::CmpViews >   TViewSet;

    // This determines the order in which views get drawn.
    // A view with a lower zPosition is in front of one with a
    // higher one.
            TViewZ      zPosition;
            TGUIRect    viewRect;

  private:
    static  int     numInstances;

  public:
    // Functions and data for managing visual details from a resource or a file.
    //
    // Load the colors and widths of view elements from a bitmap resource or file.
    // The rows of the bitmap represent the elements in the order given by
    // the TGUIElements enumeration in GUI.h. An element's width is taken
    // by counting the number of pixels in its row that have the same color
    // as the leftmost one.
    static  TPresError          SetStyle( const char *inStyle );

  protected:
    union TRGBColor;
    static  const int&       GetElementWidth( TGUIElement inElement );
    static  int              ElementWidths[];
    static  const TRGBColor& GetElementColor( TGUIElement inElement );
    static  TRGBColor        ElementColors[];

    // OS/library specific members go here.
#ifdef VCL
            TRect       viewTRect;

    // Mark a rectangle for redrawing.
    static  void        InvalidateRect( const TRect& inTRect );

    // The feedback/stimulus display form.
    // There is no .dfm resource file for this form.
    class TFBForm : public TForm
    {
      public:
             __fastcall TFBForm();
             __fastcall ~TFBForm();
        void __fastcall FormPaintHandler( TObject* );
        void __fastcall FormResizeHandler( TObject* );
        void __fastcall FormCloseHandler( TObject*, TCloseAction& );

#ifndef VCL_DOUBLEBUF
        BEGIN_MESSAGE_MAP
            VCL_MESSAGE_HANDLER( WM_ERASEBKGND, TWMEraseBkgnd, WMEraseBkgnd )
            VCL_MESSAGE_HANDLER( WM_PAINT, TWMPaint, WMPaint )
        END_MESSAGE_MAP( TForm )

        // To avoid flicker, use a WM_ERASEBKGND handler that does not do anything.
        void __fastcall WMEraseBkgnd( TWMEraseBkgnd& ) {}
        // Obtain the window's update region before BeginPaint() in the VCL
        // paint handler destroys (validates) it.
        void __fastcall WMPaint( TWMPaint& Message )
        {
          ::GetUpdateRgn( Handle, updateRgn, false );
          PaintHandler( Message );
        }

        // Mark a rectangle for redrawing.
        void __fastcall InvalidateRect( const TRect& );
        // The invalidated region of the feedback window.
        HRGN            updateRgn;

        // An offscreen bitmap object.
        Graphics::TBitmap* offscreenBitmap;
#endif // !VCL_DOUBLEBUF

        TCanvas*        GetOffscreenCanvas();
        void            AttachView( TGUIView *inView );
        void            DetachView( TGUIView *inView );

      private:
      // The set of attached views.
        TViewSet            views;
      // Current settings to be saved when the form is deleted.
        static int          lastHeight,
                            lastWidth,
                            lastLeft,
                            lastTop;
        static TWindowState lastWindowState;
    };
    friend class TGUIView::TFBForm;

private:
    static  TFBForm*        FBForm;

protected:
    static const unsigned char nonexistingColorMode = 0x10;
    typedef union TRGBColor
    {
      TColor                cl;
      struct
      {
        union
        {
          struct
          {
            unsigned char   b : 8;
            unsigned char   g : 8;
            unsigned char   r : 8;
          } RGB;
          unsigned int      rgb : 24;
        };
        unsigned char       mode : 8;
      } elem;
    } TRGBColor;

    static  TCanvas*    GetCanvas();
    static  TFBForm*    GetForm();
    static  TAlignment  GUIAlignmentToOSAlignment( TGUIAlignment inAlignment );
    static  TColor      GUIColorToOSColor( TGUIColor inColor );
    static  TFontStyles GUIFontStyleToOSFontStyle( TGUIFontStyle inFontStyle );
#else // VCL
    static const unsigned char nonexistingColorMode = 0x10;
    typedef union TRGBColor
    {
      struct
      {
        union
        {
          struct
          {
            unsigned char   b : 8;
            unsigned char   g : 8;
            unsigned char   r : 8;
          } RGB;
          unsigned int      rgb : 24;
        };
        unsigned char       mode : 8;
      } elem;
    } TRGBColor;
#endif // VCL
};

inline
bool
TGUIView::isBefore( const TGUIView  &inView ) const
{
    return ( zPosition < inView.zPosition );
}

inline
bool
TGUIView::isBehind( const TGUIView  &inView ) const
{
    return ( zPosition > inView.zPosition );
}

inline
bool
TGUIView::CmpViews::operator()( const TGUIView  *inView1,
                                const TGUIView  *inView2 ) const
{
    // std::set assumes equality if operator() returns false for both
    // permutations of its arguments. This may never happen if inView1
    // and inView2 point to different objects.
    if( inView1->isBehind( *inView2 ) )
        // Here we are sure the objects are different.
        return true;
    else if( inView1->isBefore( *inView2 ) )
        // Here too.
        return false;
    else
        // We don't know if the objects are different - they
        // have equal z positions. Therefore, compare the pointers.
        // (The < operator cannot be overloaded for pointers.)
        return ( inView1 < inView2 );
}

inline
const int&
TGUIView::GetElementWidth( TGUIElement inElement )
{
    return ElementWidths[ inElement ];
}

inline
const TGUIView::TRGBColor&
TGUIView::GetElementColor( TGUIElement inElement )
{
    return ElementColors[ inElement ];
}

#ifdef VCL
inline
TCanvas*
TGUIView::GetCanvas()
{
    return FBForm->GetOffscreenCanvas();
}

inline
TGUIView::TFBForm*
TGUIView::GetForm()
{
    return FBForm;
}

inline
TCanvas*
TGUIView::TFBForm::GetOffscreenCanvas()
{
#ifdef VCL_DOUBLEBUF
    return Canvas;
#else
    return offscreenBitmap->Canvas;
#endif // VCL_DOUBLEBUF
}

inline
void
TGUIView::TFBForm::AttachView( TGUIView *inView )
{
  views.insert( inView );
}

inline
void
TGUIView::TFBForm::DetachView( TGUIView *inView )
{
  views.erase( inView );
}
#endif // VCL

#endif GUI_VIEW_H
