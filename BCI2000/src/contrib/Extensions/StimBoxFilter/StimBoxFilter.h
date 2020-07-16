////////////////////////////////////////////////////////////////////////////////
// $Id: StimBoxFilter.h 4536 2013-08-05 14:30:13Z mellinger $
// Author: griffin.milsap@gmail.com
// Description: A filter which controls a g.STIMbox
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
#ifndef STIM_BOX_FILTER_H
#define STIM_BOX_FILTER_H

#include "GenericFilter.h"
#include "Expression/Expression.h"
#include "Thread.h"
#include "Mutex.h"

#include "gSTIMbox.imports.h"

#include <map>
#include <vector>

class StimBoxFilter : public GenericFilter
{
 public:
  // Constructor/Destructor
  StimBoxFilter();
  ~StimBoxFilter();

  friend class StimBoxThread;

 protected:
  // Virtual Interface
  void AutoConfig( const SignalProperties& ) override;
  void Preflight( const SignalProperties& Input,
                        SignalProperties& Output ) const override;
  void Initialize( const SignalProperties& Input,
                   const SignalProperties& Output ) override;
  void Process( const GenericSignal& Input,
                      GenericSignal& Output ) override;
  void StartRun() override;
  void StopRun() override;
  void Halt() override;

 private:
  // Private member methods
  void DisablePorts();

  // Private member variables
  int mPortNumber;
  HANDLE mStimBox;
  std::map< int, Expression > mDigitalOutputExpMap;

  template<class T> struct Vector : std::vector<T>
  { int Size() const { return static_cast<int>( size() ); } };
  Vector< int >    mOutputPorts,
                   mInputPorts,
                   mModeselektor,
                   mOutputPortStates;
  Vector< double > mOutputPortFreqs;

  class StimBoxThread : public Thread
  {
   public:
    StimBoxThread( StimBoxFilter* inFilt ) : mpFilter( inFilt ) {}
    ~StimBoxThread() {}
    int OnExecute() override;

    LockableObject& GetLock() { return mDataLock; }

   private:
    Lockable<Mutex> mDataLock;
    StimBoxFilter *mpFilter;

  } *mpStimBoxThread;
};

#endif // STIM_BOX_FILTER_H
