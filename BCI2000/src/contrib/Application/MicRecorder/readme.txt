Microphone Recorder
======================

Author: Tim Blakely
        tim.blakely@gmail.com
  Date: 09/16/2010 (Initial Release)
 Files: ./MicRecorderFilter.cpp
        ./MicRecorderFilter.h
        ./MicRecorder.cpp
        ./MicRecorder.h
        ./readme.txt

Description:
------------
The microphone recording filter is implemented as a standalone filter, much like the 5DT dataglove and uses the Waveform Audio API present in nearly every version of Windows since 95.  Though this means that the filter is only usable in Windows, this new MicRecorder class could be updated to support Linux/Mac with minimal changes to the underlying code if they support some type of asynchronous recording.

Upon initialization, it enumerates all the possible sound recording sources present on the system which includes any type of microphone that Windows can detect as a mic (USB, line-in, Speaker Output, S/PDIF, etc) and selects the source based on a string parameter.  The recording frequency, bit depth, and number of channels for stereo or mono recording are all set by parameters.  To determine what formats are supported by which sources, debugging information can be enabled as a boolean parameter that will enumerate all supported recording combinations to bciout.  

Hardware buffer initialization is currently hardcoded to use a hardware sample block size of 1k with enough audio buffer blocks to support up to 1 second of buffer read delay should the need arise.  Empirical tests show that this is more than adequate for all recording combinations tested, from 8-bit mono 11.025kHz to 16 bit stereo 96kHz.  As such, I opted to "hide" this parameter from the end BCI2000 user, though it would be a simple matter to add a couple parameters to tweak these settings.

Once the BCI2000 data recording stream is activated (State("Running") switches to '1'), it opens the a destination WAV file with the same name as the currently recording DAT file.  The MicRecorderFilter then initiates the hardware recording in a separate thread handled by the Win32 API.  When the hardware buffer is filled, a hardware interrupt is fired and the MicRecorder callback is called which reads the audio buffer and writes it to a file, keeping track of the total number of samples that have been written to disk.  With the current audio buffer length of 1k, this bytecount is updated a little over 90 times a second when recording at 16-bit mono 48kHz, though it can be updated more frequently by lowering the buffer length to 512 or 256 bytes at the cost of more processing time.  

During recording, each sample block passed through BCI2000 updates a state variable called MicSample with the number of samples recorded.  This state acts as an index into the WAV file.  Because this state is updated asynchronously with the actual audio recording, it records the last update of the number of samples recorded which introduces a tiny bit of jitter into the indexing state.  However, the audio is still being recorded in the meantime, which means that while the dSamplesRecorded/dSampleBlock will vary slightly, the actual offset between the audio file and the brain signal should remain constant.

Vendor:
-------
N/A

Integration into BCI2000:
-------------------------
The MicRecorder is integrated as a filter (MicRecorder) into BCI2000. Simply add the MicRecorderFilter.h/.cpp and MicRecorder.h/cpp to a module and it will be configured into the filter chain (defaults to priority 3, changeable in MicRecorderFilter.h).

Parameterization:
-----------------
The microphone recorder is configured in the panel HumanInterfaceDevices in the section Microphone. 

RecordMic			= [checked]/unchecked
					- Enable/disable recording
MicName				= [Stereo] 
					- A partial, case-sensitive string that denotes what recording device you would like to record from
MicRecordingFre		= 11.025kHz/22.05kHz/44.1kHz/[48kHz]/96kHz
					- Sampling rate of microphone recording hardware
MicBitDepth			= 8-bit/[16-bit]
					- Recording bit depth of microphone
MicNumChannels		= [Mono]/Stereo
					- Number of channels from microphone to be recorded
MicDebugInfo		= checked/[unchecked]
					- Outputs detailed debug information about each microphone device including capable recording parameter combinations

State Variables:
----------------

The MicRecorderFilter adds a single state variable called "MicSample".  This state is updated with every sample block and acts as an index into the WAV file generated.  For example, if the state variable MicSample changes to 1500, the biosignal recording is synced with the 1500th sample in the WAV file 

