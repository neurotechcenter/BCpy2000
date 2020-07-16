////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class representing an audio playback or recording stream.
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
#include "AudioDevice.h"

#include "Synchronized.h"
#include "WaitableEvent.h"
#include "PortAudio.h"
#include "EnvVariable.h"
#include "Lockable.h"
#include "ExceptionCatcher.h"
#include "Debugging.h"
#include "StaticObject.h"
#include "Thread.h"
#include <vector>
#include <string>
#include <sstream>

//  WASAPI note:
//  PortAudio provides a WASAPI host API implementation as well. We do not
//  use it for a number of reasons:
//   * Pa's output selection is device-centric, whereas WASAPI uses "endpoints"
//     such as output jacks in a device-agnostic fashion. Pa will map endpoints
//     to Pa devices, and will treat unplugged jacks as not present, which makes
//     it impossible to enumerate audio devices in a consistent fashion.
//   * Pa's ideas about how to prioritize threads, and when to sleep a thread,
//     do not play well together with our own demands for CPU time when
//     rendering stimuli.
//   * Pa WASAPI time stamp information is based on separate measurements of
//     time, and free buffer space, resulting in imprecision of at least the
//     amount of a time slice duration. Also, free buffer space is not
//     guaranteed to be updated continuously, increasing imprecision.
//     Using our own implementation, we may use the IAudioClock/IAudioClock2
//     WASAPI interfaces in order to obtain highly accurate combined time stamp
//     and sample position for an output device. This is important to minimize
//     artifacts when synchronizing audio and video playback.
//   * Pa WASAPI seems to be broken on at least some Win7 configurations.

#if _WIN32
# include <audioclient.h>
# include <mmdeviceapi.h>
# include <FunctionDiscoveryKeys_devpkey.h>
# include "../extlib/com/ComPtr.h"
# include <avrt.h>
static const DWORD sComCtx = CLSCTX_INPROC_SERVER;
static const GUID IID_IMMDeviceEnumerator = { 0xA95664D2, 0x9614, 0x4F35, { 0xA7, 0x46, 0xDE, 0x8D, 0xB6, 0x36, 0x17, 0xE6 } };
static const GUID CLSID_MMDeviceEnumerator = { 0xBCDE0395, 0xE52F, 0x467C, { 0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E } };
static const GUID IID_IAudioClient = { 0x1CB9AD4C, 0xDBFA, 0x4C32, { 0xB1, 0x78, 0xC2, 0xF5, 0x68, 0xA7, 0x03, 0xB2 } };
static const GUID IID_IAudioRenderClient = { 0xF294ACFC, 0x3146, 0x4483, { 0xA7, 0xBF, 0xAD, 0xDC, 0xA7, 0xC2, 0x60, 0xE2 } };
static const GUID IID_IAudioClock = __uuidof(IAudioClock);
static const GUID IID_IAudioClock2 = __uuidof(IAudioClock2);
static const GUID IID_IMMNotificationClient = __uuidof(IMMNotificationClient);
double OneSecond_REFERENCE_TIME = 1e7;

namespace {

template<class T> class CoTaskMemPtr
{
public:
  CoTaskMemPtr() : mp( nullptr ) {}
  CoTaskMemPtr( CoTaskMemPtr& other ) : mp( other.mp ) { other.mp = nullptr; }
  CoTaskMemPtr& operator=( CoTaskMemPtr& other ) { *Assignee() = other.mp; other.mp = nullptr; return *this; }
  ~CoTaskMemPtr() { Free(); }

  T* Ptr() { return mp; }
  T** Assignee() { Free(); return &mp; }

  operator T*() { return mp; }
  T* operator->() { return mp; }
  T& operator*() { return *mp; }

private:
  void Free() { if( mp ) ::CoTaskMemFree( mp ); mp = nullptr; }
  T* mp;
};

class auto_HANDLE
{
public:
  auto_HANDLE( HANDLE h = 0 ) : h(h) {}
  auto_HANDLE operator=( auto_HANDLE& other ) { Dispose(); h = other.h; other.h = NULL; }
  ~auto_HANDLE() { Dispose(); }
  operator HANDLE() const { return h; }
private:
  void Dispose() { if( h && h != INVALID_HANDLE_VALUE ) ::CloseHandle( h ); h = NULL; }
  HANDLE h;
};

struct Win32Fail
{
  int Error() { return err; }
protected:
  int err;
};

class HR_Fail : public Win32Fail
{
public:
  HR_Fail() {}
  HR_Fail( HRESULT hr ) : hr( hr ) { if( FAILED( hr ) ) { err = hr; throw *this; } }
  operator HRESULT() const { return hr; }
private:
  HRESULT hr;
};

class ErrFail : public Win32Fail
{
public:
  ErrFail() {}
  ErrFail( int i ) : i( i ) { if( i ) { err = ::GetLastError(); if( !err ) err = i; throw *this; } }
  operator int() const { return err; }
private:
  int i;
};

template<class T> class NullFail : public Win32Fail
{
public:
  NullFail() {}
  NullFail( T t ) : t( t ) { if( !t ) { err = ::GetLastError(); throw *this; } }
  operator T() const { return t; }
private:
  T t;
};

struct UsingCom
{
  UsingCom( DWORD dwInit )
  {
    HRESULT hr = ::CoInitializeEx( nullptr, dwInit );
    if( FAILED( hr ) )
      throw std_runtime_error << "Could not initialize COM: " << SysError( hr ).Message();
  }
  ~UsingCom()
  {
    ::CoUninitialize();
  }
};

std::string WasapiErrorString( int err )
{
  switch( err )
  {
    case AUDCLNT_E_DEVICE_INVALIDATED:
      return "Device unplugged or reconfigured (AUDCLNT_E_DEVICE_INVALIDATED)";
    case AUDCLNT_E_DEVICE_IN_USE:
      return "Another application is using the device (AUDCLNT_E_DEVICE_IN_USE)";
    case AUDCLNT_E_BUFFER_TOO_LARGE:
      return "Requested buffer size is not available (AUDCLNT_E_BUFFER_TOO_LARGE)";
    case AUDCLNT_E_UNSUPPORTED_FORMAT:
      return "Unsupported format (AUDCLNT_E_UNSUPPORTED_FORMAT)";
    case AUDCLNT_E_EXCLUSIVE_MODE_NOT_ALLOWED:
      return "Exclusive mode disabled for device (AUDCLNT_E_EXCLUSIVE_MODE_NOT_ALLOWED)";
  }
  return SysError( err ).Message();
}

} // namespace
#endif

namespace
{

struct AudioInit
{
  AudioInit()
  {
    ::Pa_Initialize();
  }
  ~AudioInit()
  {
    ::Pa_Terminate();
  }
};
static StaticObject<AudioInit> sAudioInit;

struct AudioMode
{
  enum
  {
    Unknown = 0,
    PortAudio,
#if _WIN32
    WasapiNative,
#endif
  } mode;
  union
  {
    int paHostApis[16];
#if _WIN32
    AUDCLNT_SHAREMODE wasapiShareMode;
#endif
  } details;
};

AudioMode ParseAudioMode( const std::string& inModeString )
{
  AudioMode mode = { AudioMode::Unknown };
  std::string s1 = StringUtils::ToLower( inModeString ), s2;
  if( s1.empty() || s1 == "default" )
  {
#if _WIN32
    s1 = "wasapi shared";
#else
    s1 = "portaudio";
#endif
  }
  size_t pos = s1.find_first_of( " \t\n" );
  if( pos != std::string::npos )
    s2 = s1.substr( pos + 1 ), s1 = s1.substr( 0, pos );
  if( s1 == "portaudio" )
  {
    int* pApiIdx = mode.details.paHostApis,
        *pApiIdxEnd = pApiIdx + sizeof(mode.details.paHostApis)/sizeof(*mode.details.paHostApis);
    if( s2.empty() )
    {
      const PaHostApiTypeId apis[] = // in order of preference
      {
        // Windows
        paASIO,
        // omitting paWASAPI because it appears to be broken
        paWDMKS,
        paDirectSound,
        paMME,
        // Linux
        paALSA,
        paOSS,
        paJACK,
        // OSX
        paCoreAudio,
      };
      for( auto id : apis )
        for( PaHostApiIndex i = 0; i < ::Pa_GetHostApiCount(); ++i )
          if( ::Pa_GetHostApiInfo( i )->type == id && pApiIdx < pApiIdxEnd - 1 )
            *pApiIdx++ = i;
    }
    else if( s2 == "default" )
    {
      *pApiIdx++ = ::Pa_GetDefaultHostApi();
    }
    else
    {
      std::clog << "PortAudio host apis:";
      for( PaHostApiIndex i = 0; i < ::Pa_GetHostApiCount(); ++i )
      {
        const char* name = ::Pa_GetHostApiInfo( i )->name;
        std::clog << " \"" << name << "\"";
        if( !::stricmp( s2.c_str(), name ) && pApiIdx < pApiIdxEnd - 1 )
          *pApiIdx++ = i;
      }
      std::clog << std::endl;
    }
    *pApiIdx++ = -1;
    if( mode.details.paHostApis[0] >= 0 )
      mode.mode = AudioMode::PortAudio;
  }
#if _WIN32
  else if( s1 == "wasapi" )
  {
    mode.mode = AudioMode::WasapiNative;
    if( s2.empty() || s2 == "default" || s2 == "shared" )
      mode.details.wasapiShareMode = AUDCLNT_SHAREMODE_SHARED;
    else if( s2 == "exclusive" )
      mode.details.wasapiShareMode = AUDCLNT_SHAREMODE_EXCLUSIVE;
    else
      mode.mode = AudioMode::Unknown;
  }
#endif
  return mode;
}

const struct AudioMode& AudioMode()
{
  static struct AudioMode sMode;
  if( sMode.mode == AudioMode::Unknown )
  {
    std::string s = EnvVariable( "BCI2000_AUDIO_MODE" ).Get();
    sMode = ParseAudioMode( s );
    if( sMode.mode == AudioMode::Unknown )
      throw std_runtime_error << "Unknown BCI2000_AUDIO_MODE: " << s;
  }
  return sMode;
}

std::string PortAudioError( PaError err )
{
  if( !err )
    return "";
  std::ostringstream oss;
  oss << "PortAudio error #" << err << ": " << Pa_GetErrorText( err );
  return oss.str();
}
#define Pa_GetErrorText void

struct DeviceInfo
{
  std::string name;
  bool recording, playback, recordingDefault, playbackDefault;
  PaDeviceIndex portAudioIndex;
  enum { active, disabled, unplugged, };
  int state;
#if _WIN32
  std::wstring mmDeviceId;
#endif
};
StaticObject<SynchronizedObject<std::vector<DeviceInfo>>> sDevices;

bool PortAudioDeviceScan()
{
  std::set<std::string> detectedDevices;
  (*sDevices).Mutable()->clear();
  if( AudioMode().mode != AudioMode::PortAudio )
    return false;
  auto pApiIdx = AudioMode().details.paHostApis;
  while( *pApiIdx > 0 )
  {
    PaHostApiIndex apiIdx = *pApiIdx++;
    const PaHostApiInfo* pApi = ::Pa_GetHostApiInfo( apiIdx );
    for( int i = 0; i < pApi->deviceCount; ++i )
    {
      PaDeviceIndex dev = ::Pa_HostApiDeviceIndexToDeviceIndex( apiIdx, i );
      const PaDeviceInfo* pInfo = ::Pa_GetDeviceInfo( dev );
      if( pInfo )
      {
        if( detectedDevices.find( pInfo->name ) == detectedDevices.end() )
        {
          DeviceInfo info = { pInfo->name, pInfo->maxInputChannels, pInfo->maxOutputChannels, dev, DeviceInfo::active };
          (*sDevices).Mutable()->push_back( info );
          detectedDevices.insert( info.name );
        }
      }
    }
  }
  return true;
}

bool PortAudioDefaultDevices( int* pOutInputDevice, int* pOutOutputDevice )
{
  if( pOutInputDevice )
    *pOutInputDevice = -1;
  if( pOutOutputDevice )
    *pOutOutputDevice = -1;
  PaDeviceIndex input = paNoDevice, output = paNoDevice;
  if( AudioMode().mode != AudioMode::PortAudio )
    return false;
  auto pApiIdx = AudioMode().details.paHostApis;
  while( *pApiIdx > 0 )
  {
    PaHostApiIndex apiIdx = *pApiIdx++;
    if( apiIdx != paHostApiNotFound )
    {
      const PaHostApiInfo* pApi = ::Pa_GetHostApiInfo( apiIdx );
      if( input == paNoDevice )
        input = pApi->defaultInputDevice;
      if( output == paNoDevice )
        output = pApi->defaultOutputDevice;
    }
  }
  if( input == paNoDevice )
    input = ::Pa_GetDefaultInputDevice();
  if( output == paNoDevice )
    output = ::Pa_GetDefaultOutputDevice();

  WithLocked( p = sDevices->Mutable() )
    for( int i = 0; i < p->size(); ++i )
    {
      if( (*p)[i].portAudioIndex == input && pOutInputDevice )
        *pOutInputDevice = i;
      if( (*p)[i].portAudioIndex == output && pOutOutputDevice )
        *pOutOutputDevice = i;
    }
  return true;
}

PaDeviceIndex GetPaDeviceIndex( DeviceInfo device )
{
  PaDeviceIndex paIndex = paNoDevice;
  if( AudioMode().mode != AudioMode::PortAudio )
    return paIndex;
  paIndex = device.portAudioIndex;
  if( paIndex != paNoDevice && !::Pa_GetDeviceInfo( paIndex ) )
    paIndex = paNoDevice;
  else
  {
    auto pApiIdx = AudioMode().details.paHostApis;
    while( paIndex == paNoDevice && *pApiIdx > 0 )
    {
      PaHostApiIndex apiIdx = *pApiIdx++;
      const PaHostApiInfo* pApi = ::Pa_GetHostApiInfo( apiIdx );
      for( int i = 0; i < pApi->deviceCount; ++i )
      {
        PaDeviceIndex dev = ::Pa_HostApiDeviceIndexToDeviceIndex( apiIdx, i );
        const PaDeviceInfo* pInfo = ::Pa_GetDeviceInfo( dev );
        if( pInfo && pInfo->name == device.name )
          paIndex = dev;
      }
    }
  }
  return paIndex;
}


#if _WIN32
static struct DeviceList : IMMNotificationClient
{
public:
  DeviceList();
  ~DeviceList();
  int Error() const { return mError; }
  void Scan();
  void Update( const wchar_t* );
  void Remove( const wchar_t* );
  void SetDefault( const wchar_t*, EDataFlow );

protected:
  HRESULT __stdcall QueryInterface( const IID& inIID, void** outInterface ) override
    {
      *outInterface = nullptr;
      if( inIID == IID_IMMNotificationClient )
        *outInterface = static_cast<IMMNotificationClient*>( this );
      else if( inIID == IID_IUnknown )
        *outInterface = static_cast<IUnknown*>( this );
      return *outInterface ? S_OK : E_NOINTERFACE;
    }
  ULONG __stdcall AddRef() override { return 0; }
  ULONG __stdcall Release() override { return 0; }
  HRESULT __stdcall OnDefaultDeviceChanged( EDataFlow flow, ERole role, LPCWSTR id ) override
    { if( role == eMultimedia ) SetDefault( id, flow ); return S_OK; }
  HRESULT __stdcall OnDeviceAdded( LPCWSTR id ) override
    { Update( id ); return S_OK; }
  HRESULT __stdcall OnDeviceRemoved( LPCWSTR id ) override
    { Remove( id ); return S_OK; }
  HRESULT __stdcall OnDeviceStateChanged( LPCWSTR id, DWORD state ) override
    { if( state == DEVICE_STATE_NOTPRESENT ) Remove( id ); else Update( id ); return S_OK; }
  HRESULT __stdcall OnPropertyValueChanged( LPCWSTR id, const PROPERTYKEY key ) override
    { if( key == PKEY_Device_FriendlyName ) Update( id ); return S_OK; }

private:
  UsingCom mUsingCom;
  int mError;
  com::Ptr<IMMDeviceEnumerator> mpEnumerator;

} sDeviceList;

DeviceList::DeviceList()
: mUsingCom( COINIT_APARTMENTTHREADED ) // COINIT_MULTITHREADED not allowed in GUI threads
{
  try
  {
    HR_Fail() = ::CoCreateInstance( CLSID_MMDeviceEnumerator, 0, sComCtx, IID_IMMDeviceEnumerator, mpEnumerator.Assignee() );
    HR_Fail() = mpEnumerator->RegisterEndpointNotificationCallback( this );
    Scan();
  }
  catch( HR_Fail f )
  {
    mError = f.Error();
  }
}

DeviceList::~DeviceList()
{
}

void
DeviceList::Scan()
{
  HR_Fail hr;
  int state = DEVICE_STATE_ACTIVE | DEVICE_STATE_UNPLUGGED | DEVICE_STATE_DISABLED;
  wchar_t* id = 0;
  com::Ptr<IMMDevice> pMMDevice;
  com::Ptr<IMMDeviceCollection> pCollection;
  HRESULT r = mpEnumerator->EnumAudioEndpoints( eAll, state, pCollection.Assignee() );
  if( r != E_NOTFOUND )
  {
    hr = r;
    for( int i = 0; S_OK == pCollection->Item( i, pMMDevice.Assignee() ); ++i )
    {
      hr = pMMDevice->GetId( &id );
      Update( id );
    }
  }
  EDataFlow flows[] = { eRender, eCapture };
  for( auto flow : flows )
  {
    r = mpEnumerator->GetDefaultAudioEndpoint( flow, eMultimedia, pMMDevice.Assignee() );
    if( r != E_NOTFOUND )
    {
      hr = r;
      hr = pMMDevice->GetId( &id );
      SetDefault( id, flow );
    }
  }
}

void
DeviceList::Update( const wchar_t* id )
{
  Assert( id );
  bool remove = true;
  DeviceInfo info = { "", false, false, false, false, paNoDevice, 0 };
  WithLocked( p = sDevices->Const() )
    for( auto& d : *p )
      if( d.mmDeviceId == id )
        info = d;
  if( info.mmDeviceId.empty() )
    info.mmDeviceId = id;
  com::Ptr<IMMDevice> pMMDevice;
  if( S_OK == mpEnumerator->GetDevice( id, pMMDevice.Assignee() ) )
  {
    DWORD state = 0;
    com::Ptr<IPropertyStore> pProps;
    HR_Fail() = pMMDevice->OpenPropertyStore( STGM_READ, pProps.Assignee() );
    PROPVARIANT name;
    ::PropVariantInit( &name );
    HR_Fail() = pProps->GetValue( PKEY_Device_FriendlyName, &name );
    info.name = StringUtils::ToNarrow( name.pwszVal );
    ::PropVariantClear( &name );
    HR_Fail() = pMMDevice->GetState( &state );
    switch( state )
    {
    case DEVICE_STATE_ACTIVE:
      remove = false;
      info.state = DeviceInfo::active;
      break;
    case DEVICE_STATE_UNPLUGGED:
      remove = false;
      info.state = DeviceInfo::unplugged;
      break;
    case DEVICE_STATE_DISABLED:
      remove = false;
      info.state = DeviceInfo::disabled;
      break;
    case DEVICE_STATE_NOTPRESENT:
    default:
      ;
    }
    struct { EDataFlow f; bool* b; } flows[] =
    {
      { eRender, &info.playback },
      { eCapture, &info.recording },
    };
    for( auto flow : flows )
    {
      com::Ptr<IMMDeviceCollection> pCollection;
      HRESULT r = mpEnumerator->EnumAudioEndpoints( flow.f, state, pCollection.Assignee() );
      if( r != E_NOTFOUND )
      {
        HR_Fail( r );
        com::Ptr<IMMDevice> pMMDevice;
        for( int i = 0; S_OK == pCollection->Item( i, pMMDevice.Assignee() ); ++i )
        {
          wchar_t* id2;
          HR_Fail() = pMMDevice->GetId( &id2 );
          if( !::wcscmp( id, id2 ) )
            *flow.b = true;
        }
      }
    }
  }
  if( remove )
    Remove( id );
  else WithLocked( p = sDevices->Mutable() )
  {
    bool found = false;
    for( auto& d : *p )
      if( d.mmDeviceId == id )
        found = true, d = info;
    if( !found )
      p->push_back( info );
  }
}

void
DeviceList::Remove( const wchar_t* id )
{
  Assert( id );
  WithLocked( p = sDevices->Mutable() )
  {
    auto i = p->begin();
    while( i != p->end() && i->mmDeviceId != id )
      ++i;
    if( i != p->end() )
      p->erase( i );
  }
}

void
DeviceList::SetDefault( const wchar_t* id, EDataFlow flow )
{
  WithLocked( p = sDevices->Mutable() ) for( auto& d : *p )
  {
    bool match = ( id && id == d.mmDeviceId );
    if( flow == eRender )
      d.playbackDefault = match;
    else if( flow == eCapture )
      d.recordingDefault = match;
    else if( flow == eAll )
      d.playbackDefault = d.recordingDefault = match;
  }
}

bool NativeDeviceScan()
{
  return sDeviceList.Error() == 0;
}

bool NativeDefaultDevices( int* pOutInputDevice, int* pOutOutputDevice )
{
  if( sDeviceList.Error() )
    return false;

  if( pOutInputDevice )
    *pOutInputDevice = -1;
  if( pOutOutputDevice )
    *pOutOutputDevice = -1;
  WithLocked( p = sDevices->Const() )
  {
    for( int i = 0; i < p->size(); ++i )
    {
      const DeviceInfo& info = (*p)[i];
      if( pOutInputDevice && *pOutInputDevice < 0 )
      {
        if( info.recordingDefault  )
          *pOutInputDevice = i;
      }
      if( pOutOutputDevice  && *pOutOutputDevice < 0 )
      {
        if( info.playbackDefault )
          *pOutOutputDevice = i;
      }
    }
  }
  WithLocked( p = sDevices->Const() )
  {
    for( int i = 0; i < p->size(); ++i )
    {
      const DeviceInfo& info = (*p)[i];
      if( pOutInputDevice && *pOutInputDevice < 0 )
      {
        if( info.recording && info.state == DeviceInfo::active )
          *pOutInputDevice = i;
      }
      if( pOutOutputDevice  && *pOutOutputDevice < 0 )
      {
        if( info.playback && info.state == DeviceInfo::active )
          *pOutOutputDevice = i;
      }
    }
  }
  return true;
}

#else

bool NativeDeviceScan() { return false; }
bool NativeDefaultDevices( int*, int* ) { return false; }

#endif // _WIN32

PaDeviceIndex DeviceIndexToPaDeviceIndex( int index )
{
  if( index < 0 || index >= (*sDevices)->size() )
    return paNoDevice;
  return GetPaDeviceIndex( (*sDevices->Const())[index] );
}

} // namespace

AudioDevice::AudioDevice( int index )
: mIndex( index )
{
  static bool init = Rescan();
}

bool
AudioDevice::Rescan()
{
  sAudioInit();
  return NativeDeviceScan() || PortAudioDeviceScan();
}

bool
AudioDevice::Exists() const
{
  return mIndex >= 0 && mIndex < (*sDevices)->size();
}

bool
AudioDevice::Active() const
{
  return Exists() ? (*sDevices)->at( mIndex ).state == DeviceInfo::active : false;
}

String
AudioDevice::Name() const
{
  return Exists() ? (*sDevices)->at( mIndex ).name : "";
}

bool
AudioDevice::SupportsRecording() const
{
  return Exists() ? (*sDevices)->at( mIndex ).recording : false;
}

bool
AudioDevice::SupportsPlayback() const
{
  return Exists() ? (*sDevices)->at( mIndex ).playback : false;
}

struct AudioStream::Data
{
  Data( AudioStream* );
  bool TryChange();

  static int PaStreamCallback(
    const float*, float*, unsigned long,
    const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags,
    Data*
  );

  AudioStream* mpSelf;
  int mDeviceIndex;

  Synchronized<PaStream*> mpPaStream;
  bool CreatePaStream();

#if _WIN32
  bool CreateWasapiStream( AUDCLNT_SHAREMODE );
  struct WasapiThread : Thread
  { Synchronized<AudioStream::Data*> d;
    const wchar_t* mDeviceId;
    AUDCLNT_SHAREMODE mShareMode;
    WaitableEvent mInitialized, mStart;
    auto_HANDLE mStop;
    struct OnTerminate : Runnable { WasapiThread* p; void OnRun() override; } mOnTerminate;
    Synchronized<int> mSamplesInDeviceBuffer;
    WasapiThread();
    ~WasapiThread();
    com::Ptr<IAudioClient> Initialize();
    int OnExecute();
    void EventLoop( com::Ptr<IAudioClient> );
  } mWasapiThread;
#endif

  SynchronizedObject<ClockTick> mAdcTick, mDacTick;
  const void* mpReadFrom;
  void* mpWriteTo;
  size_t mRemainingToRead,
         mRemainingToWrite;

  void (*mReadFunc)( float*, const void*&, size_t );
  void (*mWriteFunc)( void*&, const float*, size_t );
  static void ReadFromFloat32( float*, const void*&, size_t );
  static void WriteToFloat32( void*&, const float*, size_t );
  static void ReadFromInt16( float*, const void*&, size_t );
  static void WriteToInt16( void*&, const float*, size_t );
  static void ReadFromInt32( float*, const void*&, size_t );
  static void WriteToInt32( void*&, const float*, size_t );

  SynchronizedObject<String, Mutex> mError;

  String mDeviceInfo;
  enum { Destroyed = 0, Creating, Stopped, Starting, Active, Stopping, Destroying };
  Synchronized<int> mState;

  Synchronized<int> mPlaybackChannels, mRecordingChannels, 
                    mSamplesInBuffer; // may be 0 to indicate automatic choice
  Rate mSampleRate;
  Time::Interval mRecordingLatency, mPlaybackLatency;
  Synchronized<bool> mCallbackTimedOut;
};

#if _WIN32

namespace Wasapi {

class AudioClock
{
public:
  AudioClock( IAudioClient*, AUDCLNT_SHAREMODE, bci::Rate );
  const ClockTick& Now();
private:
  com::Ptr<IAudioClock> mpAudioClock;
  com::Ptr<IAudioClock2> mpAudioClock2;
  ClockTick mTick;
  float mRateFactor;
};

AudioClock::AudioClock( IAudioClient* p, AUDCLNT_SHAREMODE mode, bci::Rate rate )
: mTick( ClockTick::Null ), mRateFactor( 1 )
{
  p->GetService( IID_IAudioClock, mpAudioClock.Assignee() );
  if( mpAudioClock )
  {
    UINT64 deviceFrequency = 0;
    if( SUCCEEDED( mpAudioClock->GetFrequency( &deviceFrequency ) ) )
    {
      mRateFactor = (Rate::FromHertz(deviceFrequency) / rate).To<double>();
      mTick.rate = rate;
      mTick.type = ClockType::Hardware | ClockType::Audio | ClockType::RealTime;
    }
    if( mode == AUDCLNT_SHAREMODE_SHARED )
      mpAudioClock->QueryInterface( IID_IAudioClock2, mpAudioClock2.Assignee() );
  }
}

const ClockTick&
AudioClock::Now()
{
  bool ok = false;
  UINT64 devPos = 0;
  HR_Fail hr;
  if( mpAudioClock2 )
    hr = mpAudioClock2->GetDevicePosition( &devPos, nullptr );
  else
    hr = mpAudioClock->GetPosition( &devPos, nullptr );
  // timestamps from AudioClock interfaces are unreliable
  mTick.time = TimeUtils::MonotonicTime();
  mTick.count = devPos / mRateFactor;
  return mTick;
}

} // namespace

bool
AudioStream::Data::CreateWasapiStream( AUDCLNT_SHAREMODE inMode )
{
  mDeviceInfo.clear();
  if( mRecordingChannels > 0 )
  {
    mError.Mutable().Ref() << "Recording is not supported.";
    return false;
  }
  int index = mDeviceIndex;
  if( index < 0 )
    NativeDefaultDevices( nullptr, &index );
  if( index < 0 )
    return false;

  struct DeviceInfo device = (*sDevices)->at( index );
  mDeviceInfo = device.name;
  switch( inMode )
  {
    case AUDCLNT_SHAREMODE_EXCLUSIVE:
      mDeviceInfo += " [WASAPI exclusive]";
      break;
    case AUDCLNT_SHAREMODE_SHARED:
      mDeviceInfo += " [WASAPI shared]";
      break;
    default:
      mDeviceInfo += " [WASAPI]";
  }
  mWasapiThread.mDeviceId = device.mmDeviceId.c_str();
  mWasapiThread.mShareMode = inMode;
  Assert( !mWasapiThread.Running() );
  mWasapiThread.mStart.Reset();
  mWasapiThread.mInitialized.Reset();
  mWasapiThread.Start();
  Waitables().Add( mWasapiThread ).Add( mWasapiThread.mInitialized ).Wait();
  mWasapiThread.mInitialized.Reset();
  return mWasapiThread.Running();
}

AudioStream::Data::WasapiThread::WasapiThread()
: mInitialized( true ), mStart( true ), mStop( ::CreateEventA( nullptr, true, false, nullptr ) ),
  mDeviceId( nullptr ), d( 0 )
{
  mOnTerminate.p = this;
}

AudioStream::Data::WasapiThread::~WasapiThread()
{
  TerminateAndWait();
}

void
AudioStream::Data::WasapiThread::OnTerminate::OnRun()
{
  ::SetEvent(p->mStop);
}

com::Ptr<IAudioClient>
AudioStream::Data::WasapiThread::Initialize()
{
  HR_Fail hr_fail;
  com::Ptr<IMMDeviceEnumerator> pEnumerator;
  hr_fail = ::CoCreateInstance( CLSID_MMDeviceEnumerator, 0, sComCtx, IID_IMMDeviceEnumerator, pEnumerator.Assignee() );
  com::Ptr<IMMDevice> pDevice;
  hr_fail = pEnumerator->GetDevice( mDeviceId, pDevice.Assignee() );
  com::Ptr<IAudioClient> pClient;
  hr_fail = pDevice->Activate( IID_IAudioClient, sComCtx, nullptr, pClient.Assignee() );

  CoTaskMemPtr<WAVEFORMATEX> pFormat, pClosest;
  hr_fail = pClient->GetMixFormat( pFormat.Assignee() );
  if( mShareMode == AUDCLNT_SHAREMODE_EXCLUSIVE && !IsNaN( d->mSampleRate ) )
    pFormat->nSamplesPerSec = d->mSampleRate.Hertz();
  HRESULT supp = pClient->IsFormatSupported( mShareMode, pFormat, pClosest.Assignee() );
  if( supp == S_FALSE && pClosest )
    pFormat = pClosest;
  else if( supp == AUDCLNT_E_UNSUPPORTED_FORMAT )
  {
    if( pFormat->wFormatTag == WAVE_FORMAT_EXTENSIBLE )
    {
      auto p = reinterpret_cast<WAVEFORMATEXTENSIBLE*>( pFormat.Ptr() );
      p->SubFormat = KSDATAFORMAT_SUBTYPE_PCM;
      p->Samples.wValidBitsPerSample = 16;
    }
    else
      pFormat->wFormatTag = WAVE_FORMAT_PCM;
    pFormat->wBitsPerSample = 16;
    pFormat->nBlockAlign = pFormat->wBitsPerSample * pFormat->nChannels / 8;
    pFormat->nAvgBytesPerSec = pFormat->nBlockAlign * pFormat->nSamplesPerSec;
    supp = pClient->IsFormatSupported( mShareMode, pFormat, pClosest.Assignee() );
  }
  else
    hr_fail = supp;

  int dataFormat = 0;
  if( pFormat->wFormatTag == WAVE_FORMAT_EXTENSIBLE )
  {
    WAVEFORMATEXTENSIBLE* p = reinterpret_cast<WAVEFORMATEXTENSIBLE*>( pFormat.Ptr() );
    if( p->SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT )
      dataFormat = WAVE_FORMAT_IEEE_FLOAT;
    else if( p->SubFormat == KSDATAFORMAT_SUBTYPE_PCM )
      dataFormat = WAVE_FORMAT_PCM;
  }
  else
    dataFormat = pFormat->wFormatTag;
  d->mWriteFunc = nullptr;
  if( dataFormat == WAVE_FORMAT_IEEE_FLOAT && pFormat->wBitsPerSample == 32 )
  {
    d->mWriteFunc = &AudioStream::Data::WriteToFloat32;
  }
  else if( dataFormat == WAVE_FORMAT_PCM ) switch( pFormat->wBitsPerSample )
  {
    case 16:
      d->mWriteFunc = &AudioStream::Data::WriteToInt16;
      break;
    case 32:
      d->mWriteFunc = &AudioStream::Data::WriteToInt32;
      break;
  }
  if( !d->mWriteFunc )
  {
    d->mError.Mutable().Ref() << "Unsupported sample format";
    SuggestDebugging << d->mError.Const().Ref();
  }

  REFERENCE_TIME timeValue = 0;
  if( mShareMode == AUDCLNT_SHAREMODE_EXCLUSIVE )
    hr_fail = pClient->GetDevicePeriod( nullptr, &timeValue );
  else if( d->mSamplesInBuffer && !IsNaN( d->mSampleRate ) )
    timeValue = Floor( OneSecond_REFERENCE_TIME / d->mSampleRate.Hertz() * d->mSamplesInBuffer + 0.5 );

  int flags = AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_NOPERSIST;
  HRESULT result = pClient->Initialize( mShareMode, flags, timeValue, timeValue, pFormat, nullptr );
  if( result == AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED )
  {
    UINT32 frames = 0;
    hr_fail = pClient->GetBufferSize( &frames );
    timeValue = OneSecond_REFERENCE_TIME * frames / pFormat->nSamplesPerSec + 0.5;
    hr_fail = pDevice->Activate( IID_IAudioClient, sComCtx, nullptr, pClient.Assignee() );
    hr_fail = pClient->Initialize( mShareMode, flags, timeValue, timeValue, pFormat, nullptr );
  }
  else if( result == AUDCLNT_E_DEVICE_IN_USE )
  {
    if( mShareMode == AUDCLNT_SHAREMODE_EXCLUSIVE )
      d->mError.Mutable().Ref() << "Device is being used, cannot open in exclusive mode";
    else if( mShareMode == AUDCLNT_SHAREMODE_SHARED )
      d->mError.Mutable().Ref() << "Device is being used by another program in exclusive mode, cannot open";
  }
  hr_fail = result;

  hr_fail = pClient->GetDevicePeriod( &timeValue, nullptr );
  UINT32 deviceBufferSize = 0;
  hr_fail = pClient->GetBufferSize( &deviceBufferSize );
  d->mSampleRate = Rate::FromHertz( pFormat->nSamplesPerSec );
  d->mPlaybackChannels = pFormat->nChannels;
  if( mShareMode == AUDCLNT_SHAREMODE_SHARED )
    mSamplesInDeviceBuffer = Floor( timeValue / OneSecond_REFERENCE_TIME * pFormat->nSamplesPerSec + 0.5 );
  else if( mShareMode == AUDCLNT_SHAREMODE_EXCLUSIVE )
  {
    if( d->mSamplesInBuffer > deviceBufferSize )
    {
      d->mError.Mutable().Ref() << "Requested buffer size is " << d->mSamplesInBuffer
        << " samples, exceeds hardware buffer size (" << deviceBufferSize << " samples).\n"
        << "This case is not implemented, and unlikely to improve performance.";
      hr_fail = E_INVALIDARG;
    }
    mSamplesInDeviceBuffer = deviceBufferSize;
  }
  hr_fail = pClient->GetStreamLatency( &timeValue );
  d->mPlaybackLatency = Time::Seconds( timeValue / OneSecond_REFERENCE_TIME );
  Time::Interval minLatency = int32_t( mSamplesInDeviceBuffer ) / d->mSampleRate;
  if( mShareMode == AUDCLNT_SHAREMODE_SHARED )
    minLatency *= 2;
  if( d->mPlaybackLatency < minLatency )
    d->mPlaybackLatency = minLatency;

  return pClient;
}

int
AudioStream::Data::WasapiThread::OnExecute()
{
  if(!Thread::Notify(WhenTerminating, &mOnTerminate))
    SuggestDebugging << "Could not register termination notification";
  int err = 0;
  com::Ptr<IAudioClient> pClient;
  try
  {
    UsingCom usingCom( COINIT_APARTMENTTHREADED );
    pClient = Initialize();
    EventLoop( pClient );
  }
  catch( Win32Fail f )
  {
    err = f.Error();
  }
  if( err && d->mError->empty() )
    d->mError.Mutable().Ref() << WasapiErrorString( err );
  if( err && pClient )
    d->mpSelf->Emit( AudioStream::OnAbort, d->mpSelf );
  d->mState = Stopped;
  return err;
}

void
AudioStream::Data::WasapiThread::EventLoop( com::Ptr<IAudioClient> pClient )
{
  NullFail<HANDLE> h = ::CreateEventExA( nullptr, nullptr, 0, EVENT_ALL_ACCESS );
  auto_HANDLE bufferDone = h;
  HR_Fail hr_fail = pClient->SetEventHandle( bufferDone );
  d->mSamplesInBuffer = mSamplesInDeviceBuffer;
  mInitialized.Set();

  WithThreadTask( "Audio", ThreadUtils::Priority::Maximum )
  while( mStart.Wait() )
  {
    ::ResetEvent( mStop );

    WithLocked(pDacTick = d->mDacTick.Mutable())
    {
      pDacTick->count = -1;
      pDacTick->rate = d->mSampleRate * Ratio( 1, d->mSamplesInBuffer );
    }
    int bufferDurationMs = d->mDacTick->rate.Period().ToIntMilliseconds();
    int samplesInBuffer = d->mSamplesInBuffer;

    com::Ptr<IAudioRenderClient> pRenderClient;
    hr_fail = pClient->GetService( IID_IAudioRenderClient, pRenderClient.Assignee() );
    BYTE* pDeviceBufferData;
    hr_fail = pRenderClient->GetBuffer( samplesInBuffer, &pDeviceBufferData );
    hr_fail = pRenderClient->ReleaseBuffer( samplesInBuffer, AUDCLNT_BUFFERFLAGS_SILENT );

    Wasapi::AudioClock deviceClock( pClient, mShareMode, d->mSampleRate );
    hr_fail = pClient->Start();
    ClockTick sampleTick = deviceClock.Now();
    d->mCallbackTimedOut = false;
    int latencySamples = Round( d->mPlaybackLatency * sampleTick.rate );

    d->mState = Data::Active;
    while( WAIT_TIMEOUT == ::WaitForSingleObject( mStop, 0 ) )
    {
      Assert( d->mState == Active );
      d->mpWriteTo = nullptr;
      d->mRemainingToWrite = 0;
      HANDLE handles[] = { bufferDone, mStop };
      int waitResult = ::WaitForMultipleObjects( 2, handles, false, INFINITE );
      if( waitResult == WAIT_OBJECT_0 )
      {
        sampleTick = deviceClock.Now();
        WithLocked( p = d->mDacTick.Mutable() )
        {
          int64_t count = sampleTick.count / samplesInBuffer; // number of buffers since device start
          d->mCallbackTimedOut = ( count > p->count + 1 );
          p->count = count;
          int samplesIntoBuffer = sampleTick.count - count * samplesInBuffer; // number of samples into current buffer
          p->time = sampleTick.time + ( samplesInBuffer - samplesIntoBuffer + latencySamples ) * sampleTick.rate.Period();
        }
        HRESULT hr = pRenderClient->GetBuffer( samplesInBuffer, &pDeviceBufferData );
        while( WAIT_TIMEOUT == ::WaitForSingleObject( mStop, 0 ) && hr == AUDCLNT_E_BUFFER_TOO_LARGE )
        {
          ThreadUtils::Idle();
          hr = pRenderClient->GetBuffer( samplesInBuffer, &pDeviceBufferData );
        }
        hr_fail = hr;
        d->mpWriteTo = pDeviceBufferData;
        d->mRemainingToWrite = samplesInBuffer;
        d->mpSelf->Emit( OnTick, d->mpSelf );
        hr_fail = pRenderClient->ReleaseBuffer( samplesInBuffer, 0 );
      }
    }
    pClient->Stop();
    d->mState = Data::Stopped;
  }
  d->mpWriteTo = nullptr;
  d->mRemainingToWrite = 0;
}

#endif // _WIN32

AudioStream::Data::Data( AudioStream* p )
: mpSelf( p ),
  mDeviceIndex( -1 ),
  mpPaStream ( 0 ),
  mpWriteTo( 0 ),
  mRemainingToWrite( 0 ),
  mpReadFrom( 0 ),
  mRemainingToRead( 0 ),
  mSampleRate( 0, 0 ),
  mReadFunc( &Data::ReadFromFloat32 ),
  mWriteFunc( &Data::WriteToFloat32 ),
  mError( String( true ) )
{
  sAudioInit();

  ClockTick tick = ClockTick::Null;
  tick.time = Time::Never;
  tick.type = ClockType::Audio | ClockType::Hardware | ClockType::RealTime;
  *mDacTick.Mutable() = tick;
  *mAdcTick.Mutable() = tick;
#if _WIN32
  mWasapiThread.d = this;
  mWasapiThread.mDeviceId = nullptr;
#endif
}

int
AudioStream::Data::PaStreamCallback(
  const float* pIn, float* pOut, unsigned long samples,
  const PaStreamCallbackTimeInfo* pTime, PaStreamCallbackFlags f, Data* d )
{
  PaTime paNow = ::Pa_GetStreamTime( d->mpPaStream );
#if _WIN32 // we know stream time is derived from HPC, just as Time::MonotonicTime()
  Time now = Time( 0 ) + Time::Seconds( paNow );
#else
  Time now = TimeUtils::MonotonicTime();
#endif
  SyncMemory<>();
  if( d->mState.Atomic().IfEqual( Starting ).Exchange( Active ) == Starting )
  {
    d->mCallbackTimedOut = false;
    Debugging::SetCurrentThreadName( "Pa Callback Thread" );
  }
  else if( d->mState.Atomic().IfEqual( Stopping ).Exchange( Stopped ) == Stopping )
  {
    d->mCallbackTimedOut = false;
    Debugging::SetCurrentThreadName( "" );
  }
  SyncMemory<BeforeRead>();
  if( d->mState != Active )
    return paAbort;

  d->mpReadFrom = pIn;
  d->mRemainingToRead = pIn ? samples : 0;
  d->mpWriteTo = pOut;
  d->mRemainingToWrite = pOut ? samples : 0;

  if( pIn )
  {
    Time nextTime = now + Time::Seconds( pTime->inputBufferAdcTime - paNow );
    d->mCallbackTimedOut = d->mCallbackTimedOut || (f & paInputOverflow);
    WithLocked( p = d->mAdcTick.Mutable() )
    {
      if( d->mCallbackTimedOut )
        p->count += Ceil( ( nextTime - p->time ) * d->mSampleRate );
      else
        ++p->count;
      p->time = nextTime;
    }
  }
  if( pOut )
  {
    Time nextTime = now + Time::Seconds( pTime->outputBufferDacTime - paNow );
    d->mCallbackTimedOut = d->mCallbackTimedOut || (f & paOutputUnderflow);
    WithLocked( p = d->mDacTick.Mutable() )
    {
      if( d->mCallbackTimedOut )
        p->count += Ceil( ( nextTime - p->time ) * d->mSampleRate );
      else
        ++p->count;
      p->time = nextTime;
    }
  }
  SyncMemory<AfterWrite>();
#if TINY_DEBUG
  ExceptionCatcher().Run(
    MemberCall<int(Broadcaster*, unsigned short, void*)>
    ( &Broadcaster::Emit, d->mpSelf, OnTick, d->mpSelf )
  );
#else
  d->mpSelf->Emit( OnTick, d->mpSelf );
#endif
  d->mpReadFrom = 0;
  d->mRemainingToRead = 0;
  d->mpWriteTo = 0;
  d->mRemainingToWrite = 0;
  SyncMemory<AfterWrite>();
  return paContinue;
}

bool
AudioStream::Data::CreatePaStream()
{
  mWriteFunc = &WriteToFloat32;
  mReadFunc = &ReadFromFloat32;
  PaDeviceIndex paIndex = DeviceIndexToPaDeviceIndex( mDeviceIndex );
  if( paIndex == paNoDevice )
  {
    int index = mDeviceIndex;
    if( mPlaybackChannels > 0 )
      NativeDefaultDevices( nullptr, &index ) || PortAudioDefaultDevices( nullptr, &index );
    else if( mRecordingChannels > 0 )
      NativeDefaultDevices( &index, nullptr ) || PortAudioDefaultDevices( &index, nullptr );
    paIndex = DeviceIndexToPaDeviceIndex( index );
  }
  if( paIndex == paNoDevice )
  {
    const char* kind = mPlaybackChannels > 0 ? "playback" : "recording";
    if( mPlaybackChannels > 0 )
      mError.Mutable().Ref() << "No " << kind << " device available. Make sure your device's output is plugged in.";
    return false;
  }

  PaStreamParameters input = {};
  input.device = paIndex;
  const PaDeviceInfo* pInputInfo = Pa_GetDeviceInfo( input.device );
  const PaHostApiInfo* pInputApi = pInputInfo ? Pa_GetHostApiInfo( pInputInfo->hostApi ) : 0;
  input.channelCount = mRecordingChannels;
  input.sampleFormat = paFloat32;
  input.suggestedLatency = mRecordingLatency.Seconds();
  if( input.suggestedLatency < 0 )
    input.suggestedLatency = pInputInfo ? pInputInfo->defaultLowInputLatency : 0;
  PaStreamParameters output = input;
  output.device = paIndex;
  const PaDeviceInfo* pOutputInfo = Pa_GetDeviceInfo( output.device );
  const PaHostApiInfo* pOutputApi = pOutputInfo ? Pa_GetHostApiInfo( pOutputInfo->hostApi ) : 0;
  output.suggestedLatency = mPlaybackLatency.Seconds();
  if( output.suggestedLatency < 0 )
    output.suggestedLatency = pOutputInfo ? pOutputInfo->defaultLowOutputLatency : 0;
  output.channelCount = mPlaybackChannels;
  const char* pInputName = pInputInfo ? pInputInfo->name : "unnamed device",
              *pOutputName = pOutputInfo ? pOutputInfo->name : "unnamed device",
              *pApiName = 0;
  const PaDeviceInfo* pDevice = 0;
  PaStreamParameters* pInput = 0, *pOutput = 0;
  if( input.channelCount > 0 )
  {
    pInput = &input;
    mDeviceInfo = pInputName;
    pDevice = pInputInfo;
    pApiName = pInputApi ? pInputApi->name : 0;
  }
  if( output.channelCount > 0 )
  {
    pOutput = &output;
    if( !pInput )
      mDeviceInfo = pOutputName;
    else if( mDeviceInfo != pOutputName )
      mDeviceInfo.append( "/" ).append( pOutputName );
    pDevice = pOutputInfo;
    if( !pApiName )
      pApiName = pOutputApi ? pOutputApi->name : 0;
  }
  if( pApiName )
    mDeviceInfo.append( " [PortAudio " ).append( pApiName ).append( "]" );
  mError.Mutable()->clear();
  if( IsNaN( mSampleRate ) && pDevice )
    mSampleRate = Rate( pDevice->defaultSampleRate, Time::OneSecond );

  double sampleRate = mSampleRate.Hertz();
  PaError err = ::Pa_IsFormatSupported( pInput, pOutput, sampleRate );
  if( err )
  {
    mError.Mutable().Ref() << PortAudioError( err );
    if( mError->empty() )
    {
      if( pInput && pInput->channelCount > pInputInfo->maxInputChannels )
        mError.Mutable().Ref() << "Requested format exceeds number of available recording channels in device " << mDeviceInfo;
      if( pOutput && pOutput->channelCount > pOutputInfo->maxOutputChannels )
        mError.Mutable().Ref() << "Requested format exceeds number of available playback channels in device " << mDeviceInfo;
    }
  }
  PaStream* pStream = 0;
  mpSelf->Broadcaster::SetEnabled( false );
  if( mError->empty() )
  {
    mAdcTick.Mutable()->rate = mSampleRate / Ratio::From<int>( mSamplesInBuffer );
    mDacTick.Mutable()->rate = mAdcTick->rate;

    ::PaStreamCallback* callback = reinterpret_cast<::PaStreamCallback*>( &Data::PaStreamCallback );
    SyncMemory<AfterWrite>();
    err = ::Pa_OpenStream( &pStream, pInput, pOutput, sampleRate, mSamplesInBuffer, 0, callback, this );
    if( err == paInvalidDevice && pDevice ) // work around buggy host api wrappers
      err = paSampleFormatNotSupported;
    if( err )
      mError.Mutable().Ref() << PortAudioError( err );
  }
  if( mError->empty() )
  {
    mpPaStream = pStream;
    const PaStreamInfo* pInfo = Pa_GetStreamInfo( mpPaStream );
    mRecordingLatency = Time::Seconds( pInfo ? pInfo->inputLatency : 0 );
    mPlaybackLatency = Time::Seconds( pInfo ? pInfo->outputLatency : 0 );
    if( pInfo )
      mSampleRate = Rate( pInfo->sampleRate, Time::OneSecond );
  }
  mpSelf->Broadcaster::SetEnabled( true );
  SyncMemory<AfterWrite>();
  return mError->empty();
}

bool
AudioStream::Data::TryChange()
{
  if( mState != Stopped && mState != Destroyed )
  {
    mError.Mutable().Ref() << "Cannot change audio stream properties while running, or being created";
    return false;
  }
  mpSelf->Destroy();
  return true;
}

AudioStream::AudioStream( AudioDevice& inDevice )
: d( new Data( this ) )
{
  PaDeviceIndex paDevice = paNoDevice;
  if( inDevice.Exists() )
  {
    paDevice = GetPaDeviceIndex( (*sDevices)->at( inDevice.mIndex ) );
    d->mDeviceIndex = paDevice;
    if( paDevice == paNoDevice )
      d->mError.Mutable().Ref() << "Audio device is temporarily unavailable (no cable connected?)";
  }
  const PaDeviceInfo* pInfo = ::Pa_GetDeviceInfo( paDevice );
  if( pInfo )
  {
    d->mPlaybackChannels = pInfo->maxOutputChannels;
    d->mRecordingChannels = pInfo->maxInputChannels;
    d->mSampleRate = Rate( pInfo->defaultSampleRate, Time::OneSecond );
    d->mRecordingLatency = Time::Seconds( pInfo->defaultLowInputLatency );
    d->mPlaybackLatency = Time::Seconds( pInfo->defaultLowOutputLatency );
    if( d->mPlaybackChannels == 0 )
      d->mSamplesInBuffer = Ceil( d->mRecordingLatency * d->mSampleRate );
    else
      d->mSamplesInBuffer = Ceil( d->mPlaybackLatency * d->mSampleRate );
  }
  else
  {
    d->mPlaybackChannels = 2;
    d->mRecordingChannels = 0;
    d->mSampleRate = Rate( 44100, Time::OneSecond );
    d->mSamplesInBuffer = paFramesPerBufferUnspecified;
    d->mRecordingLatency = Time::Seconds( -1 );
    d->mPlaybackLatency = Time::Seconds( -1 );
  }
}

AudioStream::~AudioStream()
{
  Stop();
  Destroy();
  delete d;
}

const String&
AudioStream::Error() const
{
  return *d->mError;
}

const String&
AudioStream::DeviceInfo() const
{
  SyncMemory<BeforeRead>();
  return d->mDeviceInfo;
}

int
AudioStream::SamplesInBuffer() const
{
  return d->mSamplesInBuffer;
}

AudioStream&
AudioStream::SetSamplesInBuffer( int inSamples )
{
  Assert( inSamples >= 0 );
  if( d->TryChange() )
    d->mSamplesInBuffer = std::max( 0, inSamples );
  return *this;
}

int
AudioStream::PlaybackChannels() const
{
  return d->mPlaybackChannels;
}

AudioStream&
AudioStream::SetPlaybackChannels( int inChannels )
{
  Assert( inChannels >= 0 );
  if( d->TryChange() )
    d->mPlaybackChannels = std::max( 0, inChannels );
  return *this;
}

int
AudioStream::RecordingChannels() const
{
  return d->mRecordingChannels;
}

AudioStream&
AudioStream::SetRecordingChannels( int inChannels )
{
  Assert( inChannels >= 0 );
  if( d->TryChange() )
    d->mRecordingChannels = std::max( 0, inChannels );
  return *this;
}

const Rate&
AudioStream::SampleRate() const
{
  SyncMemory<BeforeRead>();
  return d->mSampleRate;
}

AudioStream&
AudioStream::SetSampleRate( const Rate& inRate )
{
  if( inRate.Sign() < 0 )
    d->mError.Mutable().Ref() << "Invalid sampling rate";
  else if( d->TryChange() )
    d->mSampleRate = inRate;
  SyncMemory<AfterWrite>();
  return *this;
}

Time::Interval
AudioStream::RecordingLatency() const
{
  SyncMemory<BeforeRead>();
  return d->mRecordingLatency;
}

AudioStream&
AudioStream::SetRecordingLatency( Time::Interval t )
{
  if( d->TryChange() )
    d->mRecordingLatency = t;
  SyncMemory<AfterWrite>();
  return *this;
}

Time::Interval
AudioStream::PlaybackLatency() const
{
  SyncMemory<BeforeRead>();
  return d->mPlaybackLatency;
}

AudioStream&
AudioStream::SetPlaybackLatency( Time::Interval t )
{
  if( d->TryChange() )
    d->mPlaybackLatency = t;
  SyncMemory<AfterWrite>();
  return *this;
}

AudioStream&
AudioStream::Create()
{
  Destroy();
  d->mError.Mutable()->clear();
  if( d->mState.Atomic().IfEqual( Data::Destroyed ).Exchange( Data::Creating ) == Data::Destroyed )
  {
    bool done = false;
    if( AudioMode().mode == AudioMode::PortAudio )
      done = d->CreatePaStream();
#if _WIN32
    else if( AudioMode().mode == AudioMode::WasapiNative )
      done = d->CreateWasapiStream( AudioMode().details.wasapiShareMode );
#endif
    d->mState = done ? Data::Stopped : Data::Destroyed;
    if( !done && d->mError->empty() )
      d->mError.Mutable().Ref() << "Could not create audio stream";
  }
  return *this;
}

AudioStream&
AudioStream::Destroy()
{
  Stop();
  if( d->mState.Atomic().IfEqual( Data::Stopped ).Exchange( Data::Destroying ) == Data::Stopped )
  {
    if( d->mpPaStream )
    {
      ::Pa_CloseStream( d->mpPaStream );
      d->mpPaStream = nullptr;
    }
#if _WIN32
    d->mWasapiThread.Terminate();
    d->mWasapiThread.mDeviceId = nullptr;
#endif
    *d->mAdcTick.Mutable() = ClockTick::Null;
    *d->mDacTick.Mutable() = ClockTick::Null;
    d->mState = Data::Destroyed;
  }
  return *this;
}

AudioStream&
AudioStream::Start( Time::Interval inTimeout )
{
  Create();
  if( !Error() )
  {
    if( d->mState.Atomic().IfEqual( Data::Stopped ).Exchange( Data::Starting ) == Data::Stopped )
    {
      d->mError.Mutable()->clear();
      std::ostringstream error;
      if( d->mpPaStream )
      {
        PaError err = Pa_StartStream( d->mpPaStream );
        if( err )
          error << PortAudioError( err );
      }
#if _WIN32
      else if( d->mWasapiThread.Running() )
        d->mWasapiThread.mStart.Set();
#endif
      Time start = TimeUtils::MonotonicTime();
      while( d->mState == Data::Starting && TimeUtils::MonotonicTime() - start < inTimeout )
        ThreadUtils::Idle();
      if( d->mState == Data::Starting )
        error << "Start() timed out after " << inTimeout.Seconds() << "s";
      d->mError.Mutable().Ref() << error.str();
      d->mState.Atomic().IfEqual( Data::Starting ).Exchange( Data::Stopped );
    }
  }
  return *this;
}

AudioStream&
AudioStream::Stop()
{
  if( d->mState.Atomic().IfEqual( Data::Active ).Exchange( Data::Stopping ) == Data::Active )
  {
    if( d->mpPaStream )
    {
      int count = 500;
      while( --count > 0 && Pa_IsStreamActive( d->mpPaStream ) )
        ThreadUtils::Idle();
      PaError err = (count > 0) ? Pa_StopStream( d->mpPaStream ) : Pa_AbortStream( d->mpPaStream );
      if( err )
        d->mError.Mutable().Ref() << PortAudioError( err );
    }
#if _WIN32
    else
    {
      if( d->mWasapiThread.Running() )
      {
        ::SetEvent( d->mWasapiThread.mStop );
        int count = 500;
        while( --count > 0 && d->mState == Data::Stopping )
          ThreadUtils::Idle();
        if( d->mState.Atomic().IfEqual( Data::Stopping ).Exchange( Data::Stopped ) == Data::Stopping )
          d->mError.Mutable().Ref() << "Stop() timed out";
      }
    }
#endif
    d->mState.Atomic().IfEqual( Data::Stopping ).Exchange( Data::Stopped );
  }
  return *this;
}

bool
AudioStream::Active() const
{
  return d->mState == Data::Active;
}

bool
AudioStream::OnTickTimedOut() const
{
  return d->mCallbackTimedOut;
}

ClockTick
AudioStream::RecordingTick() const
{
  return *d->mAdcTick;
}

ClockTick
AudioStream::PlaybackTick() const
{
  return *d->mDacTick;
}

int
AudioStream::Read( float* outData, int inSamples )
{
  int count = inSamples;
  SyncMemory<BeforeRead>();
  if( count > d->mRemainingToRead )
    count = d->mRemainingToRead;
  if( count < 0 )
    count = 0;
  if( count > 0 )
  {
    int valueCount = count * d->mRecordingChannels;
    d->mReadFunc( outData, d->mpReadFrom, valueCount );
    d->mRemainingToRead -= count;
  }
  SyncMemory<AfterWrite>();
  return count;
}

int
AudioStream::Write( const float* inData, int inSamples )
{
  int count = inSamples;
  SyncMemory<BeforeRead>();
  if( count > d->mRemainingToWrite )
    count = d->mRemainingToWrite;
  if( count < 0 )
    count = 0;
  if( count > 0 )
  {
    int valueCount = count * d->mPlaybackChannels;
    d->mWriteFunc( d->mpWriteTo, inData, valueCount );
    d->mRemainingToWrite -= count;
  }
  SyncMemory<AfterWrite>();
  return count;
}

void
AudioStream::Data::ReadFromFloat32( float* to, const void*& from, size_t count )
{
  ::memcpy( to, from, count * sizeof(float) );
  from = static_cast<const float*>( from ) + count;
}

void
AudioStream::Data::WriteToFloat32( void*& to, const float* from, size_t count )
{
  ::memcpy( to, from, count * sizeof(float) );
  to = static_cast<float*>( to ) + count;
}

void
AudioStream::Data::WriteToInt16( void*& to, const float* from, size_t count )
{
  int16_t* pOut = static_cast<int16_t*>( to );
  for( size_t i = 0; i < count; ++i )
    *pOut++ = from[i] * (1<<15) - 0.5;
  to = pOut;
}

void
AudioStream::Data::WriteToInt32( void*& to, const float* from, size_t count )
{
  int32_t* pOut = static_cast<int32_t*>( to );
  for( size_t i = 0; i < count; ++i )
    *pOut++ = from[i] * (1<<31) - 0.5;
  to = pOut;
}
