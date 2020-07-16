/******************************************************************************
 * $Id: TMSiADC.cpp 4653 2013-11-22 18:55:50Z mellinger $                                                                       *
 * Program:   TMSI.EXE                                                        *
 * Module:    TMSiADC.CPP                                                     *
 * Comment:   Definition for the TMSiADC class                                *
 * Version:   0.05                                                            *
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
 *                                                                            *
 ******************************************************************************/
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

//#define DEBUG

#include "TMSiADC.h"

#include <tchar.h>

static const int cImpedanceRate = 8; // impedance sampling rate in Hz

using namespace std;

RegisterFilter( TMSiADC, 1 );

TMSiADC::TMSiADC()
: mpMaster( NULL ),
  mValuesToRead( 0 ),
  mBufferSize( 0 ),
  mSrate( 0 ),
  mBufferMulti( 0 ),
  mHardwareCh( 0 ),
  mSoftwareCh( 0 ),
  mSampleBlockSize( 0 ),
  mSampleRate( 0 ),
  mDigitalChannel( 0 )
{
        for( size_t i = 0; i < sizeof( mSignalBuffer ) / sizeof( *mSignalBuffer ); ++i )
                mSignalBuffer[ i ] = 0;

        BEGIN_PARAMETER_DEFINITIONS
        "Source:TMSiADC int       SourceCh=            16      19       1    % // number of digitized and stored channels",
        "Source:TMSiADC int       SampleBlockSize=     12      10       1    % // number of samples transmitted at a time (multiples of 6 seem smoothest)",
        "Source:TMSiADC int       SamplingRate=       250Hz   250     125 2000 // SampleRate of the Input Device",
        "Source:TMSiADC intlist   PhysicalChannels= 16  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 "
                                                               "1       1    % // hardware indices of channels to record",
        "Source:TMSiADC floatlist SourceChGain=     16  0.0715 0.0715 0.0715 0.0715 0.0715 0.0715 0.0715 0.0715 0.0715 0.0715 0.0715 0.0715 0.0715 0.0715 0.0715 0.0715 "
                                                               "0.0715  0    % // ",
        "Source:TMSiADC floatlist SourceChOffset=   16  0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
                                                               "0       %    % // ",
        END_PARAMETER_DEFINITIONS


        mBufferMulti = OptionalParameter("TMSiBufferSizeInSampleBlocks", 4);
        double x = 100.0 / (double)mBufferMulti;
        if(x != floor(x)) bcierr << "TMSiBufferSizeInSampleBlocks should be an integer factor of 100" << endl;

        int priority = OptionalParameter("TMSiProcessPriority", 0);
        switch(priority)
        {
            case +3: SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS); break;
            case +2: SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS); break;
            case +1: SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS); break;
            case  0: SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS); break;
            case -1: SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS); break;
            case -2: SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS); break;
            default: bcierr << "unsupported value --TMSiProcessPriority=" << priority << endl;
        }
        mSleepMsec = OptionalParameter("TMSiSleepMsec", 1);

        mMeasureImpedance = bool(int(OptionalParameter("TMSiCheckImpedance", 0)));

        StartDriver();

        if( mpMaster )
          mpMaster->Reference((BOOL)(OptionalParameter("TMSiAutoReference", 1)));
}

TMSiADC::~TMSiADC()
{
        Halt();
}

void
TMSiADC::StartDriver()
{
        if( !RTDevice().InitOk )
        {
          const char lib[] = RTLOADER;
          bcierr << "Could not find library: " << lib + 1 << "\n"
                 << "Make sure that you have installed the drivers coming with your amplifier.";
          return;
        }
        // Calling functions from the TMSI SDK
        for(ULONG Index=0;Index < MAX_DEVICE;Index++)
                mpDevice[Index] = NULL;

        UseMasterSlave( mpDevice , MAX_DEVICE );

        mpMaster = mpDevice[0];
        if( mpMaster == NULL )
        {
                bcierr <<  "TMSiADC No actual master device is found" << endl;
                return;
        }

        mpMaster->Reset();
        PSIGNAL_FORMAT psf = mpMaster->GetSignalFormat( NULL );

        if( psf != NULL )
        {
                SIZE_T Size = LocalSize( psf );
                if( Size < sizeof( SIGNAL_FORMAT ) * psf->Elements )
                {
                        bcierr << "SignalFormat Error" << endl;
                        return;
                }

                mHardwareCh = psf->Elements;
                Gain = std::vector<double>( mHardwareCh, 0);
                Offset = std::vector<double>( mHardwareCh, 0);
#ifdef DEBUG
                std::string _format[3]={"Unsigned Integer","Signed Integer","Floating Point"};
                std::string _units[6]={"BIT","VOLT^-6","PERCENT","BPM","BAR","PSI"};
#endif
                for (unsigned int i=0; i< mHardwareCh; i++)
                {
                        int Exponent=psf[i].UnitExponent;
                        if (mMeasureImpedance)
                        {
                            Gain[i]   = 1.0;
                            Offset[i] = 0.0;
                        }
                        else {
                            if (Exponent)
                            {
                                    if (psf[i].UnitId==1)
                                    {     // if volts
                                            Gain[i]         = (psf[i].UnitGain  *(pow(10.0,Exponent)))*1000000;     // in uV
                                            Offset[i]       = (psf[i].UnitOffSet*(pow(10.0,Exponent)))*1000000;
                                    }
                            }
                            else {
                                    mDigitalChannel = i+1;
                                    Gain[i]   = 1.0;
                                    Offset[i] = 0.0;
                            }
                        }
#ifdef DEBUG
                        bciout << "Physical channel #" <<i+1<< " - Gain: " << Gain[i] << " Offset : " << Offset[i] <<
                                " Format: "  << _format[psf[i].Format].c_str() <<
                                " Unit: " << ((psf[i].UnitId > 5)? "unknown" :_units[psf[i].UnitId].c_str()) << std::endl;;
#endif
                }
#ifdef DEBUG
                bciout << "Device has " << mHardwareCh << " Channels available" << std::endl;
#endif
                mpMaster->Free( psf );
        }
        else
                bcierr << "Signalformat Error" << endl;
}

void
TMSiADC::Preflight( const SignalProperties&, SignalProperties& outputProperties ) const
{      // Might need a bit of work...

        int softwareCh = Parameter( "SourceCh" );
        if ( softwareCh > static_cast<int>( mHardwareCh ) )
        {
         bcierr << "Trying to read more channels than available" << std::endl;
        }
        outputProperties        = SignalProperties( softwareCh,
                                                    Parameter( "SampleBlockSize" ),
                                                    SignalType::float32 );
        if ( Parameter("PhysicalChannels")->NumValues() != softwareCh )
        {
            bcierr << "Number of elements in PhysicalChannels must match SourceCh" << endl;
        }
        bool goodVals = true;
        bool gainWarning = false;
        bool offsetWarning = false;
        for ( int i = 0; i < softwareCh ; ++i )
        {
            SIZE_T ind = int(Parameter("PhysicalChannels")(i)) - 1;
            if(ind < 0) bcierr << "A PhysicalChannels index of " << ind+1 << "is illegal" << endl;
            if(ind >= mHardwareCh) bcierr << "A PhysicalChannels index of " << ind+1 << "exceeds the hardware's maximum of " << mHardwareCh << endl;

            //Impedance has gain/offset of 1/0, warn if different but set anyway
            if ( mMeasureImpedance )
            {
                if (float(Parameter("SourceChGain")(i)) != Gain[ind] && !gainWarning)
                {
                    bciout << "Using user-defined SourceChGain values for impedance measurement, and assuming that you have correctly calibrated these ";
                    bciout << "(e.g. channel " << i+1 << " has gain " << Parameter("SourceChGain")(i) << ")." << std::endl;
                    gainWarning = true;
                }
                if (float(Parameter("SourceChOffset")(i)) != Offset[ind] && !offsetWarning)
                {
                    bciout << "Using user-defined SourceChOffset values for impedance measurement, and assuming that you have correctly calibrated these ";
                    bciout << "(e.g. channel " << i+1 << " has offset " << Parameter("SourceChOffset")(i) << ")." << std::endl;
                    offsetWarning = true;
                }
            }
            //Check whether BCI Gains are correct
            else
            {
                double prmVal = Parameter("SourceChGain")(i);
                double hardwareVal = Gain[ind];
                bool same = ( 1e-3 > ::fabs( prmVal - hardwareVal ) / ( hardwareVal ? hardwareVal : 1.0 ) );
                goodVals &= same;
                if ( !same ) bciout << "The amp driver says the gain of"
                                    << " channel " << ind+1
                                    << " is " << hardwareVal
                                    << " whereas the corresponding value in the"
                                    << " SourceChGain parameter is " << prmVal << endl;
            }
        }
        if( !goodVals )
          bcierr << "The SourceChGain values "
                 << "must match the hardware channel resolutions"
                 << endl;

        //Sampling Rate has to be 8Hz (Check for other Amps?), warn if data is updated less than once a second
        if ( mMeasureImpedance )
        {
            int trueSamplingRate = cImpedanceRate;
            if (int(Parameter("SamplingRate").InHertz()) != trueSamplingRate){
              bciout << "For impedance measurement, using a Sampling rate of " << trueSamplingRate << "Hz, rather than " << int(Parameter("SamplingRate").InHertz()) << "Hz." << std::endl;
            }
            if (int(Parameter("SampleBlockSize")) > 8){
              bciout << "SampleBlockSize=" << int(Parameter("SampleBlockSize")) << " is very big for a SamplingRate of " << trueSamplingRate << "\n";
            }
        }
        else {
            goodVals = true;
            for ( int i = 0; i < softwareCh ; ++i )
            {
                int ind = int(Parameter("PhysicalChannels")(i)) - 1;
                double prmVal = Parameter("SourceChOffset")(i);
                double hardwareVal = - Gain[ind] * Offset[ind]; // amp offset is used as y = g*x + o  whereas SourceChOffset is used as y = (x - o) * g
                bool same = ( 1e-3 > ::fabs( prmVal - hardwareVal ) / ( hardwareVal ? hardwareVal : 1.0 ) );
                goodVals &= same;
                if ( !same ) bciout << "The amp driver says the offset of"
                                    << " channel " << ind+1
                                    << " is " << hardwareVal
                                    << " whereas the corresponding value in the"
                                    << " SourceChOffset parameter is " << prmVal << endl;
            }
            if( !goodVals )
              bcierr << "The SourceChOffset values "
                     << "must match the hardware channel offsets"
                     << endl;
       }
}

void
TMSiADC::Initialize( const SignalProperties&, const SignalProperties& )
{
        mSoftwareCh           = Parameter( "SourceCh" );
        mSampleBlockSize      = Parameter( "SampleBlockSize" );
        mSampleRate           = static_cast<unsigned int>( Parameter( "SamplingRate" ).InHertz() );

        mBufferSize           = mBufferMulti*mSampleBlockSize; // in samples!: in waitfordata endblock is linked to this...
        mSrate                = 1000*mSampleRate;           // samplerate in mHz

        mPhysChanInd.clear();
        for ( SIZE_T i = 0; i < mSoftwareCh ; ++i) mPhysChanInd.push_back(int(Parameter("PhysicalChannels")(i)) - 1);
        mpMaster->SetSignalBuffer(&mSrate,&mBufferSize);

        //I wonder where the difference to mBuffersize is...
        mValuesToRead         = mSampleBlockSize * mHardwareCh * 4;   // sizeof type? type dependent? TMS appears to only give 4 byte values.

        if ( mMeasureImpedance )
        {
            mpMaster->MeasuringMode( MEASURE_MODE_IMPEDANCE, 0);
            mSampleRate = cImpedanceRate;
        }

        if (!mpMaster->Start())
                bcierr << "TMSiADC Initialize returned an error (Device Start)" << endl;
}

void
TMSiADC::Halt()
{
        if (mpMaster && mpMaster->GetDeviceState() == 1)
                mpMaster->Stop(); // if running
}

void
TMSiADC::Process(const GenericSignal&, GenericSignal& outputSignal)
{
        if (WaitForData(&mSignalBuffer[0], mValuesToRead)== TMSIOK)
        {
                const LONG * base = mSignalBuffer;
                for( unsigned int sample = 0; sample < mSampleBlockSize; ++sample )
                {
                        for( unsigned int channel = 0; channel < mSoftwareCh; ++channel )
                        {
                            int phys = mPhysChanInd[channel];
                            if(phys == mDigitalChannel - 1) outputSignal(channel, sample) = (float)((UCHAR)(~base[phys]));
                            else outputSignal( channel, sample ) = (float)(base[phys]);
                        }
                        base += mHardwareCh;
                }
        }
        else
                bcierr << "Error reading data" << endl;
}


//----------------------------------------------------------------------------------------------------
//
//  25/10/05 MMS
//
//  WaitForData:
//
//  Blocking function that fills the SignalBuffer with 'size' samples.
//
//
//
//----------------------------------------------------------------------------------------------------
int
TMSiADC::WaitForData(LONG* SignalBuffer,ULONG size)
{
        ULONG PercentFull, Overflow;
        static unsigned int mOverflow = 0;
        ULONG endblock = static_cast<ULONG>( 100.00 / mBufferMulti );
        mpMaster->GetBufferInfo(&Overflow,&PercentFull);

        while( PercentFull < endblock)
        {
                Sleep(mSleepMsec);
                mpMaster->GetBufferInfo(&Overflow,&PercentFull);
        }

        if (Overflow>mOverflow)
        {
                mOverflow=Overflow;
                bcierr << "Overflow occurred: " << Overflow  << " % Full= " << PercentFull << endl;
        }

        ULONG BytesReturned = mpMaster->GetSamples((PULONG)SignalBuffer,size);

        if (BytesReturned!=size)
                bciout << "BytesReturned: "<< BytesReturned <<"; wanted: "<< size <<endl;

        return TMSIOK;
}


//----------------------------------------------------------------------------------------------------
//
//  25/10/05 MMS
//
//  Following function is (almost) verbatim from the TMSi SDK. Needed for driver loading
///----------------------------------------------------------------------------------------------------

ULONG
TMSiADC::UseMasterSlave( RTDeviceEx **Devices , ULONG Max )
{
        ULONG x;

        for(x = 0 ; x < Max ;x++ )
        {
                Devices[x] = new RTDeviceEx(x);
                if( Devices[x] == NULL ) break;

                if( !Devices[x]->InitOk)
                {
                        delete Devices[x];
                        Devices[x] = NULL;
                        break;
                }
        }

        ULONG NrOfDevices = x;
        for( x = 0 ; x < NrOfDevices ; x++ )
        {
                TCHAR DeviceName[40] = _T("Unknown Device");
                ULONG SerialNumber = 0;
                ULONG Size;

                Size = sizeof( SerialNumber );
                RegQueryValueEx( Devices[x]->DeviceRegKey , _T("DeviceSerialNumber"), NULL , NULL , (PBYTE)&SerialNumber , &Size  );

                Size = sizeof( DeviceName );
                RegQueryValueEx( Devices[x]->DeviceRegKey , _T("DeviceDescription"), NULL , NULL , (PBYTE)&DeviceName[0] , &Size  );

                if( x!= 0 )
                {
                        HANDLE SlaveHandle = Devices[x]->GetSlaveHandle();
                        if(SlaveHandle == 0 )
                        {
                                bcierr << "Unable to get a handle from device " << (x + 1) << endl;
                                break;
                        }
                        Devices[0]->AddSlave(SlaveHandle);
                }
        }
        return NrOfDevices;
}


