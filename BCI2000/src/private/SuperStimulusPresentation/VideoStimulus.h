////////////////////////////////////////////////////////////////////////////////
// $Id: VideoStimulus.h 4597 2013-12-03 14:04:06Z mellinger $
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
#ifndef VIDEO_STIMULUS_H
#define VIDEO_STIMULUS_H

#include "BCIStream.h"
#include <string>

#include "GraphObject.h"
#include "GraphDisplay.h"
#include "VisualStimulus.h"
#include "FileUtils.h"
#include "BCIError.h"
#include "BCIStream.h"
#include "NumericConstants.h"

#include <QObject>
#include <QFile>
#include <QMediaContent>
#include <QMediaPlayer>
#include <QVideoWidget>

#include <QPixmap>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsVideoItem>
#include <QStyleOptionGraphicsItem>

#define VIDEO_STIMULUS_PRIVATE_DISPLAY 1
#define VIDEO_STIMULUS_USE_VIDEO_WIDGET 1
#define VIDEO_STIMULUS_SAVE_RENDERS 0
#define VIDEO_STIMULUS_USE_GRAPHICS_DISPLAY 0

using namespace std;
using namespace GUI;

class VideoStimulus : public QObject, public VisualStimulus, public GUI::GraphObject
{
	Q_OBJECT

 public:
  VideoStimulus( GUI::GraphDisplay& display );
  virtual ~VideoStimulus();
	
	// Property sets (basic)
	VideoStimulus& SetFile      ( const string& name   );
	VideoStimulus& SetVolume    ( const double& volume ); // Given as [0,1] - converted to [0,100]
	VideoStimulus& SetRate      ( const qreal& rate    );
  VideoStimulus& SetSize      (                      );
	VideoStimulus& SetSize      ( const GUI::Rect rect );
	VideoStimulus& SetSize      ( const int& width,
													      const int& height    );
  VideoStimulus& SetColor     ( const RGBColor color );
  VideoStimulus& SetStretching( bool enabled         );

	// Property gets
  string File    () { return mFileName;                     }
  double Volume  () { return mpMediaPlayer->volume();       }
  qreal  Rate    () { return mpMediaPlayer->playbackRate(); }
  bool   Stretch () { return doStretch;                     }

 protected:
  virtual void OnPresent();
  virtual void OnConceal();
  virtual void OnPaint( const GUI::DrawContext& );
  
 public:
  // Error checking
  // Returns true on success and false on failure
  // Verbose indicates it should throw a corresponding bcierr of problems
	bool CheckMediaPlayer            ( bool verbose );
	bool CheckMediaPlayerAvailability( bool verbose );
	bool CheckMediaPlayerError       ( bool verbose );
	bool CheckMediaPlayerStatus      ( bool verbose );

 private:
	// File information
	string mFileName,
         mError;
  int count;

	// Video information
	GUI::Rect mRect;
	QRect mQRect;
	QSize mSize;
	float mWidth, mHeight;
  bool mPresenting,
       doStretch;

	// Qt media containers
	QMediaPlayer* mpMediaPlayer;
  QVideoWidget* mpVideoWidget;

  // Qt graphics
  QGraphicsScene*     mpGraphicsScene;
  QGraphicsView*      mpGraphicsView;
  QGraphicsVideoItem* mpGraphicsVideoItem;

};

#endif // VIDEO_STIMULUS_H