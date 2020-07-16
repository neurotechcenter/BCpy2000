////////////////////////////////////////////////////////////////////////////////
// $Id: ParamList.h 5599 2017-01-17 20:04:11Z mellinger $
// Authors: gschalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: ParamList is a BCI2000 type that represents a collection of
//   parameters.
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
#ifndef PARAM_LIST_H
#define PARAM_LIST_H

#include "ParamListView.h"

class ParamList : public ParamListView
{
public:
  ParamList();
  ParamList(const ParamList&);
  ParamList(const ParamListView&);
  ParamList& operator=(const ParamList&);
  ParamList& operator=(const ParamListView&);
  ~ParamList();

 protected:
  int onCount() const override;
  Param* onInsert(const Param&, float) override;
  bool onRemove(const std::string& name) override;
  Param* onGet(const std::string& name) override;
  const Param* onGet(const std::string& name) const override;
  Param* onGet(size_t) override;
  const Param* onGet(size_t) const override;
  void onClear() override;
  void onSort() override;
  void onSetUnchanged() override;

private:
  struct Private;
  Private* p;
};

#endif // PARAM_LIST_H

