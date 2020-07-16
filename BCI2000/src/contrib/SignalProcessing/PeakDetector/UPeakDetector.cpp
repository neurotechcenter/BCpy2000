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

#include "PCHIncludes.h"
#pragma hdrstop

#include <math.h>
#include <stdio.h>

#include "UPeakDetector.h"

//---------------------------------------------------------------------------

RegisterFilter( PeakDetector, 2.C );

// **************************************************************************
// Function:   PeakDetector
// Purpose:    This is the constructor for the PeakDetector class
//             it requests parameters by adding parameters to the parameter list
//             it also requests states by adding them to the state list
// Parameters: plist - pointer to a list of parameters
//             slist - pointer to a list of states
// Returns:    N/A
// **************************************************************************
PeakDetector::PeakDetector()
: vis( NULL )
{
 BEGIN_PARAMETER_DEFINITIONS
  "PeakDetector float PosPeakThreshold= 0 0 0 2 "
    "// Threshold for positive peaks",
  "PeakDetector float NegPeakThreshold= 0 0 0 2 "
    "// Threshold for negative peaks",
  "PeakDetector int HistoryLength= 10 0 0 10 "
    "// Length of history of peak counts",
  "PeakDetector int TargetChannelPos= 0 0 0 10 "
    "// Target channel for peak detection of positive peaks",
  "PeakDetector int TargetChannelNeg= 0 0 0 10 "
    "// Target channel for peak detection of negative peaks",
  "Visualize int VisualizePeakDetector= 1 0 0 1 "
    "// visualize peak detection results (0=no 1=yes) (boolean)",
 END_PARAMETER_DEFINITIONS
}


// **************************************************************************
// Function:   ~PeakDetector
// Purpose:    This is the destructor for the PeakDetector class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
PeakDetector::~PeakDetector()
{
  delete vis;
}

// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistency with
//             input signal properties; requests minimally needed properties for
//             the output signal; checks whether resources are available.
// Parameters: Input and output signal properties pointers.
// Returns:    N/A
// **************************************************************************
void PeakDetector::Preflight( const SignalProperties& /*inSignalProperties*/,
                                    SignalProperties& outSignalProperties ) const
{
  // Parameter consistency checks: Existence/Ranges and mutual Ranges.

  // Resource availability checks.
  /* The P3 temporal filter seems not to depend on external resources. */

  // Requested output signal properties.
  outSignalProperties = SignalProperties( 2, Parameter( "HistoryLength" ) );
}

// **************************************************************************
// Function:   Initialize
// Purpose:    This function parameterizes the PeakDetector
// Parameters: paramlist - list of the (fully configured) parameter list
//             new_statevector - pointer to the statevector (which also has a pointer to the list of states)
//             new_corecomm - pointer to the communication object to the operator
// Returns:    0 ... on error
//             1 ... no error
// **************************************************************************
void PeakDetector::Initialize( const SignalProperties& Input, const SignalProperties& /*Output*/ )
{
  samples=     Input.Elements();
  visualize=   ( int )Parameter("VisualizePeakDetector");
  hz=          static_cast<int>( Input.SamplingRate() );
  posthresh=   Parameter("PosPeakThreshold");
  negthresh=   Parameter("NegPeakThreshold");
  nBins=       Parameter("HistoryLength");
  targetchpos= Parameter("TargetChannelPos");
  targetchneg= Parameter("TargetChannelNeg");

 if ( visualize )
    {
    delete vis;
    vis= new GenericVisualization( "TMPF" );
    vis->Send(CfgID::WindowTitle, "Peak Detector");
    vis->Send(CfgID::NumSamples, nBins);
    vis->Send(CfgID::MinValue, "0");
    vis->Send(CfgID::MaxValue, "100");
    }
}


// **************************************************************************
// Function:   Process
// Purpose:    This function applies the peak detector
// Parameters: input  - input signal for the
//             output - output signal for this filter
// Returns:    0 ... on error
//             1 ... no error
// **************************************************************************
void PeakDetector::Process(const GenericSignal& input, GenericSignal& output)
{
int   ch, sample;
int   num_pos_peaks, num_neg_peaks;
static bool first=true;

 // the first time, set output signal to 0
 if (first)
    {
    first=false;
    for (ch=0; ch < 2; ch++)
     for (sample=0; sample < output.Elements(); sample++)
      output(ch, sample) = 0;
    }

 // shift the number of peaks to the left to make room for the new number of peaks
 for (ch=0; ch < 2; ch++)
  for (sample=1; sample < output.Elements(); sample++)
   output(ch, sample-1) = output(ch, sample);

 // calculate the new number of peaks and assign them to the right-most (i.e., most recent) position
 num_pos_peaks=get_num_pos_peaks(&input, targetchpos);
 num_neg_peaks=get_num_neg_peaks(&input, targetchneg);
 // channel 0 ... number of positive peaks
 output(0, output.Elements()-1) = num_pos_peaks;
 // channel 1 ... number of negative peaks
 output(1, output.Elements()-1) = num_neg_peaks;

 // FILE *fp=fopen("c:\\data.txt", "ab");
 // fprintf(fp, "%d\r\n", num_pos_peaks);
 // fclose(fp);

  if( visualize )
    {
    vis->Send(output);
    }

 return;
}


// detect number of positive peaks
int PeakDetector::get_num_pos_peaks(const GenericSignal *input, int channel)
{
int     cur_idx, peak_ptr;
float   current_val, next_val;
bool    peak_flag;
int     num_peaks;

 peak_ptr=-1;
 peak_flag=false;
 num_peaks=0;

 for (cur_idx=0; cur_idx<input->Elements()-1; cur_idx++)
  {
  current_val=fabs((float)(*input)(channel, cur_idx));
  next_val=fabs((float)(*input)(channel, cur_idx+1));
  if (current_val < posthresh) peak_flag=false;
  if (current_val < next_val)               // detect a positive slope
     {
     peak_flag=true;
     peak_ptr=cur_idx;
     }
  else
     if ((current_val > next_val) && (peak_flag) && ((float)(*input)(channel, cur_idx) > posthresh))            // detect a negative slope
        {
        num_peaks++;
        peak_flag=false;
        }
//        return((cur_idx+peak_ptr+1)/2);
  }

 return(num_peaks);
}


// detect number of negative peaks
int PeakDetector::get_num_neg_peaks(const GenericSignal *input, int channel)
{
int     cur_idx, peak_ptr;
float   current_val, next_val;
bool    peak_flag;
int     num_peaks;

 peak_ptr=-1;
 peak_flag=false;
 num_peaks=0;

 for (cur_idx=0; cur_idx<input->Elements()-1; cur_idx++)
  {
  current_val=fabs((float)(*input)(channel, cur_idx));
  next_val=fabs((float)(*input)(channel, cur_idx+1));
  if (current_val < negthresh) peak_flag=false;
  if (current_val < next_val)               // detect a positive slope
     {
     peak_flag=true;
     peak_ptr=cur_idx;
     }
  else
     if ((current_val > next_val) && (peak_flag) && ((float)(*input)(channel, cur_idx) < negthresh))            // detect a negative slope
        {
        num_peaks++;
        peak_flag=false;
        }
//        return((cur_idx+peak_ptr+1)/2);
  }

 return(num_peaks);
}


