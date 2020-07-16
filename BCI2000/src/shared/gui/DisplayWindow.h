////////////////////////////////////////////////////////////////////////////////
// $Id: DisplayWindow.h 5854 2019-01-07 16:08:00Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A GraphDisplay descendant which is a frameless GUI window for
//   an application's user display.
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
#ifndef DISPLAY_WINDOW_H
#define DISPLAY_WINDOW_H

#include "GraphDisplay.h"
#include "ClockTick.h"
#include "StringUtils.h"

namespace GUI {

class DisplayWindow : public GraphDisplay
{
 public:
  enum
  { OnUpdateReceived = GraphDisplay::NextAvailableEvent,
    OnUpdateDisplayed,
    NextAvailableEvent
  };

  DisplayWindow();
  virtual ~DisplayWindow();

  // Properties
  DisplayWindow& SetTitle( const std::string& );
  const std::string& Title() const;
  DisplayWindow& SetLeft( int );
  int Left() const;
  DisplayWindow& SetTop( int );
  int Top() const;
  DisplayWindow& SetWidth( int );
  int Width() const;
  DisplayWindow& SetHeight( int );
  int Height() const;
  DisplayWindow& SetVisible( bool );
  bool Visible() const;
  DisplayWindow& Show() { return SetVisible( true ); }
  DisplayWindow& Hide() { return SetVisible( false ); }

  // An argument to Update() will be available in RenderState::userData when OnUpdateReceived
  // or OnUpdateDisplayed is broadcast.
  DisplayWindow& Update( void* = 0 );

  DisplayWindow& SetAsyncRendering( bool );
  bool AsyncRendering() const;
  DisplayWindow& SetUpdateRateDecimation( int );
  int UpdateRateDecimation() const;

  struct RenderState
  {
    int64_t contentFrame, displayFrame;
    Time requested, displayed;
    bool presentation;
    void* userData;
    RenderState( int64_t f = 0, Time t = Time::Never, void* p = nullptr )
    : contentFrame( f ), displayFrame( -1 ),
      requested( t ), displayed( Time::Never ),
      presentation( false ), userData( p ) {}
  };

  ClockTick CurrentFrame() const;
  ClockTick::Statistics FrameClockStatistics() const;
  struct RenderStatistics { int decimation; struct { Time::Interval mean, std; } render; };
  struct RenderStatistics RenderStatistics() const;
  struct PresentationStatistics { int64_t count, late; struct { Time::Interval mean, std; } latency; };
  struct PresentationStatistics PresentationStatistics() const;
  StringUtils::NameValueList Info() const;

  DisplayWindow& ConfigureBitmapBuffer( int width, int height );

 protected:
  void OnBitmapData( GUI::Bitmap&, const GUI::Rect* ) const override;

 private:
  using GraphDisplay::SetContext;

  struct Data;
  Data* d;
};

} // namespace GUI

#endif // DISPLAY_WINDOW_H
