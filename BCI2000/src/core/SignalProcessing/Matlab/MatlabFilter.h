////////////////////////////////////////////////////////////////////////////////
// $Id: MatlabFilter.h 4651 2013-11-22 16:48:25Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: This BCI2000 filter calls the Matlab engine to act upon signals,
//    parameters, and states, thus providing the full BCI2000 filter interface
//    to a Matlab filter implementation.
//
//    For each BCI2000 filter member function, there is a corresponding Matlab
//    function as follows:
//
//    GenericFilter member      Matlab function syntax
//    ====================      ======================
//    Constructor               [parameters, states] = bci_Construct
//    Destructor                bci_Destruct
//    Preflight                 out_signal_dim = bci_Preflight( in_signal_dim )
//    Initialize                bci_Initialize( in_signal_dim, out_signal_dim )
//    Process                   out_signal = bci_Process( in_signal )
//    StartRun                  bci_StartRun
//    StopRun                   bci_StopRun
//    Resting                   bci_Resting
//    Halt                      bci_Halt
//
//    Existence of the above-listed Matlab functions is not mandatory.
//    The MatlabFilter uses the Matlab 'exist' command to determine whether a
//    given function is available, and will not call the Matlab engine when this
//    is not the case.
//    If either of the bci_Preflight, bci_Initialize, or bci_Process functions
//    is not available, a warning will be displayed to the user.
//
//    Parameters and states are accessible via global Matlab structs called
//    'bci_Parameters' and 'bci_States'. In Matlab, write
//
//      global bci_Parameters bci_States;
//      my_sampling_rate = bci_Parameters.SamplingRate;
//
//    Parameters may be changed from 'bci_StopRun' and 'bci_Resting', and will
//    automatically be propagated to the other modules.
//    State values may be modified from the 'bci_Process' function.
//
//    To add parameters and states to the BCI2000 list of states, the 'bci_Construct'
//    function may return non-empty cell arrays of strings in its 'parameters'
//    and 'states' return values. The strings constituting these cell arrays must
//    follow the BCI2000 parameter/state definition syntax as described in sections
//    3.2.4 and 3.2.5 of the "BCI2000 project outline" document.
//
//    BCI2000 signals are mapped to Matlab matrices with the channel index first,
//    and sample (element) index second.
//    Signal dimension arguments of bci_Preflight and bci_Initialize are
//    vectors of integers (1x2 matrices) as in '[n_channels n_elements]'.
//
//    To report errors from Matlab functions, use Matlab's error() command.
//
//    Troubleshooting:
//    If no Matlab instance opens up, execute
//      matlab /regserver
//    from the command line when logged in with administrative privileges.
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
#ifndef MATLAB_FILTER_H
#define MATLAB_FILTER_H

#include "GenericFilter.h"
#include "MatlabWrapper.h"
#include "SharedMemory.h"

class MatlabFilter : public GenericFilter
{
 public:
          MatlabFilter();
  virtual ~MatlabFilter();
  virtual void Publish();
  virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void Initialize( const SignalProperties&, const SignalProperties& );
  virtual void Process( const GenericSignal&, GenericSignal& );
  virtual void StartRun();
  virtual void StopRun();
  virtual void Resting();
  virtual void Halt();

 private:
  void InitializeMatlabWS() const;
  void StatesToMatlabWS() const;
  void MatlabWSToStates();
  void ParamsToMatlabWS() const;
  void MatlabWSToParams();
  void InitializeMatlabProcessing( const SignalProperties&, const SignalProperties& );
  bool CallMatlab( MatlabFunction& ) const;

  enum
  { // Accepted values for the MatlabStayOpen parameter
    closeEngine = 0,
    clearVariables = 1,
    dontClear = 2,
  };
  int mMatlabStayOpen, mSampleBlockSize;
  MatlabFunction* mpBci_Process;
  SharedMemory* mpData;
  int mStatesOffset;
};

#endif // MATLAB_FILTER_H


