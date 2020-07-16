//////////////////////////////////////////////////////////////////////
// $Id: ShowParameters.h 4607 2013-10-14 13:18:08Z mellinger $
// Authors: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A dialog displaying parameters with check boxes.
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
#ifndef SHOWPARAMETERS_H
#define SHOWPARAMETERS_H

#include <QDialog>

class Param;
class ParamList;
class QTreeWidgetItem;

namespace Ui
{
  class ShowParameters;
}

class ShowParameters : public QDialog
{
  Q_OBJECT
 public:
  ShowParameters( QWidget* parent, const ParamList&, int filterType );
  ~ShowParameters();

 protected:
  void changeEvent( QEvent* e);

 private slots:
  void OnShow();
  void OnAccepted();
  void OnRejected();
  void OnExpandCollapse();
  void OnItemChanged();

 private:
  static void AdjustSize( QTreeWidgetItem*, int margin );

  Ui::ShowParameters* m_ui;
  const ParamList&    mrParamList;
  int                 mFilterType;
  bool                mChanged;
};

#endif // SHOWPARAMETERS_H
