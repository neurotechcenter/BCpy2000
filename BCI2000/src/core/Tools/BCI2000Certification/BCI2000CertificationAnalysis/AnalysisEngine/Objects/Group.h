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
#ifndef ANALYSIS_DATA_OBJECTS_GROUP_H
#define ANALYSIS_DATA_OBJECTS_GROUP_H

#include "AnalysisDataObject.h"

namespace AnalysisDataObjects {

class Group : public AnalysisDataObject
{
public:
  static AnalysisDataObject* create(Context*, const ExpressionParser::NodeList&);
  Group(Context*, const std::string& description, const UniqueList& = UniqueList());
  double add(double);
protected:
  ExpressionParser::Node* onMemberFunction(Context*, const std::string&, const ExpressionParser::NodeList&) override;
  int onEvalFlags() const override { return none; }
  bool onCanPlot(bool b) const override { return b; }
  bool onPlot() const override;
  std::string onDescription() const override;
  void addPlots(Dataplot&, std::vector<std::string>& plotdefs, std::string& xlabel, std::string& ylabel) const;
  bool onCanExport(const std::string&) const override { return false; }
private:
  std::string mDescription;
};

}

#endif // ANALYSIS_DATA_OBJECTS_GROUP_H

