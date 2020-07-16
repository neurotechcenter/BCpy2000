/////////////////////////////////////////////////////////////////////////////
// $Id: AudioExtension.cpp 5818 2018-11-09 15:50:15Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: The audio extension enables input and mixed output from/to 
//   sound cards on the machine. The extension also handles filtering of
//   audio input/output envelope to states
//
// Documentation located at: 
//   http://www.bci2000.org/wiki/index.php/Contributions:AudioExtension
//
// $BEGIN_BCI2000_LICENSE$
// $END_BCI2000_LICENSE$
/////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include <cstring>
#include "AudioExtension.h"
#include "FileUtils.h"
#include "BCIEvent.h"
#include "PrecisionTime.h"
#include "FilterDesign.h"
#include "Rectify.h"
#include "RootMeanSquare.h"

#include <iomanip>
#include <iostream>
#include <queue>
#include <cmath>
#include <set>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

Extension( AudioExtension );

bool pa_errcheck( PaError errCode );
bool parseLabel( string &label, string &type, int &data );

// Host API Names (Portaudio should really have this...)
int SIZE_HOST_APIS = 15;
string HOST_APIS[] =
{
  "Disabled",       // 0
  "DirectSound",    // 1
  "MME",            // 2
  "ASIO",           // 3
  "SoundManager",   // 4
  "CoreAudio",      // 5
  "Disabled",       // 6
  "OSS",            // 7
  "ALSA",           // 8
  "AL",             // 9
  "BeOS",           // 10
  "WDMKS",          // 11
  "JACK",           // 12
  "WASAPI",         // 13
  "AudioScienceHPI" // 14
};

// Audio Defaults
#define SAMPLE_RATE 44100

// Channel Types
#define CHANNELTYPE_INPUT "INPUT" // Input Channel: INPUT[1] is Live (Mic) Channel 1
#define CHANNELTYPE_FILE  "FILE" // File Channel: from AudioInputFile.  FILE[1] is File Channel 1
#define CHANNELTYPE_TONE  "TONE" // Tone Generator: Sine wave at TONE[X] Hz where X is a positive integer.
#define CHANNELTYPE_NOISE "NOISE" // White noise at SAMPLE_RATE.  NOISE[X] generates noise at X Hz
// #define CHANNELTYPE_SQUARE "SQUARE" // Feel free to add your own!

enum RecordingFormat
{
  RAW = 0,
  LOSSLESS = 1,
  LOSSY = 2
};

typedef Ratpoly< FilterDesign::Complex > TransferFunction;

// **************************************************************************
// Function:   AudioExtension
// Purpose:    This is the constructor for the AudioExtension class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
AudioExtension::AudioExtension() :
  mpAudioStream( NULL ),
  mpAudioInputFile( NULL ),
  mpAudioRecInputFile( NULL ),
  mpAudioRecOutputFile( NULL )
{
  // Initialize Portaudio
  pa_errcheck( Pa_Initialize() );
}

// **************************************************************************
// Function:   ~AudioExtension
// Purpose:    This is the destructor for the AudioExtension class
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
AudioExtension::~AudioExtension()
{
  // End the audio stream if its running
  KillStream();
  
  // Close any open sound file streams
  if( mpAudioInputFile ) sf_close( mpAudioInputFile );
  if( mpAudioRecInputFile ) sf_close( mpAudioRecInputFile );
  if( mpAudioRecOutputFile ) sf_close( mpAudioRecOutputFile );
  
  // Terminate Portaudio
  pa_errcheck( Pa_Terminate() );
}

// **************************************************************************
// Function:   Publish
// Purpose:    This function requests parameters by adding parameters to the
//             parameter list it also requests states by adding them to the
//             state list
// Returns:    N/A
// **************************************************************************
void AudioExtension::Publish()
{
  int hostApi = OptionalParameter( "EnableAudioExtension", 0 );
  if( hostApi == 6 ) hostApi = 0;
  if( !hostApi ) return;
	else bciwarn << "Audio Extension Enabled" << endl;

	stringstream publishInfoStrm;
  
  // Let the user know what hostapis are supported
  PaHostApiIndex numApis = Pa_GetHostApiCount();
	publishInfoStrm << "PortAudio sees the following HostAPIs" << endl;
  for( int i = 0; i < numApis; i++ )
  {
		publishInfoStrm << "    " << Pa_GetHostApiInfo(i)->name
										<< ": Supports " << Pa_GetHostApiInfo( i )->deviceCount 
										<< " devices." << endl;
    mSupportedAPIs.insert( Pa_GetHostApiInfo( i )->type );
  }
  
  mMaxInputEnvelopes = OptionalParameter( "MaxInputEnvelopes", 4 );
  mMaxOutputEnvelopes = OptionalParameter( "MaxOutputEnvelopes", 4 );
  mFramesPerBuffer = OptionalParameter( "AudioBufferSize", 1024 );
  
  stringstream hapis;
  hapis << "Source:AudioExtension int EnableAudioExtension= 0 0 0 " << SIZE_HOST_APIS - 1
        << " // Enable the AudioExtension with specific host API: ";
  for( int i = 0; i < SIZE_HOST_APIS; i++ )
  {
    bool found = mSupportedAPIs.find( i ) != mSupportedAPIs.end();
    hapis << i << ": " << ( found ? HOST_APIS[i] : "Disabled" ) << ", "; 
  }
  hapis << " (enumeration)";
  string enableDef = hapis.str();
  
  // Define these two procedural parameters
  BEGIN_PARAMETER_DEFINITIONS
    enableDef.c_str(),
  END_PARAMETER_DEFINITIONS
  
  // Define remaining parameters
  BEGIN_PARAMETER_DEFINITIONS
    "Source:AudioExtension int AudioInputDevice= -1 -1 -1 %"
    " // Device to use as input device (-1 = Default) ",
    
    "Source:AudioExtension int AudioOutputDevice= -1 -1 -1 %"
    " // Device to use as output device (-1 = Default) ",
    
    "Source:AudioExtension string AudioInputFile= %"
    " // Name of an input file to use (appended to input channels) (inputfile)",
    
    "Source:AudioExtension int AudioRecordInput= 0 0 0 1"
    " // Record mixer inputs to a file (boolean)",
    
    "Source:AudioExtension string AudioRecordOutput= %"
    " // Record mixer outputs to a file (boolean)",
    
    "Source:AudioExtension matrix AudioMixer= 0 2 % % %"
    " // Expressions mix column output from row input",
    
    "Source:AudioExtension int AudioRecordingFormat= 0 0 0 2"
    " // Change compression settings on audio output files:"
    "    0: Raw, 1: Lossless, 2: Lossy, (enumeration)",
    
    /*
    "Source:AudioExtension matrix AudioEnvelopes= "
    "{Source Filterbank State} "
    "0 "
    " % % %"
    " // Define audio envelope states and associated filters",
    */
    
    "Source:AudioExtension matrix AudioInputFilterbank= "
    "0 "
    "{Type Order Cutoff1 Cutoff2} "
    "% % %"
    " // Filterbank for audio input",
    
    "Source:AudioExtension matrix AudioOutputFilterbank= 0 {Type Order Cutoff1 Cutoff2} % % %"
    " // Filterbank for audio output",
    
    "Source:AudioExtension float AudioEnvelopeSmoothing= 20Hz 20Hz 0Hz 22000Hz"
    " // Low pass cutoff frequency for envelope",
  END_PARAMETER_DEFINITIONS
  
  // Define Input Envelopes
  for( size_t i = 0; i < mMaxInputEnvelopes; i++ )
  {
    stringstream eventDef;
    eventDef << "AudioInEnvelope" << i + 1 << " 16 0 0 0";
    string eventString = eventDef.str();
    BEGIN_EVENT_DEFINITIONS
      eventString.c_str(),
    END_EVENT_DEFINITIONS
  }
  
  // Define Output Envelopes
  for( size_t i = 0; i < mMaxOutputEnvelopes; i++ )
  {
    stringstream eventDef;
    eventDef << "AudioOutEnvelope" << i + 1 << " 16 0 0 0";
    string eventString = eventDef.str();
    BEGIN_EVENT_DEFINITIONS
      eventString.c_str(),
    END_EVENT_DEFINITIONS
  }

  // Define remaining events
  BEGIN_EVENT_DEFINITIONS
    "AudioFrame 32 0 0 0", // This rolls over once every 27 hours
  END_EVENT_DEFINITIONS
  
  // Let the user know that the AudioExtension initialized correctly.
	publishInfoStrm << "AudioExtension enabled using " << Pa_GetVersionText()  << endl;

	bciout << publishInfoStrm.str();
}

// **************************************************************************
// Function:   Preflight
// Purpose:    Checks parameters for availability and consistency with
//             input signal properties; requests minimally needed properties for
//             the output signal; checks whether resources are available.
// Parameters: Input and output signal properties.
// Returns:    N/A
// **************************************************************************
void AudioExtension::Preflight() const
{
  int hostApi = OptionalParameter( "EnableAudioExtension" );
  if( hostApi == 6 ) hostApi = 0;
  if( mSupportedAPIs.find( hostApi ) == mSupportedAPIs.end() ) hostApi = 0;
  if( !hostApi ) return;
  
  // Query existence of required states
  State( "Running" );
  
  for( size_t i = 0; i < mMaxInputEnvelopes; i++ )
  {
    stringstream ss;
    ss << "AudioInEnvelope" << i + 1;
    State( ss.str().c_str() ) = 0;
  }
  
  for( size_t i = 0; i < mMaxOutputEnvelopes; i++ )
  {
    stringstream ss;
    ss << "AudioOutEnvelope" << i + 1;
    State( ss.str().c_str() ) = 0;
  }
  
  // Check for existence of required parameters
  Parameter( "AudioEnvelopeSmoothing" );
  Parameter( "AudioRecordInput" );
  Parameter( "AudioRecordOutput" );
  Parameter( "AudioRecordingFormat" );
  
  // Retreive ifnormation for the requested HostAPI
  const PaHostApiInfo* apiInfo;
  PaHostApiIndex hostIdx;
  if( !GetHostApiConfig( PaHostApiTypeId( ( int )Parameter( "EnableAudioExtension" ) ), apiInfo, hostIdx ) )
    return;
  
	stringstream preflightInfoStrm;

  // List the devices on this api
	preflightInfoStrm << apiInfo->name << " was requested as the host API" << endl;
	preflightInfoStrm << "HostAPI: " << apiInfo->name << " supports the following "
                    << apiInfo->deviceCount << " devices." << endl;
  for( int i = 0; i < apiInfo->deviceCount; i++ )
  {
    const PaDeviceInfo* info = Pa_GetDeviceInfo( Pa_HostApiDeviceIndexToDeviceIndex( hostIdx, i ) );
		preflightInfoStrm << "    Device " << i << ": " << info->name << " Supports " << info->maxInputChannels
                      << " input channels and " << info->maxOutputChannels << " output channels." << endl;
  }
  
  // Attempt to setup requested device configuration
  int inputChannels = 0, outputChannels = 0;
  int inDev = Parameter( "AudioInputDevice" );
  int outDev = Parameter( "AudioOutputDevice" );
  GetDeviceConfig( apiInfo, hostIdx, inDev, outDev, inputChannels, outputChannels );
	preflightInfoStrm << "Device configuration supports " << inputChannels
                    << " input channels and " << outputChannels << " output channels." << endl;

	bciout << preflightInfoStrm.str();
  
  // Determine number of file input channels with libsndfile
  string audioInputFilename = Parameter( "AudioInputFile" );
  SNDFILE* audioInputFile = NULL;
  SF_INFO audioInputInfo;
  if( audioInputFilename != "" )
    if( !( audioInputFile = sf_open( audioInputFilename.c_str(), SFM_READ, &audioInputInfo ) ) )
      bcierr << "Error opening AudioInputFile: " << audioInputFilename << endl
             << "libsndfile error: "<< sf_strerror( NULL ) << endl;
  
  // Error check the samplerate on the input audio file
  if( audioInputFile && audioInputInfo.samplerate != SAMPLE_RATE )
  {
    bcierr << "AudioInputFile is encoded at " << audioInputInfo.samplerate 
           << " samples per second, but it should be encoded at " << SAMPLE_RATE
           << " samples per second in order to be used with AudioExtension" << endl;
    sf_close( audioInputFile );
    audioInputFile = NULL;
  }
  
  // Determine the number of channels
  int audioInputFileChannels = audioInputFile ? audioInputInfo.channels : 0;
  
  // Load the mixer parameter
  ExpressionMatrix preflightMixer;
  ChannelDef channelDef;
  LoadMixer( Parameter( "AudioMixer" ), preflightMixer, channelDef,
    inputChannels, outputChannels, audioInputFileChannels );
  vector< vector< float > > gainMatrix;
  EvaluateMixer( preflightMixer, gainMatrix );
  
  // Attempt to set up filters
  IIRFilter< FilterDesign::Real > preflightInputFilter, preflightOutputFilter;
  DesignFilterbank( Parameter( "AudioInputFilterbank" ), preflightInputFilter, channelDef.size() );
  DesignFilterbank( Parameter( "AudioOutputFilterbank" ), preflightOutputFilter, outputChannels );
}

// **************************************************************************
// Function:   Initialize
// Purpose:    This function parameterizes the AudioExtension
// Parameters: Input and output signal properties.
// Returns:    N/A
// **************************************************************************
void AudioExtension::Initialize()
{
  mHostAPI = OptionalParameter( "EnableAudioExtension" );
  if( mHostAPI == 6 ) mHostAPI = 0; // Disable on API 6
  if( mSupportedAPIs.find( mHostAPI ) == mSupportedAPIs.end() ) mHostAPI = 0;
  if( !mHostAPI ) return;
  
  // If there is a stream already running, we need to stop and close it.
  if( KillStream() ) return;

  // Retreive information for the requested HostAPI
  const PaHostApiInfo* apiInfo;
  PaHostApiIndex hostIdx;
  if( !GetHostApiConfig( ( PaHostApiTypeId )mHostAPI, apiInfo, hostIdx ) ) return;
  
  // Attempt to setup requested device configuration
  int inDev = Parameter( "AudioInputDevice" );
  int outDev = Parameter( "AudioOutputDevice" );
  GetDeviceConfig( apiInfo, hostIdx, inDev, outDev, mInputChannels, mOutputChannels );
  
  // Init LibSndFile for decoding audio input
  if( mpAudioInputFile ) sf_close( mpAudioInputFile );
  mpAudioInputFile = NULL;
  string audioInputFilename = Parameter( "AudioInputFile" );
  SF_INFO audioInputInfo;
  if( audioInputFilename != "" ) mpAudioInputFile = sf_open( audioInputFilename.c_str(), SFM_READ, &audioInputInfo );
  mFileChannels = mpAudioInputFile ? audioInputInfo.channels : 0;
  
  // Load the mixer parameter
  LoadMixer( Parameter( "AudioMixer" ), mMixer, mChannelDef,
    mInputChannels, mOutputChannels, mFileChannels );
  EvaluateMixer( mMixer, mGainMatrix );
  
  // Set up filters
  DesignFilterbank( Parameter( "AudioInputFilterbank" ), mInputFilter, mChannelDef.size() );
  DesignFilterbank( Parameter( "AudioOutputFilterbank" ), mOutputFilter, mOutputChannels );
  
  // Setup the internal audio signals
  SignalProperties audioInputProperties;
  audioInputProperties.SetName( "InputAudioStream" );
  audioInputProperties.SetChannels( Parameter( "AudioMixer" )->NumRows() );
  audioInputProperties.SetElements( mFramesPerBuffer );
  audioInputProperties.SetUpdateRate( SAMPLE_RATE / mFramesPerBuffer );
  audioInputProperties.SetType( SignalType::float32 );
  audioInputProperties.SetIsStream( true );
  mAudioInputBlock.SetProperties( audioInputProperties );
  mAudioInputEnvelope.SetProperties( audioInputProperties );
  
  SignalProperties audioOutputProperties;
  audioOutputProperties.SetName( "OutputAudioStream" );
  audioOutputProperties.SetChannels( Parameter( "AudioMixer" )->NumColumns() );
  audioOutputProperties.SetElements( mFramesPerBuffer );
  audioOutputProperties.SetUpdateRate( SAMPLE_RATE / mFramesPerBuffer );
  audioOutputProperties.SetType( SignalType::float32 );
  audioOutputProperties.SetIsStream( true );
  mAudioOutputBlock.SetProperties( audioOutputProperties );
  mAudioOutputEnvelope.SetProperties( audioOutputProperties );
  
  // Prepare the envelope filters
  double frequency = Parameter( "AudioEnvelopeSmoothing" ).InHertz();
  TransferFunction tf = FilterDesign::Butterworth()
    .Order( 2 )
    .Lowpass( frequency / ( float )SAMPLE_RATE )
    .TransferFunction();
  FilterDesign::Real tfGain = 1.0f / abs( tf.Evaluate( 1.0 ) );
  
  mInputEnvelopeFilter.clear();
  int numInputEnvelopes = min( mAudioInputBlock.Channels(), ( int )mMaxInputEnvelopes );
  for( int i = 0; i < numInputEnvelopes; i++ )
    mInputEnvelopeFilter.push_back( IIRFilter< FilterDesign::Real >()
      .SetGain( tfGain )
      .SetZeros( tf.Numerator().Roots() )
      .SetPoles( tf.Denominator().Roots() )
      .Initialize( 1 ) );
    
  mOutputEnvelopeFilter.clear();
  int numOutputEnvelopes = min( mAudioOutputBlock.Channels(), ( int )mMaxOutputEnvelopes );
  for( int i = 0; i < numOutputEnvelopes; i++ )
    mOutputEnvelopeFilter.push_back( IIRFilter< FilterDesign::Real >()
      .SetGain( tfGain )
      .SetZeros( tf.Numerator().Roots() )
      .SetPoles( tf.Denominator().Roots() )
      .Initialize( 1 ) );
  
  // Create the stream
  PaStreamParameters* inputParameters = NULL;
  if( mInputChannels > 0 )
  {
    inputParameters = new PaStreamParameters();
    inputParameters->device = inDev;
    inputParameters->channelCount = mInputChannels;
    inputParameters->sampleFormat = paFloat32;
    inputParameters->suggestedLatency = Pa_GetDeviceInfo( inputParameters->device )->defaultLowInputLatency;
    inputParameters->hostApiSpecificStreamInfo = NULL;
  }

  PaStreamParameters* outputParameters = NULL;
  if( mOutputChannels > 0 )
  {
    outputParameters = new PaStreamParameters();
    outputParameters->device = outDev;
    outputParameters->channelCount = mOutputChannels;
    outputParameters->sampleFormat = paFloat32;
    outputParameters->suggestedLatency = Pa_GetDeviceInfo( outputParameters->device )->defaultLowOutputLatency;
    outputParameters->hostApiSpecificStreamInfo = NULL;
  }
  
  // Open an output stream and delete temporary variables
  bool streamOpen = !pa_errcheck( Pa_OpenStream( &mpAudioStream, inputParameters, outputParameters,
    SAMPLE_RATE, mFramesPerBuffer, NULL, AudioExtension::AudioCallback, this ) );
  delete inputParameters;
  delete outputParameters;
  
  // Start the stream
  if( !streamOpen ) bcierr << "Could not open audio stream." << endl;
  
  // Prepare and start the audio thread
  mBufferProcessed = true;
  mFrameCount = 0;
  mRand.SetSeed( ( RandomGenerator::SeedType )PrecisionTime::Now() );
}

// **************************************************************************
// Function:   Execute
// Purpose:    This function deals with audio mixing in a separate thread
//             in order to disassociate the sample block latency from the
//             audio latency
// Parameters: N/A
// Returns:    0
// **************************************************************************
int AudioExtension::OnExecute()
{
  // Create buffers for sound file input and output
  float* fileInputBuf = mpAudioInputFile ? new float[ mFramesPerBuffer * mFileChannels ] : NULL;
  float* fileRecInBuf = mpAudioRecInputFile ? new float[ mFramesPerBuffer * mChannelDef.size() ] : NULL;
  float* fileRecOutBuf = mpAudioRecOutputFile ? new float[ mFramesPerBuffer * mOutputChannels ] : NULL;
  
  while( !Terminating() )
  {
    if( mBufferProcessed ) ThreadUtils::SleepForMs( 1 );
    else 
    {
      // Gather file input and add to the input block
      if( mpAudioInputFile )
      {
        sf_count_t readSize = sf_readf_float( mpAudioInputFile, fileInputBuf, mFramesPerBuffer );
        for( size_t row = 0; row < mChannelDef.size(); row++ )
          if( mChannelDef[ row ].first == CHANNELTYPE_FILE )
            for( size_t idx = mChannelDef[ row ].second; idx < mFramesPerBuffer * mFileChannels; idx += mFileChannels )
              mAudioInputBlock.SetValue( row, idx / mFileChannels, readSize ? fileInputBuf[ idx ] : 0.0f );
      }
      
      // Synthesize new input and add it to the input block
      for( size_t i = 0; i < mChannelDef.size(); i++ )
      {
        int frequency = mChannelDef[i].second;
        if( mChannelDef[i].first == CHANNELTYPE_TONE )
          for( size_t frame = 0; frame < mFramesPerBuffer; frame++ )
            mAudioInputBlock.SetValue( i, frame, sin( 2 * M_PI * frequency * ( ( mFrameCount + frame ) / ( float )SAMPLE_RATE ) ) );

        if( mChannelDef[i].first == CHANNELTYPE_NOISE )
        {
          if( frequency == 0 ) frequency = SAMPLE_RATE;
          int period = frequency = SAMPLE_RATE / frequency;
          if( period < 1 ) period = 1;
          float val = ( float )mAudioInputBlock.Value( i, mAudioInputBlock.Elements() - 1 );
          for( size_t frame = 0; frame < mFramesPerBuffer; frame++ )
          {
            if( ( mFrameCount + frame ) % period == 0 )
              val = ( float )mRand.Random() / ( float )mRand.RandMax();
            mAudioInputBlock.SetValue( i, frame, val );
          }
        }
      }
      
      // Record input block to sound file if necessary
      if( mpAudioRecInputFile )
      {
        int idx = 0;
        for( size_t frame = 0; frame < mFramesPerBuffer; frame++ )
          for( size_t chan = 0; chan < mChannelDef.size(); chan++ )
            fileRecInBuf[ idx++ ] = ( float )mAudioInputBlock( chan, frame );
        sf_writef_float( mpAudioRecInputFile, fileRecInBuf, mFramesPerBuffer );
      }
      
      // Extract input envelopes and push to events
      mAudioInputEnvelope = mAudioInputBlock;
      mInputFilter.Process( mAudioInputEnvelope, mAudioInputEnvelope );
      for( size_t chan = 0; chan < mInputEnvelopeFilter.size(); chan++ )
      {
        GenericChannel channelData( mAudioInputEnvelope, static_cast<int>( chan ) );
        Rectify::FullWaveRectify( channelData, channelData );
        mInputEnvelopeFilter[chan].Process( channelData, channelData );
        unsigned short value = static_cast< unsigned short >( RootMeanSquare( channelData ) * 65535.0f );
        bcievent << "AudioInEnvelope" << chan + 1 << " " << value;
      }

      // Mix input to output block
      for( int outChan = 0; outChan < mAudioOutputBlock.Channels(); outChan++ )
      {
        for( size_t frame = 0; frame < mFramesPerBuffer; frame++ )
          mAudioOutputBlock.SetValue( outChan, frame, 0.0f );
        for( size_t inChan = 0; inChan < mMixer.size(); inChan++ )
        {
          float gain = mGainMatrix[ inChan ][ outChan ];
          for( size_t frame = 0; frame < mFramesPerBuffer; frame++ )
          {
            float newValue = ( float )mAudioOutputBlock( outChan, frame );
            newValue = newValue + ( gain * ( float )mAudioInputBlock( inChan, frame ) );
            mAudioOutputBlock.SetValue( outChan, frame, newValue );
          }
        }
      }

      // Filter and extract output envelopes and push to events
      mAudioOutputEnvelope = mAudioOutputBlock;
      mOutputFilter.Process( mAudioOutputEnvelope, mAudioOutputEnvelope );
      for( size_t chan = 0; chan < mOutputEnvelopeFilter.size(); chan++ )
      {
        GenericChannel channelData( mAudioOutputEnvelope, static_cast<int>( chan ) );
        Rectify::FullWaveRectify( channelData, channelData );
        mOutputEnvelopeFilter[chan].Process( channelData, channelData );
        unsigned short value = static_cast< unsigned short >( RootMeanSquare( channelData ) * 65535.0f );
        bcievent << "AudioOutEnvelope" << chan + 1 << " " << value;
      }
      
      // Record output block to sound file if necessary
      if( mpAudioRecOutputFile )
      {
        int idx = 0;
        for( size_t frame = 0; frame < mFramesPerBuffer; frame++ )
          for( int chan = 0; chan < mOutputChannels; chan++ )
            fileRecOutBuf[ idx++ ] = ( float )mAudioOutputBlock( chan, frame );
        sf_writef_float( mpAudioRecOutputFile, fileRecOutBuf, mFramesPerBuffer );
      }
      
      // Push the new sample count to an event
      mFrameCount += mFramesPerBuffer;
      bcievent << "AudioFrame " << mFrameCount ;
      
      // Mark this buffer as processed
      mBufferProcessed = true;
    }
  }
  
  // Cleanup allocated file I/O buffers
  delete [] fileInputBuf;
  delete [] fileRecInBuf;
  delete [] fileRecOutBuf;
  
  return 0;
}

// **************************************************************************
// Function:   Process
// Purpose:    
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void AudioExtension::Process()
{
  if( !mHostAPI ) return;
 
  // Evaluate the Mixer and create the gain matrix for this block
  EvaluateMixer( mMixer, mGainMatrix );
}

// **************************************************************************
// Function:   StartRun
// Purpose:    
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void AudioExtension::StartRun()
{
  if( !mHostAPI ) return;
  
  // Determine Output Reording Format and filenames
  int recordingFormat = 0;
  string extension;    
  switch( ( int )Parameter( "AudioRecordingFormat" ) )
  {
    case LOSSLESS:
      recordingFormat = SF_FORMAT_FLAC | SF_FORMAT_PCM_16;
      extension = ".flac";
      break;
    case LOSSY:
      recordingFormat = SF_FORMAT_OGG | SF_FORMAT_VORBIS;
      extension = ".ogg";
      break;
    default:
    case RAW:
      recordingFormat = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
      extension = ".wav";
      break;
  }
  
  // Setup Input Recording
  SF_INFO audioRecInputInfo;
  audioRecInputInfo.channels = static_cast<int>( mChannelDef.size() );
  audioRecInputInfo.format = recordingFormat;
  audioRecInputInfo.samplerate = SAMPLE_RATE;
  if( ( int )Parameter( "AudioRecordInput" ) && mChannelDef.size() )
  {
    string file = FileUtils::StripExtension( CurrentRun() ) + "_in" + extension; 
    mpAudioRecInputFile = sf_open( 
      file.c_str(),
      SFM_WRITE, &audioRecInputInfo );
  }
  
  // Setup Output Recording
  SF_INFO audioRecOutputInfo;
  audioRecOutputInfo.channels = mOutputChannels;
  audioRecOutputInfo.format = recordingFormat;
  audioRecOutputInfo.samplerate = SAMPLE_RATE;
  if( ( int )Parameter( "AudioRecordOutput" ) && mOutputChannels ) 
  {
    string file = FileUtils::StripExtension( CurrentRun() ) + "_out" + extension;
    mpAudioRecOutputFile = sf_open(
      file.c_str(),
      SFM_WRITE, &audioRecOutputInfo );
  }
  
  // Start Audio I/O Stream and processing
  if( mpAudioInputFile ) sf_seek( mpAudioInputFile, 0, SEEK_SET );
  if( !pa_errcheck( Pa_StartStream( mpAudioStream ) ) ) Start();
}

// **************************************************************************
// Function:   StopRun
// Purpose:    
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void AudioExtension::StopRun()
{
  if( !mHostAPI ) return;
  
  // Kill the audio processing thread and stop the stream
  Terminate();
  pa_errcheck( Pa_StopStream( mpAudioStream ) );
  while( pa_errcheck( Pa_IsStreamActive( mpAudioStream ) ) ) Pa_Sleep( 10 );
  
  // Close the output audio files if necessary
  if( mpAudioRecInputFile ) sf_close( mpAudioRecInputFile );
  mpAudioRecInputFile = NULL;
  if( mpAudioRecOutputFile ) sf_close( mpAudioRecOutputFile );
  mpAudioRecOutputFile = NULL;
}

// **************************************************************************
// Function:   AudioCallback
// Purpose:    Recieve audio input data and send mixed output data
// Parameters: input, output, number of frames per buffer, internal clock,
//             stream status, and a pointer to the object
// Returns:    True if Audio Host API Information was retreived correctly
// **************************************************************************
int AudioExtension::AudioCallback( const void *inputBuffer, void *outputBuffer,
  unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
  PaStreamCallbackFlags statusFlags, void *userData )
{
  AudioExtension* audioExtension = ( AudioExtension* )userData;
  
  // Push audio input data into the input buffer block
  float* in = ( float* )inputBuffer;
  int inputChannels = audioExtension->mInputChannels;
  for( size_t row = 0; row < audioExtension->mChannelDef.size(); row++ )
    if( audioExtension->mChannelDef[ row ].first == CHANNELTYPE_INPUT )
      for( size_t idx = audioExtension->mChannelDef[ row ].second; 
           idx < audioExtension->mFramesPerBuffer * inputChannels;
           idx += inputChannels )
        audioExtension->mAudioInputBlock.SetValue( row, idx / inputChannels, in[ idx ] );
  
  // Notify the AudioExtension thread that new data has been posted
  audioExtension->mBufferProcessed = false;
  
  // Populate output stream from the audio output buffer block
  int idx = 0;
  float* out = ( float* )outputBuffer;
  for( int frame = 0; frame < audioExtension->mAudioOutputBlock.Elements(); frame++ )
    for( int chan = 0; chan < audioExtension->mAudioOutputBlock.Channels(); chan++ )
      out[ idx++ ] = ( float )audioExtension->mAudioOutputBlock( chan, frame );
  
  return 0;
}

// **************************************************************************
// Function:   GetHostApiConfig
// Purpose:    Attempt to retreive information about the requested Host API 
//             and perform error checking
// Parameters: input hostApi: A PaHostApiTypeId for the requested HostAPI
//             output apiInfo*: A pointer to const Host API information
//             output hostIdx: An index for the Host API
// Returns:    True if Audio Host API Information was retreived correctly
// **************************************************************************
bool AudioExtension::GetHostApiConfig( PaHostApiTypeId hostApi, const PaHostApiInfo* &apiInfo,
                                       PaHostApiIndex &hostIdx ) const
{
  // Initialize PortAudio with the default device on the requested host api
  hostIdx = Pa_HostApiTypeIdToHostApiIndex( PaHostApiTypeId( hostApi ) );
  if( hostIdx < 0 && pa_errcheck( hostIdx ) ) return false;
  apiInfo = Pa_GetHostApiInfo( hostIdx );
  if( !apiInfo )
  { 
    bcierr << "Error occurred retrieving requested Host API: " << HOST_APIS[ hostApi ] << endl;
    const PaHostErrorInfo* hostError = Pa_GetLastHostErrorInfo();
    if( hostError->errorCode != 0 )
      bcierr << "PortAudio Host API Error " 
             << hostError->errorCode << ": " 
             << hostError->errorText << endl;
    return false;
  }
  
  return true;
}

// **************************************************************************
// Function:   GetDeviceConfig
// Purpose:    Attempt to detect devices on requested host API and set up
//             audio I/O to requested 
// Parameters: input: const apiInfo*: from GetHostApiConfig
//             input/output inputIdx: An index for the input device on this host API
//             input/output outputIdx: Index for the output device on this host API
//             output inputChannels: The number of inputs on this config.
//             output outputChannels: The number of outputs on this config.
// Returns:    N/A
// **************************************************************************
void AudioExtension::GetDeviceConfig( const PaHostApiInfo* apiInfo, PaHostApiIndex hostIdx,
  int &inputIdx, int &outputIdx, int &inputChannels, int &outputChannels ) const
{
  // Get the input/output devices for the requested Host API
  PaDeviceIndex inputDevice = ( inputIdx == -1 ) ? 
    apiInfo->defaultInputDevice :
    Pa_HostApiDeviceIndexToDeviceIndex( hostIdx, inputIdx );
  PaDeviceIndex outputDevice = ( outputIdx == -1 ) ? 
    apiInfo->defaultOutputDevice :
    Pa_HostApiDeviceIndexToDeviceIndex( hostIdx, outputIdx );
  const PaDeviceInfo* inDevInfo = Pa_GetDeviceInfo( inputDevice );
  const PaDeviceInfo* outDevInfo = Pa_GetDeviceInfo( outputDevice );
  
  // Perform error checking
  if( !inDevInfo )
  {
    if( inputIdx == -1 )
      bcierr << "Error retrieving default input device on requested Host API" << endl;
    else
      bcierr << "Error retrieving input device " << inputIdx
                  << " on requested Host API; are you sure it's valid?" << endl;
  }
  if( !outDevInfo )
  {
    if( outputIdx == -1 )
      bcierr << "Error retrieving default output device on requested Host API" << endl;
    else
      bcierr << "Error retrieving output device " << outputIdx
                  << " on requested Host API; are you sure it's valid?" << endl;
  }
  
  // Determine the number of inputs and outputs this combination supports
  inputChannels = ( inDevInfo ) ? inDevInfo->maxInputChannels : 0;
  outputChannels = ( outDevInfo ) ? outDevInfo->maxOutputChannels : 0;
  inputIdx = inputDevice;
  outputIdx = outputDevice;
}

// **************************************************************************
// Function:   LoadMixer
// Purpose:    Load the mixer expression matrix and perform error checking 
//             on the input.
// Parameters: matrix: The mixer parameter
//             mixer: A Matrix of expressions which corresponds to the mixer
//             audioIns: The number of detected audio input channels
//             audioOuts: The number of detected audio output channels
//             fileIns: The number of detected input audio file channels
// Returns:    N/A
// **************************************************************************
void AudioExtension::LoadMixer( const ParamRef &matrix, ExpressionMatrix &mixer,
  ChannelDef &channelDef, int audioIns, int audioOuts, int fileIns ) const
{
  // Reset the mixer and channel maps
  mixer.clear();
  channelDef.clear();
  
  // Sanity check
  int numRows = matrix->NumRows();
  int numCols = matrix->NumColumns();
  if( numCols != audioOuts ) 
    bcierr << matrix->Name() << " should have one column per output.  There are " 
           << audioOuts << " outputs." << endl;
           
  // Populate the channelmaps
  for( int r = 0; r < numRows; r++ )
  {
    int data = 0;
    string label = matrix->RowLabels()[r], type;
    if( !parseLabel( label, type, data ) ) continue;
    
    // Handle input channels (error checking and zero-indexing)
    if( type == CHANNELTYPE_INPUT || type == CHANNELTYPE_FILE )
    {
      data = data - 1;
      int numIns = ( type == CHANNELTYPE_INPUT ) ? audioIns : fileIns;
      if( data >= numIns || data < 0 )
        bcierr << "Requested " << type << " channel " << data + 1 << " on row " 
               << r + 1 << " of " <<  matrix->Name() << " is out of bounds." << endl
               << "There are " << numIns << " " << type << " channels available." << endl;
    } 
    
    channelDef.push_back( pair< string, int >( type, data ) );
    
    // Validate the expressions and populate the mixer
    mixer.push_back( ExpressionVector() );
    for( int c = 0; c < numCols; c++ ) 
    {
      Expression cell( matrix( r, c ) );
      mixer[r].push_back( cell );
      mixer[r][c].Compile();
    }
  }
}

// **************************************************************************
// Function:   EvaluateMixer
// Purpose:    Stop and close the audio stream/thread if it is running
//             on the input.
// Parameters: N/A
// Returns:    true if error occurred, false if not.
// **************************************************************************
void AudioExtension::EvaluateMixer( ExpressionMatrix &mixer, vector< vector< float > > &gainMatrix ) const
{
  // Calculate the new gain matrix
  if( gainMatrix.size() != mixer.size() ) gainMatrix.resize( mixer.size() );
  for( size_t inChan = 0; inChan < mixer.size(); inChan++ )
  {
    if( gainMatrix[ inChan ].size() != mixer[ inChan ].size() ) 
      gainMatrix[ inChan ].resize( mixer[ inChan ].size() );
    for( size_t outChan = 0; outChan < mixer[ inChan ].size(); outChan++ )
      gainMatrix[ inChan ][ outChan ] = ( float )mixer[ inChan ][ outChan ].Evaluate();
  }
}

// **************************************************************************
// Function:   KillStream
// Purpose:    Stop and close the audio stream/thread if it is running
//             on the input.
// Parameters: N/A
// Returns:    true if error occurred, false if not.
// **************************************************************************
bool AudioExtension::KillStream()
{
  // If we have a stream currently running, we need to kill it
  Terminate();
    
  // Stop the stream and wait for it to be stopped
  if( Pa_IsStreamActive( mpAudioStream ) )
  {
    if( pa_errcheck( Pa_StopStream( mpAudioStream ) ) ) return true;
    while( !Pa_IsStreamStopped( mpAudioStream ) ) Pa_Sleep( 10 );
  }

  if( pa_errcheck( Pa_CloseStream( mpAudioStream ) ) ) return true;
  
  return false;
}

// **************************************************************************
// Function:   PopulateFilterbank
// Purpose:    Load a filterbank from a parameter matrix
// Parameters: N/A
// Returns:    true if error occurred, false if not.
// **************************************************************************
void AudioExtension::DesignFilterbank( const ParamRef &matrix, IIRFilter< FilterDesign::Real > &filter, size_t numCh ) const
{
  if( matrix->NumColumns() != 4 )
    bcierr << "Filterbank Matrix \"" << matrix->Name() 
           << "\" should have 4 columns." << endl; 
  else
  {
    TransferFunction transferFunction( 1.0 );
    FilterDesign::Real gain = 1.0f;
  
    for( int row = 0; row < matrix->NumRows(); row++ )
    {
      FilterDesign::Butterworth filt;
      filt.Order( matrix( row, 1 ) );
      if( matrix( row, 0 ) == "Lowpass" )
        filt.Lowpass( matrix( row, 2 ) / ( float )SAMPLE_RATE );
      else if( matrix( row, 0 ) == "Highpass" )
        filt.Highpass( matrix( row, 2 ) / ( float )SAMPLE_RATE );
      else if( matrix( row, 0 ) == "Bandpass" )
        filt.Bandpass( matrix( row, 2 ) / ( float )SAMPLE_RATE,
                       matrix( row, 3 ) / ( float )SAMPLE_RATE );
      else if( matrix( row, 0 ) == "Bandstop" )
        filt.Bandstop( matrix( row, 2 ) / ( float )SAMPLE_RATE,
                       matrix( row, 3 ) / ( float )SAMPLE_RATE );
      else
        bcierr << "Unknown filter characteristic \"" << matrix( row, 0 ) 
               << "\" on row " << row << " of " << matrix->Name() << endl;
             
      transferFunction *= filt.TransferFunction();
      double eval = ( matrix( row, 0 ) == "Highpass" 
                   || matrix( row, 0 ) == "Bandpass" ) ? -1.0 : 1.0;
      gain /= abs( filt.TransferFunction().Evaluate( eval ) );
    }

    filter.SetGain( gain )
          .SetZeros( transferFunction.Numerator().Roots() )
          .SetPoles( transferFunction.Denominator().Roots() )
          .Initialize( numCh );
  }
}

// Portaudio error checking
bool pa_errcheck( PaError errCode )
{
  if ( errCode > paNoError ) {
    bcierr << "PortAudio error: " << Pa_GetErrorText( errCode ) << endl;
    return true;
  }
  return false;
}

// Determine if a string is an integer
bool isInteger( string str )
{
  for( size_t i = 0; i < str.size(); i++ )
    if( !isdigit( str[i] ) ) return false;
  return true;
}

// Parse out the custom input format labelling
bool parseBrackets( string &label, string &type, string &data )
{
  // Find indices
  size_t start = label.find_first_of( "[" );
  size_t end = label.find_first_of( "]" );
  
  // Error Checking
  if( start == label.npos || end == label.npos || start >= end ) 
  {
    bcierr << "Malformed label : " << label << "; Are you missing brackets? " << endl;
    return false;
  }
  
  // Parsing
  type = label.substr( 0, start );
  data = label.substr( start + 1, end - start - 1 );
  return true;
}

// Parse brackets, but with a special condition.  1, 2, and such can be used
// directly as INPUT channels
bool parseLabel( string &label, string &type, int &data )
{
  // Handle a special case of label parsing
  string sdata;
  if( isInteger( label ) ) 
  {
    type = CHANNELTYPE_INPUT;
    sdata = label;
  } 
  else if( !parseBrackets( label, type, sdata ) ) return false;
  istringstream( sdata ) >> data;
  data = abs( data );
  return true;
}
