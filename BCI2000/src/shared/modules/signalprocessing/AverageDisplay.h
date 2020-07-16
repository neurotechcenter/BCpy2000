////////////////////////////////////////////////////////////////////////////////
// $Id: AverageDisplay.h 3798 2012-02-01 18:07:06Z mellinger $
// Description: A BCI2000 filter that computes epoch averages of its input
//   signal, and displays these in a visualization window.
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
#ifndef AVERAGE_DISPLAY_H
#define AVERAGE_DISPLAY_H

#include <vector>
#include "GenericVisualization.h"
#include "GenericFilter.h"
#include "Color.h"

#define SET_BASELINE

class AverageDisplay : public GenericFilter
{
  enum
  {
    maxPower = 1
  };

 public:
  AverageDisplay();
  void Preflight( const SignalProperties&, SignalProperties& ) const;
  void Initialize( const SignalProperties&, const SignalProperties& );
  void Process( const GenericSignal&, GenericSignal& );
  bool AllowsVisualization() const { return false; }

 private:
  std::vector<GenericVisualization> mVisualizations;
  std::vector<int> mChannelIndices;
  std::vector<int> mTargetCodes;

  // Indices are [ channel ][ sample ].
  std::vector<std::vector<double> > mSignalOfCurrentRun;

  // Indices are [ power ][ channel ][ targetCode ][ sample ].
  std::vector<std::vector<std::vector<std::vector<double> > > > mPowerSums;

  int mLastTargetCode;

#ifdef SET_BASELINE
  // Baseline stuff that should really be factored out.
  std::vector<double> mBaselines, mBaselineSamples;
#endif // SET_BASELINE
  // A list of channel colors to use for the overlayed display.
  static const RGBColor sChannelColors[];
};

#endif // AVERAGE_DISPLAY_H
