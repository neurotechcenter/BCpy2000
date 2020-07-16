/////////////////////////////////////////////////////////////////////////////
//
// File: GUIArtifactView.cpp
//
// Date: Nov 7, 2001
//
// Author: Juergen Mellinger
//
// Description: The TGUIArtifactView class implements the GUI specific details
//              of the TArtifactView class.
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
# error This is the VCL implementation of TGUIArtifactView.
#endif

#include "GUIArtifactView.h"
#include "ViewsRes.h"

TGUIArtifactView::TGUIArtifactView()
: TGUIView( artifactViewZ ),
  indicatorVisible( false ),
  indicatorBuffer( NULL ),
  indicTRect( 0, 0, 0, 0 )
{
}

TGUIArtifactView::~TGUIArtifactView()
{
    delete indicatorBuffer;
}

void
TGUIArtifactView::Resized()
{
    TGUIView::Resized();
    if( indicatorBuffer != NULL )
    {
        indicTRect.Left = viewTRect.Left + ( viewTRect.Width() - indicatorBuffer->Width ) / 2;
        indicTRect.Top = viewTRect.Top + ( viewTRect.Height() - indicatorBuffer->Height ) / 2;
        indicTRect.Right = indicTRect.Left + indicatorBuffer->Width;
        indicTRect.Bottom = indicTRect.Top + indicatorBuffer->Height;
    }
}

void
TGUIArtifactView::Paint()
{
    if( indicatorVisible )
        GetCanvas()->Draw( indicTRect.Left, indicTRect.Top, indicatorBuffer );
}

TPresError
TGUIArtifactView::InitIndicator()
{
    TPresError  result = presNoError;

    delete indicatorBuffer;
    indicatorBuffer = new Graphics::TBitmap;
    try
    {
        indicatorBuffer->LoadFromResourceName( ( int )HInstance, cArtIndicatorName );
        indicatorBuffer->Transparent = true;
        indicatorBuffer->TransparentMode = tmAuto;

        Resized();
    }
    catch ( const EResNotFound& )
    {
        result = presResNotFoundError;
        gPresErrors.AddError( result, cArtIndicatorName );
    }

    return result;
}

void
TGUIArtifactView::ShowIndicator()
{
    indicatorVisible = true;
    TGUIView::InvalidateRect( indicTRect );
}

void
TGUIArtifactView::HideIndicator()
{
    indicatorVisible = false;
    TGUIView::InvalidateRect( indicTRect );
}


