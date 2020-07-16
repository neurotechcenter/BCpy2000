////////////////////////////////////////////////////////////////////////////////
// $Id: TrialStatistics.h 3798 2012-02-01 18:07:06Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that encapsulates trial statistics for an application.
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
#ifndef TRIAL_STATISTICS_H
#define TRIAL_STATISTICS_H

#include "Environment.h"
#include <vector>

class TrialStatistics: public EnvironmentExtension
{
  public:
    void   Publish() {}
    void   Preflight() const;
    void   Initialize();
    void   Process() {};

    void   Reset();
    void   Update( int inTargetCode, int inResultCode );
    void   UpdateInvalid();

    int    Hits()    const;
    int    Total()   const;
    int    Invalid() const;
    double Bits()    const;

  private:
    int                            mInvalidTrials;
    std::vector<std::vector<int> > mTargetsResultsMatrix;
};

#endif // TRIAL_STATISTICS_H
