////////////////////////////////////////////////////////////////////////////////
// $Id: VisDisplay.cpp 5649 2017-07-14 17:34:26Z mellinger $
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
#include "VisDisplay.h"

#include "VisID.h"
#include "VisDisplayBase.h"
#include "VisDisplayWindow.h"
#include "VisDisplayMemo.h"
#include "VisDisplayGraph.h"
#include "VisDisplayBitmap.h"
#include "MetaTypes.h"
#include "ThreadUtils.h"

using namespace std;

static VisDisplayProxy* spProxy = nullptr;

void
VisDisplay::Initialize(QWidget* inpWidget, QMenu* inpVisMenu)
{
  qRegisterMetaType<std::string>();
  qRegisterMetaType<std::vector<float>>();
  qRegisterMetaType<BitmapImage>();

  Assert( ThreadUtils::InMainThread() );
  Assert( spProxy == nullptr );
  spProxy = new VisDisplayProxy;
  VisDisplayBase::Initialize();
  VisDisplayWindow::SetParentWindow(inpWidget);
  VisDisplayWindow::SetWindowMenu(inpVisMenu);
}

void
VisDisplay::Deinitialize()
{
  Assert( ThreadUtils::InMainThread() );
  VisDisplayBase::Clear();
  delete spProxy;
  spProxy = nullptr;
}

void
VisDisplay::Create( const std::string& inVisID, const std::string& inKind )
{
  Assert( spProxy );
  QMetaObject::invokeMethod(
    spProxy,
    "Create",
    Qt::AutoConnection,
    Q_ARG( std::string, inVisID ),
    Q_ARG( std::string, inKind )
  );
}

void
VisDisplay::ClearVisuals()
{
  Assert( spProxy );
  QMetaObject::invokeMethod(
    spProxy,
    "Clear",
    Qt::AutoConnection
  );
}

void
VisDisplay::HandleSignal( const std::string& inVisID, int inChannels, const std::vector<float>& inSignal )
{
  Assert( spProxy );
  if( spProxy->MayPostData() )
    QMetaObject::invokeMethod(
      spProxy,
      "HandleSignal",
      Qt::AutoConnection,
      Q_ARG( std::string, inVisID ),
      Q_ARG( int, inChannels ),
      Q_ARG( std::vector<float>, inSignal )
    );
}

void
VisDisplay::HandleMemo( const std::string& inVisID, const std::string& inText )
{
  Assert( spProxy );
  if( spProxy->MayPostData() )
    QMetaObject::invokeMethod(
      spProxy,
      "HandleMemo",
      Qt::AutoConnection,
      Q_ARG( std::string, inVisID ),
      Q_ARG( std::string, inText )
    );
}

void
VisDisplay::HandleBitmap( const std::string& inVisID, const BitmapImage& inBitmap )
{
  Assert( spProxy );
  if( spProxy->MayPostData() )
    QMetaObject::invokeMethod(
      spProxy,
      "HandleBitmap",
      Qt::AutoConnection,
      Q_ARG( std::string, inVisID ),
      Q_ARG( BitmapImage, inBitmap )
    );
}

void
VisDisplay::HandleProperty( const std::string& inVisID, CfgID inCfgID, const std::string& inValue )
{
  Assert( spProxy );
  QMetaObject::invokeMethod(
    spProxy,
    "HandleProperty",
    Qt::AutoConnection,
    Q_ARG( std::string, inVisID ),
    Q_ARG( int, inCfgID ),
    Q_ARG( std::string, inValue ),
    Q_ARG( int, VisDisplayBase::UserDefined )
  );
}

void
VisDisplay::HandlePropertyMessage( const std::string& inVisID, CfgID inCfgID, const std::string& inValue )
{
  Assert( spProxy );
  QMetaObject::invokeMethod(
    spProxy,
    "HandleProperty",
    Qt::AutoConnection,
    Q_ARG( std::string, inVisID ),
    Q_ARG( int, inCfgID ),
    Q_ARG( std::string, inValue ),
    Q_ARG( int, VisDisplayBase::MessageDefined )
  );
}

// VisDisplayProxy
void
VisDisplayProxy::Create( const std::string& inVisID, const std::string& inKind )
{
  VisID visID = VisID( inVisID ).ToLayer();
  if( inKind == "Memo" )
    VisDisplayBase::Create<VisDisplayMemo>( visID );
  else if( inKind == "Graph" )
    VisDisplayBase::Create<VisDisplayGraph>( visID );
  else if( inKind == "Bitmap" )
    VisDisplayBase::Create<VisDisplayBitmap>( visID );
}

void
VisDisplayProxy::HandleSignal( const std::string& inVisID, int ch, const std::vector<float>& inSignal )
{
  if( MayHandleData() )
    VisDisplayBase::HandleSignal( VisID( inVisID ).ToLayer(), ch, inSignal );
}

void
VisDisplayProxy::HandleMemo( const std::string& inVisID, const std::string& inText )
{
  if( MayHandleData() )
    VisDisplayBase::HandleMemo( VisID( inVisID ).ToLayer(), inText );
}

void
VisDisplayProxy::HandleBitmap( const std::string& inVisID, const BitmapImage& inBitmap )
{
  if( MayHandleData() )
    VisDisplayBase::HandleBitmap( VisID( inVisID ).ToLayer(), inBitmap );
}

void
VisDisplayProxy::HandleProperty( const std::string& inVisID, int inCfgID, const std::string& inValue, int inConfigState )
{
  VisID visID( inVisID );
  if( visID.LayerID().empty() )
    VisDisplayBase::HandleProperty( visID.WindowID(), inCfgID, inValue, inConfigState );
  VisDisplayBase::HandleProperty( visID.ToLayer(), inCfgID, inValue, inConfigState );
}

void
VisDisplayProxy::Clear()
{
  VisDisplayBase::Clear();
}

bool
VisDisplayProxy::MayPostData()
{
  return ++mQueueLength < 10000;
}

bool
VisDisplayProxy::MayHandleData()
{
  return --mQueueLength < 1000;
}

