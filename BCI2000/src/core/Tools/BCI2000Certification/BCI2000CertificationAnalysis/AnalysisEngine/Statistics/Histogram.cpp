////////////////////////////////////////////////////////////////////////////////
// $Id: Functions.cpp 5664 2017-07-22 18:13:09Z mellinger $
// Author: juergen.mellinger@neurotechcenter.org
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2019: BCI2000 team and many external contributors ]
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
#include "Histogram.h"

Histogram
Histogram::fromQuantiles(const std::vector<real>& q, real count, real maxres)
{ // Histogram dP/dx is reciprocal derivative of quantile function x(P).

  Histogram h;
  int n = q.size() - 1;
  if(n < 1)
    return h;

  auto& data = h.mData;

  data.push_back(std::make_pair(q.front(), 0));
  int i1 = 0, i2 = 0;
  while(i2 < n)
  {
    real x1 = q[i1], x2 = q[++i2];
    while(i2 < n && x2 - x1 < maxres)
      x2 = q[++i2];
    if(i2 == n && x2 - x1 < maxres && data.size() > 1)
    {
      auto& p1 = data[data.size()-2], &p2 = data[data.size()-1];
      x1 = p1.first;
      p2.first = x2;
      p1.second += count*(i2 - i1)/(x2 - x1)/n;
      p2.second = p1.second;
    }
    else
    {
      real h = count*(i2 - i1)/(x2 - x1)/n;
      data.push_back(std::make_pair(x1, h));
      data.push_back(std::make_pair(x2, h));
    }
    i1 = i2;
  }
  data.push_back(std::make_pair(q.back(), 0));
  return h;
}

const Histogram::Data&
Histogram::data() const
{
  return mData;
}

