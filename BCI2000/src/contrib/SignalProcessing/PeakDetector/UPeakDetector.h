/* $BEGIN_BCI2000_LICENSE$
 * 
 * This file is part of BCI2000, a platform for real-time bio-signal research.
 * [ Copyright (C) 2000-2012: BCI2000 team and many external contributors ]
 * 
 * BCI2000 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * BCI2000 is distributed in the hope that it will be useful, but
 *                         WITHOUT ANY WARRANTY
 * - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * $END_BCI2000_LICENSE$
 */
//---------------------------------------------------------------------------

#ifndef UPeakDetectorH
#define UPeakDetectorH
//---------------------------------------------------------------------------

#include "GenericFilter.h"
#include "GenericVisualization.h"

class PeakDetector : public GenericFilter
{
private:
       int samples;               // dimension of data matrix
       int channels;              // dimension of data matrix
       int hz;
       int targetchpos;
       int targetchneg;
       float    posthresh, negthresh;
       bool visualize;
       GenericVisualization *vis;
       int get_num_pos_peaks(const GenericSignal *input, int channel);
       int get_num_neg_peaks(const GenericSignal *input, int channel);
       int nBins;

public:
       PeakDetector();
  virtual ~PeakDetector();
  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void Process(const GenericSignal& Input, GenericSignal& Output);
};
#endif

