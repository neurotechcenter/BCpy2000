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
#ifndef BASIC_STATISTICS_H
#define BASIC_STATISTICS_H

#include <vector>

class BasicStatistics
{
public:
  typedef double real;

  BasicStatistics();
  BasicStatistics& computeFrom(const std::vector<real>&);
  BasicStatistics& scale(double f);
  BasicStatistics& translate(double t);
  BasicStatistics& join(const BasicStatistics& s);

  real min() const { return quantile(0); }
  real max() const { return quantile(1); }
  real median() const { return quantile(0.5); }
  real quantile(real p) const;
  real count() const;
  real mean() const;
  real var() const;
  real mode() const { return computeMode().first; }
  real modeCount() const { return computeMode().second; }
  const std::vector<real>& sortedData() const { return mSortedData; }

private:
  std::pair<real, real> computeMode() const;

  real p0, p1, p2;
  std::vector<real> mSortedData;
};

#endif // BASIC_STATISTICS_H
