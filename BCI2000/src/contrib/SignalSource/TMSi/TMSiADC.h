/******************************************************************************
 * $Id: TMSiADC.h 4653 2013-11-22 18:55:50Z mellinger $                                                                       *
 * Program:   TMSI.EXE                                                        *
 * Module:    TMSiADC.H                                                       *
 * Comment:   Definition for the TMSiADC class                                *
 * Version:   0.04                                                            *
 * Author:    M.M.Span                                                        *
 * Copyright: (C) RUG University of Groningen                                 *
 ******************************************************************************
 * Version History:                                                           *
 *                                                                            *
 * V0.01 - 12/10/2005 - First start                                           *
 * V0.02 - 27/10/2005 - Primary working version: entered testing phase        *
 * V0.03 - 25/01/2006 - Multiple devices (PORTI) useable                      *
 *                      Synchronizing PORTIs work in progress                 *
 * V0.04 - 03/04/2006 - Porti Synchro disbanded. Working on selecting channels*
 * V0.05 - 15/05/2006 - Using the features pull out unused channels from the  *
 *                      common reference pool                                 *
 *
 * Revision 1.1  2006/07/04 18:45:50  mellinger
 * Put files into CVS.
 *
 * Revision 1.2  2006/07/05 15:20:10  mellinger
 * Minor formatting and naming changes; removed unneeded data members.
 *
 * Revision 2.0  2009/10/25 jhill
 * - Allow selection of a subset of physical channels to acquire.
 * - Support impedance measurement and acquisition of digital channel values.
 * - Various OptionalParameters for tweaking performance.
 * - Crash fixes.
 *
 ******************************************************************************/
// MMS adaption from the TMSiDemo in driver
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

#ifndef TMSiADCH
#define TMSiADCH

// #define DEBUG

#include "GenericADC.h"
#include <windows.h>
#include <vector>

#define ClassName ClassName_
#include "RTDevice.h"     // from TMSi driver
#include "Feature.h"      // from TMSi driver

#define MAX_DEVICE                  20 //Max number of devices supported
#define USE_MASTER_SLAVE            TRUE

#define MEASURE_MODE_NORMAL         ((ULONG)0x0)
#define MEASURE_MODE_IMPEDANCE      ((ULONG)0x1)
#define MEASURE_MODE_CALIBRATION    ((ULONG)0x2)
#define MEASURE_MODE_IMPEDANCE_EX   ((ULONG)0x3)
#define MEASURE_MODE_CALIBRATION_EX ((ULONG)0x4)
#define MAXBUFFERSIZE               4*1024*1024
#define TMSIOK  0
#define TMSIERR 1

class TMSiADC : public GenericADC
{
 public:
               TMSiADC();
  virtual      ~TMSiADC();

  void Preflight( const SignalProperties&, SignalProperties& ) const;
  void Initialize( const SignalProperties&, const SignalProperties& );
  void Process( const GenericSignal&, GenericSignal& );
  void Halt();

 private:
  int   WaitForData(LONG*,ULONG);
  void  StartDriver();
  ULONG UseMasterSlave( RTDeviceEx** Devices , ULONG Max );

 private:
  RTDeviceEx*  mpDevice[MAX_DEVICE];
  RTDeviceEx*  mpMaster;

  LONG         mSignalBuffer[MAXBUFFERSIZE];
  ULONG        mValuesToRead;
  ULONG        mBufferSize;
  ULONG        mSrate;
  ULONG        mSleepMsec;

  unsigned int mBufferMulti;

  std::vector<double> Gain;
  std::vector<double> Offset;

  std::vector<int>    mPhysChanInd;

  boolean             mMeasureImpedance;

  unsigned int mHardwareCh,
               mSoftwareCh,
               mSampleBlockSize,
               mSampleRate,
               mDigitalChannel;
};

#endif // TMSiADCH
