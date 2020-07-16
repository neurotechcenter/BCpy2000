////////////////////////////////////////////////////////////////////////////////
// $Id: BioRadioADC.h 4373 2013-02-25 13:21:16Z mellinger $
// $Log$
// Revision 1.2  2006/07/05 15:21:19  mellinger
// Formatting and naming changes.
//
// Revision 1.1  2006/07/04 18:44:25  mellinger
// Put files into CVS.
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
#ifndef BioRadioADCH
#define BioRadioADCH

#include "GenericADC.h"
#include "BR_defines.h"
#include "BR150.h"
#include "FileUtils.h"

class BioRadioADC : public GenericADC
{
 public:
                BioRadioADC();
  virtual       ~BioRadioADC();

  virtual void  Preflight( const SignalProperties&, SignalProperties& ) const;
  virtual void  Initialize( const SignalProperties&, const SignalProperties& );
  virtual void  Process( const GenericSignal&, GenericSignal& );
  virtual void  Halt();

 private:
          void  GetData(int channel);
          void  ClearSampleIndices();

 private:
  int           mSamplerate;
  int           mSourceCh;
  int           mSampleBlockSize;
  int           mVoltageRange;
  const double* mpIndex;
  BR150         mBioRadio150;
  int           mDataRead;
  int           mTracker;
  int           mSampleIndex[ALL_CHANNELS];
  int           mComPort;
  mutable FileUtils::TemporaryFile mTempFile;
 };

#endif // BioRadioADCH

