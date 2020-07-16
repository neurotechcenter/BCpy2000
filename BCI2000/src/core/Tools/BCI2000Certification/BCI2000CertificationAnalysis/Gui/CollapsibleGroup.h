////////////////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.org
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
#ifndef COLLAPSIBLE_GROUP_H
#define COLLAPSIBLE_GROUP_H

#include <QWidget>
#include "QtSignalConnector.h"

class QToolButton;
class QCheckBox;
class QGridLayout;

class CollapsibleGroup : public QWidget, public QtSignalConnector
{
  QToolButton* mpToolButton;
  QCheckBox* mpCheckBox;
  QGridLayout* mpLayout;
  QWidget* mpHeader, *mpContent;
  struct ClickableLabel;
  ClickableLabel* mpLabel;

public:
  CollapsibleGroup(const QString& title);
  void setContentLayout(QLayout* pLayout);
  QLayout* contentLayout() const;
  bool isCollapsed() const;
  void setCollapsed(bool b = true);
  void setUsingCheckBox(bool b = true);
  bool isUsingCheckBox() const;
private:
  void onCollapsedToggled();
  using QWidget::layout;
  using QWidget::setLayout;
};

#endif // COLLAPSIBLE_GROUP_H
