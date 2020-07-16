////////////////////////////////////////////////////////////////////////////////
// $Id: VideoStimulus.cpp 4597 2013-12-03 14:04:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de, josh.goldberg.819@gmail.com
// Description: A stimulus consisting of a video.
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
#ifndef VIDEO_STIMULUS_CPP
#define VIDEO_STIMULUS_CPP

#include "PCHIncludes.h"
#pragma hdrstop

#include "VideoStimulus.h"

#include "FileUtils.h"
#include "BCIError.h"
#include "BCIStream.h"
#include "NumericConstants.h"
#include "GraphObject.h"
#include "GraphDisplay.h"

#define VIDEO_USE_PLAYLIST false
#define VIDEO_USE_TEMPFILE true

VideoStimulus::VideoStimulus( GraphDisplay& display )
: GraphObject( display, ImageStimulusZOrder ) // Put a VideoStimulusZOrder in GraphObject.h?
{
  
}

// I'm not doing any memory management now, as pointer usage is still in flux
VideoStimulus::~VideoStimulus()
{

}

VideoStimulus&
VideoStimulus::SetFile( const string& fileName )
{
  
	return *this;
}

VideoStimulus&
VideoStimulus::SetVolume( const qreal& volume )
{
  
	return *this;
}

VideoStimulus&
VideoStimulus::SetRate( const qreal& rate)
{
  
	return *this;
}

VideoStimulus&
VideoStimulus::SetSize( const int& width, const int& height )
{
  
	return *this;
}
VideoStimulus&
VideoStimulus::SetSize( const GUI::Rect rect)
{
	return SetSize(rect.right - rect.left, rect.bottom - rect.top);
}


void
VideoStimulus::OnPresent()
{
  
}

void
VideoStimulus::OnConceal()
{
  
}

void
VideoStimulus::OnPaint( const GUI::DrawContext& inDC )
{
  
}

bool
VideoStimulus::CheckMediaPlaylist( bool verbose, const string& fileName )
{
  return false;
}

bool
VideoStimulus::CheckMediaPlayer( bool verbose = false )
{
  return false;
}

bool
VideoStimulus::CheckMediaPlayerAvailability( bool verbose = false )
{
	return false;
}

bool
VideoStimulus::CheckMediaPlayerError( bool verbose = false )
{
	return false;
}

bool
VideoStimulus::CheckMediaPlayerStatus( bool verbose = false )
{
	return false;
}

bool
VideoStimulus::CheckMediaPlayerBufferStatus( bool verbose = false )
{
	return false;
}

#endif // VIDEO_STIMULUS_CPP