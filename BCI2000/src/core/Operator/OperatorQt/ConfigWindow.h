//////////////////////////////////////////////////////////////////////
// $Id: ConfigWindow.h 4607 2013-10-14 13:18:08Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: The Operator module's parameter configuration dialog.
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
#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QDialog>
#include <set>
#include <map>
#include <string>
#include "ParamDisplay.h"

class Param;
class ParamList;
class Preferences;
class QScrollArea;

namespace Ui
{
  class ConfigWindow;
}

class ConfigWindow : public QDialog
{
  Q_OBJECT

 public:
  ConfigWindow(QWidget *parent = 0);
  ~ConfigWindow();
  int  Initialize( ParamList*, Preferences* );
  void RenderParameter( Param* );

 private:
  void DeleteAllTabs();
  void RenderParameters( const std::string& section );
  void UpdateParameters();

 private slots:
  void OnCfgTabControlChange();
  void OnClose();
  void OnSaveParametersClick();
  void OnLoadParametersClick();
  void OnConfigureSaveFilterClick();
  void OnConfigureLoadFilterClick();
  void OnHelpClick();

 protected:
  void changeEvent(QEvent *e);

 private:
  bool LoadParameters( const QString& inFileName );
  void DisposeWidgets();

 private:
  Ui::ConfigWindow* m_ui;

 private:
  ParamList*   mpParameters;
  Preferences* mpPreferences;

  typedef std::set<QWidget*> WidgetContainer;
  WidgetContainer  mWidgets;
  typedef std::map<std::string, ParamDisplay> DisplayContainer;
  DisplayContainer mParamDisplays;
  std::string      mCurTab;
  bool             mUserSwitchedTabs;
  QString          mOriginalTitle;
  QScrollArea*     mpScrollArea;
  int              mScrollPos;
};

extern ConfigWindow* gpConfig;

#endif // CONFIGWINDOW_H
