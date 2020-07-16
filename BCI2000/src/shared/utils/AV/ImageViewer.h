////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A component that displays still images.
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
#ifndef IMAGE_VIEWER_H
#define IMAGE_VIEWER_H

#include "GraphObject.h"
#include "StringUtils.h"

class ImageViewer : public GUI::GraphObject
{
public:
  ImageViewer( GUI::GraphDisplay& display, int zOrder = ShapeZOrder );
  virtual ~ImageViewer();

  operator bool() const { return IsOpen(); }
  ImageViewer& Open( const std::string& s ) { return SetFile( s ); }
  ImageViewer& Close() { return SetFile( "" ); }

  // Properties
  int NativeWidth() const;
  int NativeHeight() const;

  ImageViewer& SetFile( const std::string& );
  const String& File() const;
  bool IsOpen() const;
  const String& Error() const;
  String ContainerFormat() const;
  String Codec() const;

 protected:
  // GraphObject event handlers
  virtual void OnResize( GUI::DrawContext& );
  virtual void OnPaint( const GUI::DrawContext& );

 private:
  struct Data;
  Data* d;
};

#endif // IMAGE_VIEWER_H
