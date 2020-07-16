//////////////////////////////////////////////////////////////////////
// $Id: Preferences.h 3949 2012-04-23 15:33:15Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A class to hold user preferences.
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
///////////////////////////////////////////////////////////////////////
#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>
#include <QDir>

class Preferences
{
 public:
  Preferences();
  ~Preferences();
  void ReadFromCommandLine();
  void ReadSettings();
  void WriteSettings();

  enum
  {
    Beginner = 1,
    Intermediate,
    Advanced,
  };
  int mUserLevel;

  enum
  {
    AfterModulesConnected = 0,
    OnConnect = AfterModulesConnected,
    OnSetConfig,
    OnExit,
    OnResume,
    OnSuspend,
    OnStart,
    OnShutdown,
    numScriptEvents
  };
  QString mScript[ numScriptEvents ];
  bool    mCmdlineSpecified[ numScriptEvents ];

  enum
  {
    numButtons = 4,
  };
  struct
  {
    QString Name,
            Cmd;
  } mButtons[ numButtons ];

  QDir mCurrentDir;
};

#endif // PREFERENCES_H
