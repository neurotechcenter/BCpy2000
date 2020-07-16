////////////////////////////////////////////////////////////////////////////////
//
// File: FBArteCorrection.h
//
// Description: Slow Wave Class Definition
//              written by Dr. Thilo Hinterberger 2000-2001
//              Copyright University of Tuebingen, Germany
//
// Changes:     2003, juergen.mellinger@uni-tuebingen.de: some bugs fixed.
//              Feb 8, 2004, jm: Adaptations to changes in BCI2000 framework,
//              minor reformulations, reformatting.
//              Feb 24, 2004, jm: Moved the TFBArteCorrection class into
//              separate files.
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
#ifndef FBArteCorrectionH
#define FBArteCorrectionH

#include "GenericFilter.h"
#include "GenericVisualization.h"
#include <vector>

class TFBArteCorrection : public GenericFilter
{
  public:
                 TFBArteCorrection();
    virtual      ~TFBArteCorrection() {}
    virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
    virtual void Initialize( const SignalProperties&, const SignalProperties& );
    virtual void Process( const GenericSignal&, GenericSignal& );

  private:
    enum ArteMode
    {
      off = 0,
      linearSubtraction,
      subtractionIfSupporting,
      subtractionWithAbort,
    }                    mArteMode;
    std::vector<int>     mArteChList;     // list of input channels assigned with the corresponding ArteCh
    std::vector<float>   mArteFactorList; // correction factor for channel n
};

#endif // FBArteCorrectionH
