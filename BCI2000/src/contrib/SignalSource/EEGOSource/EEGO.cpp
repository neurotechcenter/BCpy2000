////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors: Steffen Heimes (steffen.heimes@eemagine.com)
// Description: EEGO RT as signal source implementation
//
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
#include "PCHIncludes.h"
#pragma hdrstop

#include <memory>

#include <eemagine/sdk/factory.h>

#include "EEGO.h"
#include "BCIStream.h"

#include "ThreadUtils.h" // for SleepFor()
#include "FileUtils.h" // for working dir loading

RegisterFilter( EEGO, 1 );
     // Change the location as appropriate, to determine where your filter gets
     // sorted into the chain. By convention:
     //  - filter locations within SignalSource modules begin with "1."
     //  - filter locations within SignalProcessing modules begin with "2."
     //       (NB: SignalProcessing modules must specify this with a Filter() command in their PipeDefinition.cpp file too)
     //  - filter locations within Application modules begin with "3."


EEGO::EEGO()
: mpCurrentOpenStream(nullptr)
, mpCurrentAmplifier(nullptr)
, mTriggerIndex(0)
, mLastTriggerLevel(std::numeric_limits<decltype(mLastTriggerLevel)>::max())
, mCurrentTriggerState(0)
{
  // Format: (It can contain newlines)
  // Section DataType Name= Value DefaultValue LowRange HighRange // Comment

 BEGIN_PARAMETER_DEFINITIONS

    "Source:Signal%20Properties int SourceCh= 64 "
       "64 1 88 % // number of digitized and stored channels, channels > 64 are aux channels",

    "Source:Signal%20Properties int SampleBlockSize= 32 "
       "32 1 % // number of samples transmitted at a time",

    "Source:Signal%20Properties int SamplingRate= 500Hz "
       "500Hz 512Hz 1000Hz 1024Hz 2000Hz 2048Hz 4000Hz 4096Hz 8000Hz 8192Hz 16000Hz 16384Hz 0.0 % // sample rate",

	"Source:Signal%20Properties int SourceRefVoltageRange= 150 "
        "150 100 2000 // The voltage range if the reference channels. [mV]",
    
    "Source:Signal%20Properties int SourceBipVoltageRange= 150 " 
		"150 150 5000 // The voltage range if the bipolar channels. [mV]"

 END_PARAMETER_DEFINITIONS


  // ...and likewise any state variables.

 BEGIN_STREAM_DEFINITIONS
  // IMPORTANT NOTE ABOUT BUFFEREDADC AND STATES:
  // * BCI2000 States can only be populated "per sample" if they're populated synchronously.
  // * States can only be populated synchronously by calling State( "Y" )( idx ) = X; in OnProcess.
  // * States can also be populated asynchronously by calling bcievent << "Y" << x; in DoAcquire.
  // * Asynchronous state population using bcievent cannot occur per sample, but only per sampleblock.
  // * If a state changes multiple times in a single sample block, data WILL BE LOST using the bcievent interface.
  // * This presents a problem for many amplifier APIs because many of them must be instantiated and run in a single
  //   thread (they are not thread-safe) and access to API state data must be managed carefully if its needed in the main thread.
  "eegoRTADCSyncTriggers 8 0 0 0", // <name> <bit width> <initial value>
 END_STREAM_DEFINITIONS

}


EEGO::~EEGO()
{
  // You should think twice before deallocating memory here as opposed to OnHalt().
  // OnHalt() is automatically called by BufferedADC upon destruction of this object.
}

void EEGO::AutoConfig(const SignalProperties&)
{
}

void
EEGO::OnHalt()
{
  // De-allocate any memory reserved in OnInitialize, stop any threads, etc.
  // Good practice is to write the OnHalt() method such that it is safe to call it even *before*
  // OnInitialize, and safe to call it twice (e.g. make sure you do not delete [] pointers that
  // have already been deleted:  set them to NULL after deletion).

  // Note that OnStopAcquisition() will be called immediately before this, in the acquisition
  // thread. OnStopAcquisition() is the proper place to do any amplifier-API cleanup.
  delete mpCurrentOpenStream;
  mpCurrentOpenStream = nullptr;

  delete mpCurrentAmplifier;
  mpCurrentAmplifier = nullptr;
  // This method will always be called before OnHalt is called.
}

void
EEGO::OnPreflight( SignalProperties& Output ) const
{
  // The user has pressed "Set Config" and we need to sanity-check everything.
  // For example, check that all necessary parameters and states are accessible:
  //
  // Parameter( "Milk" );
  // State( "Bananas" );
  //
  // Also check that the values of any parameters are sane:
  //
  if( Parameter( "SamplingRate" ).InHertz() < 0.1 )
    bcierr << "SamplingRate cannot be zero" << std::endl;
  //
  // Errors issued in this way, during the Preflight phase, still allow the user
  // to open the Config dialog box, fix bad parameters and re-try.  By contrast,
  // errors and C++ exceptions at any other stage (outside Preflight) will make
  // the system stop, such that BCI2000 will need to be relaunched entirely.

  int numberOfChannels = Parameter( "SourceCh" );
  int samplesPerBlock  = Parameter( "SampleBlockSize" );
  SignalType sigType = SignalType::float32;  // could also parameterize this
  Output = SignalProperties( numberOfChannels + 1, samplesPerBlock, sigType );
  Output.ChannelLabels()[numberOfChannels] = StateMark + std::string( "eegoRTADCSyncTriggers" );

  // TODO: Is this correct to communicate to the system that we have [V] as units?
  // Try to set value range to avoid NaN errors
  for (int ch = 0; ch < Output.Channels() - 1; ++ch)
  {
    PhysicalUnit& u = Output.ValueUnit(ch);
    u.SetSymbol("V");
    u.SetGain(1e-6);
  }

  // Get number of amps and delete them to free the resources.
  auto amps = GetFactory().getAmplifiers();
  auto numAmps = amps.size();
  std::unique_ptr<eemagine::sdk::amplifier> currAmp(nullptr);

  if (numAmps == 1)
  {
	  currAmp.reset(amps.front());
	  amps.erase(amps.begin());
  }
  else if(numAmps == 0)
  {
	  bcierr << "No amplfiers found!\n";
  }
  else
  {
	  bcierr << numAmps << " amplifiers found. Must be exactly 1 available to decide which one to use.\n";
  }

  // Cleanup of all other amplifiers if they exist
  for (auto amp : amps)
  {
	  delete amp;
  }
}

void
EEGO::OnInitialize( const SignalProperties& Output )
{
  // The user has pressed "Set Config" and all Preflight checks have passed.
  // The system will now transition into an "Initialized" state.
  // The signal properties can no longer be modified, but the const limitation has gone, so
  // the EEGO instance itself can be modified. Allocate any memory you need
  // store any information you need in private member variables.

  // As long as the system is in an initialized state, the module should be pushing data from
  // the amplifier through the system.  Initialized != Running, however.  The system can call
  // StartRun and StopRun many times while remaining initialized.  The system will only become
  // uninitialized again once OnHalt is called.

  // Don't bother with any amplifier-API stuff here, however: instead, do this in
  // OnStartAcquisition() which will be called in the acquisition thread immediately after this
  // method finishes.
  // ---- Did this anyway! The streaming thread is to time critical to initialize the amp and the stream. Would run
  // into timeouts if done anyway. And additionally wait for samples to arrive.

  auto samplesPerSecond = Parameter( "SamplingRate" ).InHertz();

  // build path to dll
  // Create path to the dll
  mpCurrentAmplifier = GetFactory().getAmplifier();
  auto ranges = GetRanges(*mpCurrentAmplifier);

  // Open the EEG stream
  mpCurrentOpenStream = mpCurrentAmplifier->OpenEegStream(samplesPerSecond, ranges.reference, ranges.bipolar);

  // Find the trigger channel
  auto triggerChannels = GetChannelIndicesOfType(mpCurrentOpenStream->getChannelList(), eemagine::sdk::channel::channel_type::trigger);
  if (triggerChannels.size() != 1)
  {
    bcierr << "Invalid amount of trigger channels. Exactly 1 needed but we have " << triggerChannels.size() << " instead.";
  }
  else
  {
    mTriggerIndex = triggerChannels[0];
  }

  if (mpCurrentOpenStream->getChannelList().size() < Output.Channels())
  {
	  bciwarn << "More channels are setup for streaming than supported by amplifier! Will only use the first " << mpCurrentOpenStream->getChannelList().size() << " channels!";
  }

  // Flush buffers and return when data is streamed from the amplifier
  mCurrentBuffer = eemagine::sdk::buffer();
  mCurrentBufferPosition = 0;

  // Update Buffer if neccessary
  while (mCurrentBufferPosition >= mCurrentBuffer.getSampleCount())
  {
    mCurrentBufferPosition = 0;
    mCurrentBuffer = mpCurrentOpenStream->getData();

    if (mCurrentBuffer.getSampleCount() == 0)
      ThreadUtils::SleepForMs(1); // Do not overload the system
  }
}

void
EEGO::OnStartAcquisition()
{
  // This method is called from the acquisition thread once the system is initialized.
  // You should use this space to start up your amplifier using its API.  Any initialization
  // here is done in the acquisition thread, so non-thread-safe APIs should work if initialized here.

  // Flush all buffers for a clean start.
  mCurrentBufferPosition = 0;
  mCurrentBuffer = eemagine::sdk::buffer();
  mLastTriggerLevel = std::numeric_limits<decltype(mLastTriggerLevel)>::max();

  // flush
  mpCurrentOpenStream->getData();
}

void
EEGO::DoAcquire(GenericSignal& Output)
{
  // Now we're acquiring a single SampleBlock of data in the acquisition thread.
  // Keep in mind that even though we're writing this data from another thread, the main thread
  // will block until we've written the SampleBlock, so be sure this is done in a timely manner
  // or the system will not be able to perform in real-time.

  // Internally, BufferedADC writes this data to a buffer, then locks a mutex and pushes the data
  // into the GenericSignal output in the main thread.  The size of this buffer (in units of time)
  // is parameterized by "SourceBufferSize" declared by BufferedADC.  Be careful not to overflow!
  auto els = Output.Elements();
  auto numChannels = std::min(Output.Channels() - 1, (int)mpCurrentOpenStream->getChannelList().size());
  for (int el = 0; el < Output.Elements(); el++)
  {
    // Update Buffer if neccessary
    while (mCurrentBufferPosition >= mCurrentBuffer.getSampleCount())
    {
      mCurrentBufferPosition = 0;
      mCurrentBuffer = mpCurrentOpenStream->getData();


      if (mCurrentBuffer.getSampleCount() == 0)
        ThreadUtils::SleepForMs(1); // Do not overload the system
    }

    for (int ch = 0; ch < numChannels; ch++)
    {
      const auto& value = mCurrentBuffer.getSample(ch, mCurrentBufferPosition);
      const auto valueAfter = Output.Properties().ValueUnit(ch).PhysicalToRawValue(value);
      Output.SetValue(ch, el, valueAfter);
    }

    // Trigger check
    // create triggers / state changes
    const unsigned currentTriggerLevel = mCurrentBuffer.getSample(mTriggerIndex, mCurrentBufferPosition);
    const unsigned currentNewTriggers = currentTriggerLevel & ~mLastTriggerLevel;
    mLastTriggerLevel = currentTriggerLevel;

#if 0 // indeed, that would be quite strange
    if (currentNewTriggers != mCurrentTriggerState
      && Phase() != nonaccess)  // ?Test? To avoid to set states when we are not allowed to. Bit strange.
    {
      State("eegoRTADCSyncTriggers")(el) = currentNewTriggers;
      mCurrentTriggerState = currentNewTriggers;
    }
	
#else // should be like this instead:
    Output( Output.Channels() - 1, el ) = currentNewTriggers;
    mCurrentTriggerState = currentNewTriggers;
#endif
    mCurrentBufferPosition++;
  }
}

void
EEGO::StartRun()
{
  // The user has just pressed "Start" (or "Resume")
  // flush buffer
  mCurrentBuffer = eemagine::sdk::buffer();
  mCurrentBufferPosition = 0;
  mpCurrentOpenStream->getData();
}

void
EEGO::StopRun()
{
  // The Running state has been set to 0, either because the user has pressed "Suspend",
  // because the run has reached its natural end.
  // flush buffer
  mCurrentBuffer = eemagine::sdk::buffer();
  mCurrentBufferPosition = 0;
  mpCurrentOpenStream->getData();
}

void
EEGO::OnStopAcquisition()
{
  // This method is called from the acquisition thread just before it exits.  Use this method
  // to shut down the amplifier API (undoing whatever was done in OnStartAcquisition).
  // Immediately after this returns, the system will go into an un-initialized state and
  // OnHalt will be called in the main thread: (there you can undo whatever was done in
  // OnInitialize).

  // flush buffer
  mCurrentBuffer = eemagine::sdk::buffer();
  mCurrentBufferPosition = 0;
  mpCurrentOpenStream->getData();
}

std::vector<unsigned>
EEGO::GetChannelIndicesOfType(const std::vector<eemagine::sdk::channel>& list, const eemagine::sdk::channel::channel_type type) const
{
  using eemagine::sdk::channel;

  // Return value
  std::vector<unsigned> indices;

  // find all channels
  auto channelIt = std::find_if(list.begin(), list.end(), [type](const channel& ch) { return ch.getType() == type; });
  while (channelIt != list.end())
  {
    indices.push_back(channelIt->getIndex());
    channelIt = std::find_if(++channelIt, list.end(), [type](const channel& ch) { return ch.getType() == type; });
  }

  return indices;
}

double EEGO::FindSignalRange(std::vector<double> ranges, const std::string paramName) const
{
	// Find the lowest range which is larger or equal to the specified range. If the range is larger than requested spew a warning and use
	// the largest supported
	auto signalRangeAskedFor = Parameter(paramName) / 1000.; // [mV] -> [V]

	// Should be sorted but to make sure
	std::sort(ranges.begin(), ranges.end());

	// Find smallest value supporting the desired range and warn if no such value could be found.
	auto rangeToUse = std::find_if(ranges.begin(), ranges.end(), [&](double val) { return signalRangeAskedFor <= val; });
	double retval = ranges.back();

	if (rangeToUse == ranges.end())
	{
		bciwarn << paramName << " of " << signalRangeAskedFor << "V is not supported. Largest range is " << ranges.back() << "V is used.\n";
	}
	else
	{
		bciout << "Using " << paramName << " of " << *rangeToUse << "V.\n";
		retval = *rangeToUse;
	}

	return retval;
}

EEGO::Ranges EEGO::GetRanges(const eemagine::sdk::amplifier& amp) const
{
	// Find the most currect signal range to the ones specified

	Ranges retval;

	retval.reference = FindSignalRange(amp.getReferenceRangesAvailable(), "SourceRefVoltageRange");
	retval.bipolar = FindSignalRange(amp.getBipolarRangesAvailable(), "SourceBipVoltageRange");

	return retval;
}

eemagine::sdk::factory EEGO::GetFactory() const
{
	/// Should abstract the path finding routines etc. Currently not meaning full as only static binding is used.
	/// But if that changes we need to find a way to load a DLL in the app bundle.
	auto fac = eemagine::sdk::factory();
	return fac;
}
