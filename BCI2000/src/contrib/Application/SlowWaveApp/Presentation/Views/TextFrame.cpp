/////////////////////////////////////////////////////////////////////////////
//
// File: TextFrame.cpp
//
// Date: Oct 22, 2001
//
// Author: Juergen Mellinger
//
// Description: A frame to display text. This class is the GUI independent
//              interface used by the other presentation classes.
//              All GUI dependent functions are in the TGUITextFrame
//              class.
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

#include <list>
#include <string>

#include "TextFrame.h"
#include "PresErrors.h"
#include "Utils/Util.h"

using namespace std;

TTextFrame::TTextFrame()
{
}

TTextFrame::~TTextFrame()
{
}

TPresError
TTextFrame::SetRect( const Param    *inParamPtr )
{
    TPresError err = viewRect.ReadFromParam( inParamPtr );
    if( err == presNoError )
        TGUITextFrame::Resized();
    return presNoError;
}

TPresError
TTextFrame::SetTextProperties( const Param  *inParamPtr )
{
    TPresError  err = TTextProperties::SetTextProperties( inParamPtr );
    TGUITextFrame::Resized();
    return err;
}

void
TTextFrame::SetText( const TInputSequence &inText )
{
    TGUITextFrame::SetText( inText );
}

TInputSequence
TTextFrame::GetText() const
{
    return TGUITextFrame::GetText();
}

void
TTextFrame::AddText( const TInputSequence &inText )
{
    TInputSequence curText = GetText();
    // Interpret ASCII control characters.
    for( TInputSequence::const_iterator i = inText.begin(); i != inText.end(); ++i )
        switch( *i )
        {
            case '\a': // bell
                TGUITextFrame::Bell();
                break;
            case '\b': // backspace
                if( curText.length() > 0 )
                    curText = curText.substr( 0, curText.length() - 1 );
                break;
            default:
                curText += *i;
        }
    SetText( curText );
}

void
TTextFrame::LoadText( const char    *inFileName )
{
    Util::TPath         curPath;
    string              inputFileName = curPath + inFileName;
    if( TGUITextFrame::LoadText( inputFileName.c_str() ) != presNoError )
    {
#if 1
        gPresErrors.AddError( presFileOpeningError, inputFileName.c_str() );
#else
        SetText( "" );
#endif
    }
}

void
TTextFrame::SaveText( const char    *inFileName )
{
    // This is a work around for a strange problem with static
    // class variables being destructed before the application's main form
    // on program exit.
    // (SaveText() is called from ~PresSpellerModel().)
    Util::TPath         curPath( pathBuffer );
    string              outputFileName = curPath + inFileName;
    if( TGUITextFrame::SaveText( outputFileName.c_str() ) != presNoError )
        gPresErrors.AddError( presFileOpeningError, outputFileName.c_str() );
}

void
TTextFrame::ShowCursor()
{
    TGUITextFrame::ShowCursor();
}

void
TTextFrame::HideCursor()
{
    TGUITextFrame::HideCursor();
}


