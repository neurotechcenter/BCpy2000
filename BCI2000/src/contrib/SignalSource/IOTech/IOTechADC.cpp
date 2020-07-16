////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors:
// Description: IOTechADC implementation
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
#include "IOTechADC.h"
#include "BCIStream.h"
#include "BCIEvent.h"


#include "ThreadUtils.h"

using namespace std;

// Make the source filter known to the framework.
RegisterFilter( IOTechADC, 1 ); // ADC filters must be registered at location "1" in order to work.

IOTechADC::IOTechADC()
: mDeviceHandle( ), // Each plain data member should appear in the initializer list.
  mpBuffer( 0 ), // Make sure we can call delete[] without crashing even if we never called new[].
  mSourceCh( 0 ),
  mSampleBlockSize( 0 ),
  mSamplingRate( 0 ),
  init( 0 )
{
}

IOTechADC::~IOTechADC()
{
  // The destructor is where to make sure that all acquired resources are released.
  // Memory deallocation (calling delete[] on a NULL pointer is OK).
  delete[] mpBuffer;
}

void
IOTechADC::OnPublish()
{
 BEGIN_PARAMETER_DEFINITIONS

    "Source:Signal%20Properties int SourceCh= 64 64 1 128 "
       "auto 1 % // number of digitized and stored channels",

    "Source:Signal%20Properties int SampleBlockSize= 16 5 1 128 "
       "auto 1 % // number of samples transmitted at a time",

    "Source:Signal%20Properties float SamplingRate=  128 128 1 4000 "
       "auto 0.0 % // sample rate",

    "Source:Signal%20Properties list SourceChGain= 1 auto "
       " // physical units per raw A/D unit",

    "Source:Signal%20Properties list SourceChOffset= 1 auto "
       " // raw A/D offset to subtract, typically 0",

    "Source:Signal%20Properties list ChannelNames= 1 auto "
       " // names of amplifier channels",
	   
 END_PARAMETER_DEFINITIONS

}

void
IOTechADC::OnAutoConfig()
{

}

void
IOTechADC::OnPreflight( SignalProperties& Output ) const
{
  // The user has pressed "Set Config" and we need to sanity-check everything.

  // Internally, signals are always represented by GenericSignal::ValueType == double.
  // Here, you choose the format in which data will be stored, which may be
  // int16, int32, or float32.
  // Typically, you will choose the format that your amplifier natively provides, in
  // order to avoid loss of precision when converting, e.g., int32 to float32.
  SignalType sigType = SignalType::float32;
  

  int samplesPerBlock = Output.Elements();
  int numberOfSignalChannels = Output.Channels();

  if ( Parameter( "ChannelNames" )->NumValues() != Parameter( "SourceCh" ) )
	  bcierr << "Number of columns in Channels must equal value of SourceCh.";

  Output = SignalProperties( numberOfSignalChannels, samplesPerBlock, sigType );
  // A channel name starting with @ indicates a trigger channel.
  // Note that the IOTechADC instance itself, and its members, are read-only during the
  // Preflight phase---note the "const" at the end of the OnPreflight prototype above.
  // Any methods called by OnPreflight must also be declared as "const" in the same way.
}

void
IOTechADC::OnInitialize( const SignalProperties& Output )
{
  mSourceCh = Parameter( "SourceCh" );
  mSampleBlockSize = Parameter( "SampleBlockSize" );
  mSamplingRate = Parameter( "SamplingRate" );

  pDAQ = new CDaqCOMInterface();
  BOOL res = pDAQ->Initialize();
  if (res)
    {
        pDAQ->m_Rate = mSamplingRate;
        pDAQ->m_Scans = mSampleBlockSize;
        res = pDAQ->OpenDevice();
	} else {
		bcierr << "Unable to initialize IOTechDAQ3000."
           << endl;
	}
    if (res)
    {
		ParamRef Chs = Parameter( "ChannelNames" );
		int i=0;
		for( i=0; i<mSourceCh; i++ )
		{
			ChannelIDs[i] = Chs( i );
		}
		res = pDAQ->SetupChannels( ChannelIDs, mSourceCh );
	} else {

		bcierr << "Unable to open IOTechDAQ3000."
           << endl;

	}

	init = 1;

	pDAQ->Start();

  // The signal properties can no longer be modified, but the const limitation has gone, so
  // the IOTechADC instance itself can be modified. Allocate any memory you need
  // store any information you need in private member variables.
  mNumberOfSignalChannels = Parameter( "SourceCh" );
  int bufferBytes = Output.Channels() * Output.Elements() * sizeof( float );
  delete[] mpBuffer;
  mpBuffer = new char[bufferBytes];
  ::memset( mpBuffer, 0, bufferBytes );
  // Buffer allocation may happen in OnStartAcquisition as well, if more appropriate.
 
}

void
IOTechADC::OnStartAcquisition()
{
  // This method is called from the acquisition thread once the system is initialized.
  // You should use this space to start up your amplifier using its API.  Any initialization
  // here is done in the acquisition thread, so non-thread-safe APIs should work if initialized here.

}

void
IOTechADC::DoAcquire( GenericSignal& Output )
{
  // Now we're acquiring a single SampleBlock of data in the acquisition thread, which is stored
  // in an internal buffer until the main thread is ready to process it.

  // Internally, BufferedADC writes this data to a buffer, then locks a mutex and pushes the data
  // into the GenericSignal output in the main thread.  The size of this buffer is parameterized by
  // "SourceBufferSize" declared by BufferedADC, which will be interpreted as follows:
  // * When set to a naked number, the number will be rounded to the nearest integer, and specify
  //   the number of BCI2000 data blocks (cf the SampleBlockSize parameter). The buffer's
  //   duration in time will vary with changes to the SamplingRate and SampleBlockSize parameters.
  // * When set to a number followed with an SI time unit, the buffer's size will be automatically
  //   chosen to match the specified time duration as close as possible. By default, the value is 2s.
  //   SI time units must be appended without white space, and consist of
  //   a valid SI prefix (such as m for milli=1e-3, mu for micro=1e-6, k for kilo=1e3),
  //   followed with a lowercase s (for seconds).

  // Keep in mind that even though we're writing this data from another thread, the main thread
  // cannot continue without data, so be sure this is done in a timely manner
  // or the system will not be able to perform in real-time.

  // IMPORTANT NOTES ABOUT BUFFERING

  // Ideally, the BCI2000 processing time ("Roundtrip time") is always shorter than the physical
  // duration of a sample block, i.e. every single block of data has been processed before its
  // successor arrives. In that ideal case, buffering makes no difference, because the main thread will
  // always wait for the acquisition thread to deliver the next block of data into the internal
  // buffer, and copy it from there immediately.

  // If, on average, processing takes longer than the physical duration of a sample block, buffering
  // will not help to improve things either, because the buffer will be filled faster than
  // it is being emptied, and thus it will overflow after a certain time. Depending on buffering strategy,
  // buffer overflow may be dealt with by looping (i.e., overwriting data that has not been delivered yet),
  // or by blocking (not accepting any new data before there is space in the buffer).
  // For scientific purposes -- as opposed to, e.g., entertainment applications -- silently omitting
  // data is not an option, so BufferedADC will use the blocking strategy, and deliver all data blocks,
  // but delayed by the physical duration of its buffer.

  // So the _only_ situation in which buffering is actually useful is for cases when processing is not
  // able to keep up with data acquisition for short amounts of time. Typical examples are lengthy 
  // computations that happen from time to time, such as updating a classifier matrix, or initialization
  // work in algorithm implementations that are not factored out into initialization and update operations
  // (otherwise, you would just do lengthy operations inside Initialize()).
  // In such cases, you should set the SourceBufferSize parameter to an estimate of the longest expected
  // delay.


  float* rgElems;
  SAFEARRAY *psa = SafeArrayCreateVector(VT_R4, 0, mSampleBlockSize * mSourceCh);
  int retScans = pDAQ->FetchData(psa);

  if (retScans==mSampleBlockSize)
    {
        SafeArrayAccessData(psa, (void**)&rgElems);

        for(int chan = 0; chan < mSourceCh; chan++)
            for (int sample = 0; sample < mSampleBlockSize; sample++)
				Output( chan, sample ) = rgElems[mSourceCh*sample + chan];

        SafeArrayUnaccessData(psa);
    }
    else
    {
	    bcierr << "Error reading data" << endl;
    }
}

void
IOTechADC::OnStopAcquisition()
{
  // This method is called from the acquisition thread just before it exits.  Use this method
  // to shut down the amplifier API (undoing whatever was done in OnStartAcquisition).
  // Immediately after this returns, the system will go into an un-initialized state and
  // OnHalt will be called in the main thread.

	if ( init == 1 )
		pDAQ->Stop();
}

