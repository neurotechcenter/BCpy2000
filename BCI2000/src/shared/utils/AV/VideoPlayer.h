////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A GraphObject for video rendering.
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
#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include "GraphObject.h"
#include "AVPlayer.h"

class VideoPlayer : public AVPlayer, public GUI::GraphObject
{
 public:
  VideoPlayer( GUI::GraphDisplay& display, int zOrder = ShapeZOrder );
  virtual ~VideoPlayer();

  int NativeWidth() const;
  int NativeHeight() const;

  double FramesPerSecond() const;
  double NativeFramesPerSecond() const;

 protected:
  // AVPlayer event handlers
  const char* OnSetFile( const std::string& );
  const char* OnAdvance( int );
  const char* OnRenderingTick( const ClockTick&, bool );
  // GraphObject event handlers
  virtual void OnResize( GUI::DrawContext& );
  virtual void OnTick( const ClockTick& );
  virtual void OnPaint( const GUI::DrawContext& );

 private:
  struct Data;
  Data* d;
};

#endif // VIDEO_PLAYER_H
