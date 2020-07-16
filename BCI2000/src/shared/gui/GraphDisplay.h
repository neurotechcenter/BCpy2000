////////////////////////////////////////////////////////////////////////////////
// $Id: GraphDisplay.h 5523 2016-09-09 19:56:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that represents a display rectangle for a set of
//   GraphObjects.
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
#ifndef GRAPH_DISPLAY_H
#define GRAPH_DISPLAY_H

#include "GraphObject.h"
#include "Uncopyable.h"
#include "Broadcaster.h"

#include "Bitmap.h"
#include "Color.h"
#include <set>
#include <list>

struct AudioSinkRef;

namespace GUI {

class Bitmap;

class GraphDisplay : public Broadcaster, private Uncopyable
{
 public:
  GraphDisplay();
  virtual ~GraphDisplay();

  // Properties
  GraphDisplay& SetContext( const DrawContext& );
  GUI::DrawContext Context() const;
  GraphDisplay& SetColor( RGBColor );
  RGBColor Color() const;
  GraphDisplay& SetAlignment( int );
  int Alignment() const;

  GraphDisplay& SetAssociatedAudio( AudioSinkRef* );
  AudioSinkRef* AssociatedAudio() const;

  GraphDisplay& ClearClicks();
  GraphObject* ConsumeClick();

  GraphDisplay& DeleteObjects();

  // Graphics functions
  //  Invalidate the display's entire area
  GraphDisplay& Invalidate();
  //  Invalidate a rectangle given in pixel coordinates
  GraphDisplay& InvalidateRect( const Rect& );

  // Get invalid region
  GUI::Region InvalidRegion() const;
  // Read bitmap data, resampled to target resolution
  Bitmap BitmapData( int width = 0, int height = 0, const GUI::Rect* = 0 ) const;

  // Coordinate conversion for object drawing
  Rect NormalizedToPixelCoords( const Rect& ) const;
  Rect PixelToNormalizedCoords( const Rect& ) const;

  // Events
  void Change();
  void Paint();

  bool Tick( const ClockTick& );
  bool Click( const GUI::Point& );

  // Broadcasting
  enum { OnTick = 1, OnClick, NextAvailableEvent };

  const ClockTick& Tick() const;
  const GUI::Point& Click() const;

 protected:
  virtual void OnBitmapData( GUI::Bitmap&, const GUI::Rect* ) const;

  // Management of GraphObjects
 private:
  // Add() and Remove() are only provided for GraphObject self registering
  // and unregistering in the GraphObject constructor and destructor.
  // Calling them from elsewhere will lead to inconsistency between
  // a GraphObject's display reference, and the display it is attached to.
  friend class GraphObject;
  GraphDisplay& Add( GraphObject* obj );
  GraphDisplay& Remove( GraphObject* );
  GraphDisplay& Presented( GraphObject* );

 private:
  struct Data;
  Data* d;
};

} // namespace GUI

#endif // GRAPH_DISPLAY_H
