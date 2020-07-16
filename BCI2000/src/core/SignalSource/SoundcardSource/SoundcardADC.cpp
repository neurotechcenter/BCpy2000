////////////////////////////////////////////////////////////////////////////////
// $Id: $
// Authors:
// Description: SoundcardADC implementation
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
#include "SoundcardADC.h"
#include "AudioDevice.h"
#include "BCIStream.h"

using namespace std;

RegisterFilter( SoundcardADC, 1 );

struct SoundcardADC::Data
{
  Data( SoundcardADC* );
  ~Data();
  void StreamCallback();
  void ScanForDevices();

  SoundcardADC* mpSelf;
  AudioStream* mpStream;
  float* mpData;
  MemberCall<void(Data*)> mStreamCallback;
  std::vector<AudioDevice> mDevices;
};

SoundcardADC::Data::Data( SoundcardADC* pSelf )
: mpSelf( pSelf ),
  mpStream( 0 ),
  mpData( 0 ),
  mStreamCallback( &Data::StreamCallback, this )
{
}

SoundcardADC::Data::~Data()
{
  delete mpStream;
  delete[] mpData;
}

void
SoundcardADC::Data::ScanForDevices()
{
  AudioDevice::Rescan();
  mDevices.clear();
  for( AudioDevice device = AudioDevice::First(); device.Exists(); device = device.Next() )
    if(device.SupportsRecording() )
      mDevices.push_back( device );
}

void
SoundcardADC::Data::StreamCallback()
{
  int currentSample = 0;
  GenericSignal* pBuffer = mpSelf->LockWriteBuffer();
  for( int samplesRead = 1; samplesRead > 0; )
  {
    samplesRead = mpStream->Read( mpData, pBuffer->Elements() - currentSample );
    int idx = 0;
    for( int sample = 0; sample < samplesRead; ++sample )
    {
      for( int ch = 0; ch < pBuffer->Channels(); ++ch )
        pBuffer->Value( ch, currentSample + sample ) = mpData[idx++];
      idx += mpStream->RecordingChannels() - pBuffer->Channels();
    }
    currentSample += samplesRead;
  }
  Assert( currentSample == pBuffer->Elements() );
  mpSelf->ReleaseWriteBuffer( pBuffer );
}

// SoundcardADC
SoundcardADC::SoundcardADC()
: d( new Data( this ) )
{
}

SoundcardADC::~SoundcardADC()
{
  delete d;
}

void
SoundcardADC::OnPublish()
{
  d->ScanForDevices();
  size_t count = d->mDevices.size();
  std::ostringstream oss;
  oss << "Source int AudioInputDevice= auto 0 0 " << count
      << " // audio input device 0: auto ";
  for( size_t i = 0; i < d->mDevices.size(); ++i )
    oss << i + 1 << ": " << d->mDevices[i].Name() << " ";
  oss << "(enumeration)" << flush;
  std::string deviceParam = oss.str();

 BEGIN_PARAMETER_DEFINITIONS
  "Source int SourceCh= auto "
      "auto 0 % // number of digitized and stored channels",
  "Source int SampleBlockSize= auto "
      "auto 1 % // number of samples transmitted at a time",
  "Source float SamplingRate= auto "
      "auto 1.0 % // sample rate",
  "Source list SourceChGain= 1 auto "
      " // Volts per ADC unit for unattenuated consumer line input",
  "Source list SourceChOffset= 1 auto "
      " // raw offset",
  "Source list ChannelNames= 1 auto "
      " // names of amplifier channels",
  deviceParam.c_str()
 END_PARAMETER_DEFINITIONS
}

AudioStream*
SoundcardADC::CreateStream() const
{
  AudioStream* p = new AudioStream( d->mDevices[Parameter( "AudioInputDevice" ) - 1] );
  p->SetSampleRate( Rate( Parameter( "SamplingRate" ).InHertz(), Time::OneSecond ) )
    .SetRecordingChannels( Parameter( "SourceCh" ) )
    .SetPlaybackChannels( 0 )
    .SetSamplesInBuffer( Parameter( "SampleBlockSize" ) )
    .SetRecordingLatency( Time::Seconds( 0.01 ) );
  return p;
}

void
SoundcardADC::OnAutoConfig()
{
  if( d->mDevices.empty() )
    throw bcierr << "No audio devices available for input";

  int idx = 0;
  for( int i = 0; idx == 0 && i < d->mDevices.size(); ++i )
    if( !AudioStream( AudioDevice( d->mDevices[i] ) ).Error() )
      idx = i + 1;

  if( idx < 0 )
    Parameter( "AudioInputDevice" ) = "";
  else
    Parameter( "AudioInputDevice" ) = idx;
  idx = ActualParameter( "AudioInputDevice" );
  if( idx < 1 || idx > d->mDevices.size() )
    throw bcierr << "Selected AudioInputDevice #" << idx << " does not exist";
  AudioStream s( d->mDevices[idx-1] );
  if( s.Error() )
    throw bcierr << "Could not connect to device: " << s.Error();

  Parameter( "SourceCh" ) = s.RecordingChannels();
  int channels = ActualParameter( "SourceCh" );
  Parameter( "ChannelNames" )->SetNumValues( channels );
  Parameter( "SourceChGain" )->SetNumValues( channels );
  Parameter( "SourceChOffset" )->SetNumValues( channels );
  if( channels == 1 )
    Parameter( "ChannelNames" )( 0 ) = "audio";
  else if( channels == 2 )
  {
    Parameter( "ChannelNames" )( 0 ) = "audioL";
    Parameter( "ChannelNames" )( 1 ) = "audioR";
  }
  else for( int i = 0; i < channels; ++i )
    Parameter( "ChannelNames" )( i ) << "audio" << i + 1;

  for( int i = 0; i < channels; ++i )
  {
    double gainFactor = 0.447;
    Parameter( "SourceChGain" )( i ) << gainFactor << "V";
    Parameter( "SourceChOffset" )( i ) = 0;
  }
  Parameter( "SamplingRate" ) << s.SampleRate().Hertz() << "Hz";
  int sampleBlockSize = s.SampleRate().Hertz() / 100;
  if( sampleBlockSize < 1 )
    sampleBlockSize = 1;
  if( s.SamplesInBuffer() > 0 )
    sampleBlockSize = ( ( sampleBlockSize + s.SamplesInBuffer() - 1 ) / s.SamplesInBuffer() ) * s.SamplesInBuffer();
  Parameter( "SampleBlockSize" ) =  sampleBlockSize;
}

void
SoundcardADC::OnPreflight( SignalProperties& Output ) const
{
  AudioStream* p = CreateStream();
  if( p->Start().Error() )
    throw bcierr << "Fatal audio stream error: " << p->Error();
  delete p;
  Output.ValueUnit().SetRawMin( -1 ).SetRawMax( 1 );
}

void
SoundcardADC::OnInitialize( const SignalProperties& Output )
{
  delete[] d->mpData;
  d->mpData = new float[Output.Channels() * Output.Elements()];
  delete d->mpStream;
  d->mpStream = CreateStream();
  if( d->mpStream->Error() )
    throw bcierr << "Fatal audio stream error: " << d->mpStream->Error();
}

void
SoundcardADC::OnStartAcquisition()
{
  d->mpStream->AddListener( AudioStream::OnTick, &d->mStreamCallback );
  d->mpStream->Start();
}

void
SoundcardADC::OnStopAcquisition()
{
  d->mpStream->Stop();
  d->mpStream->RemoveListener( AudioStream::OnTick, &d->mStreamCallback );
}

