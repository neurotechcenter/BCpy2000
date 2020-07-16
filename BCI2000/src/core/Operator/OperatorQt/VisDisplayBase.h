////////////////////////////////////////////////////////////////////////////////
// $Id: VisDisplayBase.h 5409 2016-06-24 15:44:09Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class for visualization displays.
//   Also handles message dispatching and storage of visualization properties.
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
#ifndef VIS_DISPLAY_BASE_H
#define VIS_DISPLAY_BASE_H

#include "VisDisplay.h"

#include "VisID.h"
#include "Lockable.h"
#include "Synchronized.h"
#include <QWidget>
#include <map>
#include <string>
#include <sstream>

class GenericSignal;
class BitmapImage;

class VisDisplayBase : public QWidget
{
  Q_OBJECT

 public:
  typedef enum // Possible states of properties ("configs").
  {
    Default = 0,     // May be overridden by a message or by user settings.
    OnceUserDefined, // A previous user setting, read from the registry.
    UserDefined,     // Set by user.
    MessageDefined,  // Set by a message, overrides user.
  } ConfigState;

  template<class T> static void Create( const VisID& );

 protected:
  VisDisplayBase( const VisID& inVisID );
 public:
  virtual ~VisDisplayBase();

  static void Initialize();
  static void Clear();

  static void HandleSignal( const VisID&, int ch, const std::vector<float>& );
  static void HandleMemo( const VisID&, const std::string& );
  static void HandleBitmap( const VisID&, const BitmapImage& );
  static void HandleProperty( const VisID&, CfgID, const std::string& value, int configState );

 protected:
  struct ConfigSettings;
  virtual void OnSetConfig( ConfigSettings& );
  virtual void OnContextMenu( const QPoint& ) {}
  virtual void OnSignal( const GenericSignal& ) {}
  virtual void OnMemo( const QString& ) {}
  virtual void OnBitmap( const BitmapImage& ) {}
  virtual void OnProperty( CfgID, const char*, ConfigState ) {}

 private:
  void HandleSignal( int, const std::vector<float>& );
  void HandleMemo( const QString& );
  void HandleBitmap( const BitmapImage& );
  void HandleProperty( CfgID, const char*, ConfigState );

 private slots:
  void ContextMenu( const QPoint& );
  void SetConfig( ConfigSettings& );

 protected:
  VisID mVisID;

  // visID->display instance
  struct VisContainer : std::map<VisID, VisDisplayBase*>
  {
    void Clear();
  };
  static VisContainer& Visuals();

 protected:
  // configID->value
  typedef std::map< CfgID, std::string > ConfigSettingsBase;
  class ConfigSettings : public ConfigSettingsBase, public Lockable<>
  {
   public:
    bool Get( CfgID id, std::string&, ConfigState minState = Default );
    bool Put( CfgID id, const std::string&, ConfigState state );
    template<typename T> bool Get( CfgID id, T&, ConfigState minState = Default );
    template<typename T> bool Put( CfgID id, const T&, ConfigState state );
    ConfigState& State( CfgID id ) { return mStates[ id ]; }

   private:
    std::map< CfgID, ConfigState > mStates;
  };

  // visID->config information
  class ConfigContainer : public std::map<std::string, ConfigSettings>
  {
   public:
    void Save();
    void Restore();
  };
  static ConfigContainer& Visconfigs();

private:
  static VisContainer sVisuals;
  static ConfigContainer sVisconfigs;
};

template<class T>
void VisDisplayBase::Create( const VisID& inVisID )
{
  VisDisplayBase* pExisting = VisDisplayBase::Visuals()[inVisID];
  delete pExisting;
  VisDisplayBase* pObject = new T( inVisID );
  VisDisplayBase::Visuals()[inVisID] = pObject;
  pObject->SetConfig( Visconfigs()[inVisID] );
}

template<typename T>
bool
VisDisplayBase::ConfigSettings::Get( CfgID id, T& t, ConfigState minState )
{
  std::string s;
  if( !Get( id, s, minState ) )
    return false;
  if( s.empty() )
    return true;
  std::istringstream is( s );
  T value;
  if( is >> value )
    t = value;
  return !is.fail();
}

template<typename T>
bool
VisDisplayBase::ConfigSettings::Put( CfgID id, const T& t, ConfigState state )
{
  std::ostringstream os;
  os << t;
  return !os.fail() && Put( id, os.str(), state );
}

#endif // VIS_DISPLAY_BASE_H
