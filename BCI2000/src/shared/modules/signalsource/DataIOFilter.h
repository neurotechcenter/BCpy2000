////////////////////////////////////////////////////////////////////////////////
// $Id: DataIOFilter.h 5500 2016-08-09 13:40:14Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A filter that handles data acquisition from a GenericADC,
//   storing through a GenericFileWriter, and signal calibration into muV.
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
#ifndef DATA_IO_FILTER_H
#define DATA_IO_FILTER_H

#include "GenericFilter.h"
#include "GenericVisualization.h"
#include "EventQueue.h"
#include "TimeUtils.h"
#include "ReusableThread.h"

#include <vector>
#include <queue>

class GenericFileWriter;
class GenericADC;

class DataIOFilter: public GenericFilter
{
 public:
          DataIOFilter();
  virtual ~DataIOFilter();
  virtual void Publish();
  virtual void AutoConfig( const SignalProperties& Input );
  virtual void Preflight( const SignalProperties& Input,
                                SignalProperties& Output ) const;
  virtual void Initialize( const SignalProperties& Input,
                           const SignalProperties& Output );
  virtual void StartRun();
  virtual void StopRun();

  virtual void Process( const GenericSignal& Input,
                              GenericSignal& Output );
  virtual void Resting( const GenericSignal& Input,
                              GenericSignal& Output );
  virtual void Halt();

  virtual bool AllowsVisualization() const { return false; }
  virtual bool TimedCalls() const { return false; }

 private:
  bool ParameterUnconfigured( const char* ) const;
  void DoProcess( GenericSignal&, bool );
  void AdjustProperties( SignalProperties& ) const;
  void AcquireData();
  static void Downsample( const GenericSignal& Input,
                                GenericSignal& Output );
  static void CopyBlock( const GenericSignal& Input,
                               GenericSignal& Output,
                               int            block );
  void ProcessBCIEvents();
  void DoVisualize();

  GenericADC*            mpADC;
  GenericFilter*         mpSourceFilter;
  GenericFileWriter*     mpFileWriter;
  GenericSignal          mOutputBuffer;
  StateVector            mStatevectorBuffer;

  bool                   mVisualizeSource,
                         mVisualizeTiming;
  int                    mVisualizeSourceDecimation,
                         mVisualizeSourceBufferSize;
  GenericVisualization   mSourceVis,
                         mTimingVis;
  GenericSignal          mDecimatedSignal,
                         mTimingSignal;
  mutable GenericSignal  mADCOutput,
                         mSourceFilterOutput,
                         *mpFileWriterInput,
                         mVisSourceBuffer;
  int                    mBlockCount;
  double                 mBlockDuration;
  int                    mSampleBlockSize;
  int                    mPrevStimulusTime;
  EventQueue             mBCIEvents;
  Time                   mAcquired;
  bool                   mWasResting;

  struct Private;
  Private* p;
};

#endif // DATA_IO_FILTER_H
