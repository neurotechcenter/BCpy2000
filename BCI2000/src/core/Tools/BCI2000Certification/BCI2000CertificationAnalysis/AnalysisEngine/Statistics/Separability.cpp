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
#include "Separability.h"
#include <cstdint>

template<typename U>
SeparabilityResult<typename U::value_type>
separabilityAnalysis(U begin, U end)
{
  typedef U::value_type real;
  static const real inf = std::numeric_limits<real>::infinity(),
                    nan = std::numeric_limits<real>::quiet_NaN();
  real min = inf, max = -inf;
  size_t count = 0;
  real p1 = 0, p2 = 0;
  for(auto i = begin; i != end; ++i)
  {
    auto val = *i;
    if(val == val) // not NaN
    {
      min = std::min(min, val);
      max = std::max(max, val);
      ++count;
      p1 += val;
      p2 += val*val;
    }
  }
  SeparabilityResult<real> r;
  r.mean1 = p1/count;
  r.mean2 = r.mean1;
  r.threshold = r.mean1;
  r.accuracy = 0;
  r.rsq = 0;
  r.noise = p2/count - p1*p1/count/count;
  if(max == min)
    return r;

  // 10% error in the threshold will be acceptable,
  // combined with a margin of 10 times to account
  // for the effect of outliers on the total range
  // gives a histogram size of 100 bins.
  static const size_t histbins = 100;
  std::vector<int> hist(histbins, 0);
  for(auto i = begin; i != end; ++i)
  {
    auto val = *i;
    if(val == val)
    {
      int bin = ::floor((val - min)/(max - min)*(hist.size()-1));
      ++hist[bin];
    }
  }
  r.accuracy = (max - min)/(hist.size() - 1);

  std::vector<std::pair<size_t, int>> peaks;
  std::pair<size_t, int> peak = std::make_pair(0, 0);
  for(size_t i = 0; i < hist.size(); ++i)
  {
    if(hist[i] >= peak.second)
    {
      peak.first = i;
      peak.second = hist[i];
    }
    else if(hist[i] <= peak.second/2)
    {
      peaks.push_back(peak);
      peak.first = i;
      peak.second = hist[i];
    }
  }
  peaks.push_back(peak);
  // Select that pair of peaks for which the product of distance
  // and heights is maximal.
  size_t max_vol = 0, max_i = 0, max_j = 0;
  for(size_t i = 0; i < peaks.size(); ++i)
  {
    for(size_t j = i + 1; j < peaks.size(); ++j)
    {
      int64_t vol = (peaks[j].first - peaks[i].first)*peaks[i].second*peaks[j].second;
      if(vol > max_vol)
      {
        max_i = i;
        max_j = j;
        max_vol = vol;
      }
    }
  }
  if(max_i == max_j)
    return r;
  // Select center of peaks for threshold.
  r.threshold = min + (peaks[max_i].first + peaks[max_j].first)*0.5*(max - min)/(hist.size()-1);
  // For an estimate of the amount of information contained the signal, we proceed as follows:
  // * infer labels for individual samples by comparing to the threshold,
  // * compute coefficient of determination (r-squared) between inferred labels, and signal.
  // See http://www.bci2000.org/wiki/Glossary#Computation for details.
  real n1 = 0, n2 = 0, s1 = 0, s2 = 0, q1 = 0, q2 = 0;
  for(auto i = begin; i != end; ++i)
  {
    if(*i == *i) // not NaN
    {
      auto val = *i;
      if(val < r.threshold)
      {
        n1 += 1;
        s1 += val;
        q1 += val*val;
      }
      else
      {
        n2 += 1;
        s2 += val;
        q2 += val*val;
      }
    }
  }
  r.mean1 = s1/n1;
  r.mean2 = s2/n2;
  real G = (s1+s2)*(s1+s2)/(n1+n2);
  r.rsq = (s1*s1/n1 + s2*s2/n2 - G)/(q1 + q2 - G);
  r.noise *= (1- r.rsq);
  return r;
}

template SeparabilityResult<double> separabilityAnalysis(std::vector<double>::iterator, std::vector<double>::iterator);
template SeparabilityResult<double> separabilityAnalysis(std::vector<double>::const_iterator, std::vector<double>::const_iterator);
