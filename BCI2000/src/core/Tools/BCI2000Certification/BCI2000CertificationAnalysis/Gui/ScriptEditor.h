////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@neurotechcenter.org
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2018: BCI2000 team and many external contributors ]
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
#ifndef SCRIPT_EDITOR_H
#define SCRIPT_EDITOR_H

#include <QWidget>

class ScriptEditor : public QWidget
{
  Q_OBJECT
public:
  ScriptEditor(QWidget* = nullptr);
  ~ScriptEditor();
  void setFileNameUtf8(const std::string&);
  const std::string& fileNameUtf8() const;
  void setContentUtf8(const std::string&);
  std::string contentUtf8() const;
  void setRows(int);
  int rows() const;
  void setCols(int);
  int cols() const;
  void showMenuBar();
  void setEditable(bool);
  bool isEditable() const;
signals:
  void textChanged();
public slots:
  void open(const std::string& fileNameUtf8);
  void open() { open(fileNameUtf8()); }
  void saveAs();
  void close();
private:
  struct Private;
  Private* p;
};

#endif // SCRIPT_EDITOR_H
