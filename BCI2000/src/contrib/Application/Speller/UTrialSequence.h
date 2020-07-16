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
//---------------------------------------------------------------------------

#ifndef UTrialSequenceH
#define UTrialSequenceH
//---------------------------------------------------------------------------
#include <vector>

#include "GenericVisualization.h"
#include "UserDisplay.h"
#include "Environment.h"

#define SEQ_ITI                 1
#define SEQ_PTP                 2
#define SEQ_FEEDBACK            3
#define SEQ_OUTCOME             4
#define SEQ_CONGRATULATIONS     5

class TRIALSEQUENCE : private Environment
{
private: 	// User declarations
        int     ititime, max_ititime;                   // in units of SampleBlocks
        int     pretrialtime, max_pretrialtime;         // in units of SampleBlocks
        int     feedbacktime, max_feedbacktime;         // in units of SampleBlocks
        int     outcometime, max_outcometime;           // in units of SampleBlocks
        int     congrattime, max_congrattime;           // in units of SampleBlocks
        int     cur_sequence;                           // current sequence
        USERDISPLAY     *userdisplay;
        GenericVisualization    *vis;
        unsigned short oldrunning;
        TARGET  *selectedtarget;
        bool    highlightcorrecttarget;
        void    SuspendTrial();
public:		// User declarations
        TRIALSEQUENCE::TRIALSEQUENCE();
        TRIALSEQUENCE::~TRIALSEQUENCE();
        int     Initialize(USERDISPLAY *, int);
        int     correcttargetID;                        // targetID of the next correct target
        void    ITI();
        void    PTP();
        void    Feedback(const std::vector<float>& controlsignal);
        TARGET  *Outcome();
        void    Congratulations();
        TARGET  *Process(const std::vector<float>&);
        void    ResetTrialSequence();
        void    Switch2Congratulations();
        /*shidong starts*/
        int     NUM_TARGETS;
        /*shidong ends*/
};
#endif
