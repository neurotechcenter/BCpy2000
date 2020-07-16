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

#ifndef NImxADCH
#define NImxADCH
//---------------------------------------------------------------------------

#include "NIDAQmx.h"

#include "UParameter.h"
#include "UState.h"
#include "UGenericSignal.h"
#include "GenericADC.h"

#define NIDAQ_ERR_GENERICERR    0
#define NIDAQ_ERR_NOERR         1

#define NIDAQ_MAX_CHANNELS      128
#define NIDAQ_MAX_BUFFERS       50      // how many buffers do we want ?

#define NIDAQ_iDBmodeON         1
#define NIDAQ_iDBmodeOFF        0

#define NIDAQ_MODE_CLEARALLMESSAGES   0
#define NIDAQ_MODE_ADDMESSAGE         1
#define NIDAQ_MODE_REMOVEMESSAGE      2

class NIADC : public GenericADC
{
protected:
int16     iStatus;
int32     iRetVal;
int16     iDevice;
int16     iChan;
int16     iGain;
float64     dSampRate;
float64     dScanRate;
float64     dGainAdjust;
float64     dOffset;
int16     iUnits;
int16     iSampTB;
int16     iScanTB;
uInt16     uSampInt;
uInt16     uScanInt;
int16     iHalfReady;
int16     iDAQstopped;
uInt32     ulRetrieved;
int16     iIgnoreWarning;
int16     iYieldON;
int     i, numChans;
uInt32     SampleBlockSize, ulCount;
int16     iNumMUXBrds;
int16     chanVector[NIDAQ_MAX_CHANNELS], gainVector[NIDAQ_MAX_CHANNELS];
int16     *piBuffer, *piHalfBuffer[NIDAQ_MAX_BUFFERS];
// f64     pdVoltBuffer[NIDAQ_MAX_BUFFER];
int32     lTimeout;
uInt32     ulPtsTfr;
TaskHandle taskHandle;
int32      error;
char       deviceName[6];
char       errBuff[2048];

        int     samplingRate;
        int     blocksize;
        int     channels;
private:
        int     Start();
        int     Stop();
        int     ADConfig();
        void    GetData();
        int     cur_buffers;               // how many data buffers do we currently have ?
        TCriticalSection *data_critsec;    // critical section for data FIFO
        static  NIADC* cur_adc;
        static  int32 CVICALLBACK Callback (TaskHandle , int32 , uInt32 , void *);
public:
        NIADC::NIADC();       // overwrite constructor
        NIADC::~NIADC();
        virtual void     Preflight(const SignalProperties&, SignalProperties&) const;
        virtual void     Initialize();
        virtual void     Process(const GenericSignal*, GenericSignal*);
        virtual void     Halt();

};

#endif



