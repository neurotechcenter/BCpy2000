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
#include "BasicStatistics.h"
#include "Histogram.h"
#include <limits>

namespace {
  const BasicStatistics::real nan() { return std::numeric_limits<BasicStatistics::real>::quiet_NaN(); }
  const BasicStatistics::real eps() { return std::numeric_limits<BasicStatistics::real>::epsilon(); }
}

BasicStatistics::BasicStatistics()
: p0(0), p1(0), p2(0)
{
}

BasicStatistics&
BasicStatistics::computeFrom(const std::vector<real>& data)
{
  mSortedData = data;
  std::sort(mSortedData.begin(), mSortedData.end());

  p0 = mSortedData.size();
  p1 = 0;
  p2 = 0;
  for(auto v : mSortedData)
  { // ascending order minimizes numeric error
    p1 += v;
    p2 += v*v;
  }
  return *this;
}

BasicStatistics&
BasicStatistics::scale(double f)
{
  p1 *= f;
  p2 *= f*f;
  for(auto& q : mSortedData)
    q *= f;
  return *this;
}

BasicStatistics&
BasicStatistics::translate(double t)
{
  p2 += p0*t*t + 2*p1*t;
  p1 += p0*t;
  for(auto& q : mSortedData)
    q += t;
  return *this;
}

BasicStatistics&
BasicStatistics::join(const BasicStatistics& s)
{
  if(s.mSortedData.empty())
    return *this;
  if(mSortedData.empty())
    return *this = s;

  std::vector<real> data;
  data.reserve(mSortedData.size() + s.mSortedData.size());
  size_t i = 0, j = 0;
  while(i < mSortedData.size() && j < s.mSortedData.size())
  {
    if(mSortedData[i] < s.mSortedData[j])
      data.push_back(mSortedData[i++]);
    else
      data.push_back(s.mSortedData[j++]);
  }
  while(i < mSortedData.size())
    data.push_back(mSortedData[i++]);
  while(j < s.mSortedData.size())
    data.push_back(s.mSortedData[j++]);
  mSortedData = data;
  p0 += s.p0;
  p1 += s.p1;
  p2 += s.p2;
  return *this;
}

double 
BasicStatistics::quantile(double p) const
{
  if(mSortedData.size() < 2)
    return nan();
  p = std::max(0.0, p);
  p = std::min(1.0, p);
  return mSortedData[(mSortedData.size() - 1)*p];
}

double 
BasicStatistics::count() const
{
  return p0;
}

double 
BasicStatistics::mean() const
{
  return p1/p0;
}

double 
BasicStatistics::var() const
{
  return p2/p0 - p1*p1/p0/p0;
}

std::pair<double, double>
BasicStatistics::computeMode() const
{
  double curValue = nan();
  int curCount = 0;
  double modeValue = nan();
  int modeCount = 0;
  for (int i = 0; i < mSortedData.size(); ++i)
  {
    if (mSortedData[i] != curValue)
    {
      if (curCount > modeCount)
      {
        modeCount = curCount;
        modeValue = curValue;
      }
      curCount = 1;
      curValue = mSortedData[i];
    }
    else
    {
      ++curCount;
    }
  }
  if (curCount > modeCount)
  {
    modeCount = curCount;
    modeValue = curValue;
  }
  return std::make_pair(modeValue, modeCount);
}
