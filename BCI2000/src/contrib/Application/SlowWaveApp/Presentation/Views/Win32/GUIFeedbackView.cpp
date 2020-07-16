/////////////////////////////////////////////////////////////////////////////
//
// File: GUIFeedbackView.cpp
//
// Date: Nov 7, 2001
//
// Author: Juergen Mellinger
//
// Description: The TGUIFeedbackView class implements the GUI specific details
//              of the TFeedbackView class.
//
// Changes:
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
# error This is the VCL implementation of TGUIFeedbackView.
#endif

#include <math.h>

#include "GUIFeedbackView.h"
#include "Utils/Util.h"
#include "ViewsRes.h"


TGUIFeedbackView::TGUIFeedbackView()
: TGUIView( feedbackViewZ ),
  cursorX( 0.0 ),
  cursorY( 0.0 ),
  cursorBrightness( 1.0 ),
  cursorVisible( false ),
#ifdef PERUVIAN_BRIGHTNESS_HACK
  brightnessCursor( false ),
#endif // PERUVIAN_BRIGHTNESS_HACK
  cursorBitmap( NULL ),
  normalBitmap( NULL ),
  invertedBitmap( NULL ),
  cursorTRect( 0, 0, 0, 0 )
{
}

TGUIFeedbackView::~TGUIFeedbackView()
{
    delete normalBitmap;
    delete invertedBitmap;
}

void
TGUIFeedbackView::Paint()
{
    TCanvas *canvas = GetCanvas();

#ifdef PERUVIAN_BRIGHTNESS_HACK
    if( brightnessCursor )
    {
        float       brightness;
        if( cursorVisible )
            brightness = cursorBrightness;
        else
            brightness = 0.5;
        TRGBColor   fillColor;
        fillColor.cl = clWhite;
        fillColor.elem.RGB.r *= brightness;
        fillColor.elem.RGB.g *= brightness;
        fillColor.elem.RGB.b *= brightness;
        canvas->Brush->Color = fillColor.cl;
        canvas->FillRect( viewTRect );
    }
#endif // PERUVIAN_BRIGHTNESS_HACK

    if( cursorBitmap == NULL )
        return;

    // Draw the cursor.
    if( cursorVisible )
    {
        if( cursorBrightness != 1.0 )
        {
            Graphics::TBitmap   *modulatedBitmap = new Graphics::TBitmap;
            int width = cursorBitmap->Width,
                height = cursorBitmap->Height;
            TRGBColor   originalTransColor,
                        newTransColor;
            originalTransColor.cl = clMin;
            newTransColor.cl = clMin;
            newTransColor.elem.mode = nonexistingColorMode;
            if( cursorBitmap->Transparent )
                originalTransColor.cl = cursorBitmap->TransparentColor;
            modulatedBitmap->TransparentMode = tmFixed;
            modulatedBitmap->TransparentColor = newTransColor.cl;
            modulatedBitmap->Transparent = true;
            modulatedBitmap->Height = height;
            modulatedBitmap->Width = width;
            Graphics::TCanvas   *modulatedCanvas = modulatedBitmap->Canvas,
                                *originalCanvas = cursorBitmap->Canvas;

            for( int i = 0; i < width; ++i )
                for( int j = 0; j < height; ++j )
                {
                    TRGBColor  pixelColor;
                    pixelColor.cl = originalCanvas->Pixels[ i ][ j ];
                    if( pixelColor.elem.rgb == originalTransColor.elem.rgb )
                    {
                        pixelColor.cl = newTransColor.cl;
                    }
                    else
                    {
                        pixelColor.elem.RGB.r *= cursorBrightness;
                        pixelColor.elem.RGB.g *= cursorBrightness;
                        pixelColor.elem.RGB.b *= cursorBrightness;
                    }
                    modulatedCanvas->Pixels[ i ][ j ] = pixelColor.cl;
                }

            canvas->Draw( cursorTRect.Left, cursorTRect.Top, modulatedBitmap );
            delete modulatedBitmap;
        }
        else
            canvas->Draw( cursorTRect.Left, cursorTRect.Top, cursorBitmap );
    }
}

void
TGUIFeedbackView::Resized()
{
    TGUIView::Resized();
    SetCursorCoordinates( cursorX, cursorY, cursorBrightness );
}

TPresError
TGUIFeedbackView::InitCursor( const char* inCursorFile )
{
#ifdef PERUVIAN_BRIGHTNESS_HACK
    if( inCursorFile == PERUVIAN_CURSOR )
    {
        brightnessCursor = true;
        return presNoError;
    }
#endif // PERUVIAN_BRIGHTNESS_HACK

    delete normalBitmap;
    normalBitmap = NULL;
    delete invertedBitmap;
    invertedBitmap = NULL;
    cursorBitmap = NULL;

    TPresError  result = presNoError;

    Graphics::TBitmap   *bitmap = new Graphics::TBitmap();
    if( inCursorFile != NULL )
    {
        Util::TPath cursorPath( inCursorFile );
        try
        {
            bitmap->LoadFromFile( cursorPath.c_str() );
        }
        catch( ... )
        {
          try
          {
            bitmap->LoadFromResourceName( ( int )HInstance, inCursorFile );
          }
          catch( const EResNotFound& )
          {
            result = presResNotFoundError;
          }
        }
        gPresErrors.AddError( result, cursorPath.c_str() );
    }

    if( inCursorFile == NULL || result != presNoError )
    {
        result = presNoError;

        try
        {
            bitmap->LoadFromResourceName( ( int )HInstance, cFbCursorName );
        }
        catch ( const EResNotFound& )
        {
            result = presResNotFoundError;
            gPresErrors.AddError( result, cFbCursorName );
        }
    }

    if( result == presNoError )
    {
        try
        {
            // The cursor file contains two cursors: A "normal" one and
            // an "inverted" one for indicating an invalid trial.
            // For any of the two, the color of its bottom left pixel is taken as
            // the transparent color.
            TRect   destRect( 0, 0, bitmap->Width / 2 - 1, bitmap->Height ),
                    srcRect( destRect );
            bitmap->Transparent = false;

            normalBitmap = new Graphics::TBitmap();
            normalBitmap->Height = bitmap->Height;
            normalBitmap->Width = bitmap->Width / 2 - 1;
            normalBitmap->Canvas->CopyRect( destRect, bitmap->Canvas, srcRect );
            normalBitmap->Transparent = true;
            normalBitmap->TransparentMode = tmAuto;

            srcRect.Left = bitmap->Width / 2 - 1;
            srcRect.Right = bitmap->Width;
            invertedBitmap = new Graphics::TBitmap();
            invertedBitmap->Height = bitmap->Height;
            invertedBitmap->Width = bitmap->Width / 2 - 1;
            invertedBitmap->Canvas->CopyRect( destRect, bitmap->Canvas, srcRect );
            invertedBitmap->Transparent = true;
            invertedBitmap->TransparentMode = tmAuto;

            cursorBitmap = normalBitmap;
        }
        catch ( ... )
        {
            result = presGenError;
        }
    }
    delete  bitmap;

    if( result == presNoError )
        Resized();
    return result;
}

void
TGUIFeedbackView::SetCursorCoordinates( float inX, float inY, float inBrightness )
{
#ifdef PERUVIAN_BRIGHTNESS_HACK
    if( brightnessCursor )
      TGUIView::InvalidateRect( viewTRect );
#endif // PERUVIAN_BRIGHTNESS_HACK
    TGUIView::InvalidateRect( cursorTRect );
    cursorX = inX;
    cursorY = inY;
    cursorBrightness = inBrightness;
    if( cursorBitmap )
    {
        float   cursorWidth = ( float )cursorBitmap->Width,
                cursorHeight = ( float )cursorBitmap->Height,
                viewWidth = ( float )viewTRect.Width(),
                viewHeight = ( float )viewTRect.Height();

        int     posX = floor( viewWidth * cursorX - cursorWidth / 2.0 ),
                posY = floor( viewHeight * cursorY - cursorHeight / 2.0 );

        cursorTRect.Left = viewTRect.Left + posX;
        cursorTRect.Top = viewTRect.Top + posY;
        cursorTRect.Right = cursorTRect.Left + cursorBitmap->Width;
        cursorTRect.Bottom = cursorTRect.Top + cursorBitmap->Height;
        TGUIView::InvalidateRect( cursorTRect );
    }
    else
        cursorTRect = TRect( 0, 0, 0, 0 );
}

void
TGUIFeedbackView::ShowCursor()
{
    cursorVisible = true;
    TGUIView::InvalidateRect( cursorTRect );
}

void
TGUIFeedbackView::HideCursor()
{
    cursorVisible = false;
    TGUIView::InvalidateRect( cursorTRect );
}

void
TGUIFeedbackView::NormalCursor()
{
    cursorBitmap = normalBitmap;
    TGUIView::InvalidateRect( cursorTRect );
}

void
TGUIFeedbackView::InvertCursor()
{
    cursorBitmap = invertedBitmap;
    TGUIView::InvalidateRect( cursorTRect );
}

