/////////////////////////////////////////////////////////////////////////////
//
// File: GUIStimulusView.cpp
//
// Date: Nov 7, 2001
//
// Author: Juergen Mellinger
//
// Description: The TGUIStimulusView class implements the GUI specific details
//              of the TStimulusView class.
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
# error This is the VCL implementation of TGUIStimulusView.
#endif

#include <cassert>
#include <mmsystem.h>
#include <mplayer.hpp>

#include "GUIStimulusView.h"

TGUIStimulusView::TGUIStimulusView()
: TGUIView( stimulusViewZ ),
  visible( false ),
  bitmap( NULL ),
  mediaPlayer( NULL ),
  useMediaPlayer( false ),
  bitmapTRect( 0, 0, 0, 0 ),
  timerID( NULL )
{
    mediaPlayer = new TMediaPlayer( ( TComponent* )NULL );
    mediaPlayer->Visible = false;
    mediaPlayer->AutoOpen = false;
    mediaPlayer->AutoRewind = true;
    mediaPlayer->Parent = TGUIView::GetForm();
    mediaPlayer->Display = new TWinControl( mediaPlayer );
    mediaPlayer->Display->Visible = false;
    mediaPlayer->Display->Parent = TGUIView::GetForm();
    mediaPlayer->OnNotify = MediaPlayerNotifyHandler;
}

TGUIStimulusView::~TGUIStimulusView()
{
    if( timerID != NULL )
        ::timeKillEvent( timerID );
    delete bitmap;
    delete mediaPlayer;
}

void
TGUIStimulusView::Paint()
{
    if( bitmap == NULL )
        return;

    // Draw the bitmap.
    if( visible )
        GetCanvas()->Draw( bitmapTRect.Left, bitmapTRect.Top, bitmap );
}

void
TGUIStimulusView::Resized()
{
    TGUIView::Resized();
    int     viewWidth = viewTRect.Width(),
            viewHeight = viewTRect.Height();

    if( bitmap != NULL )
    {
        int     bitmapWidth = bitmap->Width,
                bitmapHeight = bitmap->Height,
                posX = viewTRect.Left + ( viewWidth - bitmapWidth ) / 2,
                posY = viewTRect.Top + ( viewHeight - bitmapHeight ) / 2;

        bitmapTRect.Left = posX;
        bitmapTRect.Top = posY;
        bitmapTRect.Right = posX + bitmap->Width;
        bitmapTRect.Bottom = posY + bitmap->Height;
    }
    else
        bitmapTRect = TRect( 0, 0, 0, 0 );

    if( useMediaPlayer )
    {
        int     displayWidth = mediaPlayer->DisplayRect.Width(),
                displayHeight = mediaPlayer->DisplayRect.Height(),
                posX = viewTRect.Left + ( viewWidth - displayWidth ) / 2,
                posY = viewTRect.Top + ( viewHeight - displayHeight ) / 2;

        mediaPlayer->Display->BoundsRect = TRect( posX, posY,
                                            posX + displayWidth,
                                            posY + displayHeight );
    }
}

void
TGUIStimulusView::AttachVisualStimulus( const char* inFile )
{
    TGUIView::InvalidateRect( viewTRect );
    delete bitmap;
    bitmap = NULL;
    mediaPlayer->Close();
    useMediaPlayer = false;

    if( inFile == NULL || *inFile == '\0' )
        return;

    TPresError  err = presNoError;
    bitmap = new Graphics::TBitmap();
    try
    {
        bitmap->LoadFromFile( inFile );
        bitmap->Transparent = false;
        Resized();
    }
    catch( const EFOpenError& )
    {
        err = presFileOpeningError;
    }
    catch( const EInvalidGraphic& )
    {
        err = presFileOpeningError;
    }
    
    if( err != presNoError )
    {
        delete bitmap;
        bitmap = NULL;
        err = presNoError;
        try
        {
            mediaPlayer->FileName = inFile;
            mediaPlayer->Open();
            useMediaPlayer = true;
            Resized();
        }
        catch( const EMCIDeviceError& )
        {
            err = presFileOpeningError;
        }
    }

    if( err != presNoError )
        gPresErrors.AddError( err, inFile );
}

void
TGUIStimulusView::PresentVisualStimulus( int inDuration )
{
    if( useMediaPlayer )
    {
        mediaPlayer->Display->Visible = true;
        mediaPlayer->Play();
        return;
    }

    if( inDuration <= 0 )
        return;

    if( timerID != NULL )
        ::timeKillEvent( timerID );

    timerID = ::timeSetEvent( inDuration, stimulusTimeResolution,
                                HideVisualStimulus, ( DWORD )this, TIME_ONESHOT );
    if( timerID != NULL )
    {
      visible = true;
      TGUIView::InvalidateRect( viewTRect );
    }

}

void
CALLBACK
TGUIStimulusView::HideVisualStimulus( UINT inTimerID, UINT, DWORD inInstance, DWORD, DWORD )
{
    TGUIStimulusView    *caller = ( TGUIStimulusView* )inInstance;
    assert( inTimerID == caller->timerID );
    caller->visible = false;
    caller->timerID = NULL;
    TGUIView::InvalidateRect( caller->viewTRect );
}

void
__fastcall
TGUIStimulusView::MediaPlayerNotifyHandler( TObject* )
{
    if( mediaPlayer->Mode != mpPlaying )
    {
        mediaPlayer->Display->Visible = false;
        TGUIView::InvalidateRect( viewTRect );
    }
}

