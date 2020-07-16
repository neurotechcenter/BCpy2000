////////////////////////////////////////////////////////////////////////////////
// $Id: VisDisplay.h 5649 2017-07-14 17:34:26Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: An interface class for visualization displays.
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
#ifndef VIS_DISPLAY_H
#define VIS_DISPLAY_H

#include "CfgID.h"
#include "Synchronized.h"
#include <string>
#include <QObject>

class GenericSignal;
class BitmapImage;
class QWidget;
class QMenu;

class VisDisplay
{
public:
  static void Initialize(QWidget* parent, QMenu* visMenu = nullptr);
  static void Deinitialize();
  static void ClearVisuals();

  static void Create( const std::string& visID, const std::string& kind );
  static void HandleSignal( const std::string& visID, int channels, const std::vector<float>& );
  static void HandleMemo( const std::string& visID, const std::string& );
  static void HandleBitmap( const std::string& visID, const BitmapImage& );
  static void HandlePropertyMessage( const std::string& visID, CfgID, const std::string& );
  static void HandleProperty( const std::string& visID, CfgID, const std::string& );
};

// Proxy object for transmitting data across thread boundaries,
// necessary because Qt restricts both GUI object creation, and rendering, to the main thread.
// The proxy object will be created in the main thread, and its slots may be called asynchronously
// from other threads.
class VisDisplayProxy : public QObject
{
  Q_OBJECT
private:
  VisDisplayProxy() {}
  ~VisDisplayProxy() {}

private slots:
  void Create( const std::string& visID, const std::string& kind );
  void HandleSignal( const std::string& visID, int channels, const std::vector<float>& );
  void HandleMemo( const std::string& visID, const std::string& );
  void HandleBitmap( const std::string& visID, const BitmapImage& );
  void HandleProperty( const std::string& visID, int, const std::string&, int );
  void Clear();

private:
  bool MayPostData();
  bool MayHandleData();
  Synchronized<int> mQueueLength;

  friend class VisDisplay;
};

#endif // VIS_DISPLAY_H
