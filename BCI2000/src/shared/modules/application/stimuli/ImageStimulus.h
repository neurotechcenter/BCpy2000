////////////////////////////////////////////////////////////////////////////////
// $Id: ImageStimulus.h 4765 2014-11-25 17:43:14Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A stimulus consisting of an image.
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
#ifndef IMAGE_STIMULUS_H
#define IMAGE_STIMULUS_H

#include "VisualStimulus.h"
#include "GraphObject.h"

class ImageStimulus : public VisualStimulus, public GUI::GraphObject
{
 public:
  ImageStimulus( GUI::GraphDisplay& display );
  virtual ~ImageStimulus();
  // Properties
  ImageStimulus& SetFile( const std::string& );
  const std::string& File() const;
  ImageStimulus& SetRenderingMode( int );
  int RenderingMode() const;

  int NativeWidth() const;
  int NativeHeight() const;
  int OriginalWidth() const { return NativeWidth(); }
  int OriginalHeight() const { return NativeHeight(); }

 protected:
  // GraphObject event handlers
  virtual void OnPaint( const GUI::DrawContext& );
  virtual void OnChange( GUI::DrawContext& );
  virtual void OnMove( GUI::DrawContext& );
  virtual void OnResize( GUI::DrawContext& );

 private:
  void AdjustRect( GUI::Rect& ) const;

 private:
  std::string mFile;
  int mRenderingMode;
  struct ImageStimulusPrivateData* mpData;
};

#endif // IMAGE_STIMULUS_H

