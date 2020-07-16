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
/*/
#ifndef TaskH
#define TaskH

#include <stdio.h>

#include "GenericVisualization.h"
#include "GenericFilter.h"
#include "UTarget.h"

#include "UserDisplay.h"
#include "UTargetSequence.h"
#include "UTrialSequence.h"


class TTask : public GenericFilter
{
private:
        GenericVisualization    *vis;
        TARGETLIST      *targets, oldactivetargets;
        USERDISPLAY     *userdisplay;
        TARGETSEQUENCE  *targetsequence;
        TRIALSEQUENCE   *trialsequence;
        int             Wx, Wy, Wxl, Wyl;
        void            HandleSelected(TARGET *selected);
        int             DetermineCorrectTargetID();
        /*shidong starts*/
        //bool            alternatebackup;
        ///BYTE            currentbackuppos;
        //BYTE            GetCurrentBackupPos();
        FILE            *a;
        bool            debug;
        int             NumberTargets;
        int            IgnoreMistakes;

        bool            CheckTree(int  root) const;
        bool            checkInt(AnsiString input) const;
        /*shidong ends*/

	//VK
	bool		CheckTargetMatrix() const;
        
        AnsiString      DetermineNewResultText(AnsiString resulttext, AnsiString predicted);
        AnsiString      DetermineCurrentPrefix(AnsiString resulttext);
        AnsiString      DetermineDesiredWord(AnsiString resulttext, AnsiString spelledtext);
public:
          TTask();
  virtual ~TTask();

  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void Process( const GenericSignal& Input, GenericSignal& Output );
};
#endif
