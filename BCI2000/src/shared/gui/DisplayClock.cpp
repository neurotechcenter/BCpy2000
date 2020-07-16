////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: An class that manages access to display timing information,
//   and vblank synchronization.
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
#include "DisplayClock.h"
#include "StringUtils.h"
#include "EnvVariable.h"
#include "ThreadUtils.h"
#include "Debugging.h"

#if _WIN32
# include <Windows.h>
#endif

namespace {

struct PlatformClock
{
  PlatformClock() : mTick( ClockTick::Null )
  {
    mTick.type = ClockType::RealTime | ClockType::Video;
  }
  virtual ~PlatformClock() {}
  virtual bool Init( void* ) = 0;
  virtual bool GetLastVbl( Time& t ) const = 0;
  virtual bool WaitForVbl() const = 0;

  ClockTick mTick;
};

struct SoftwareClock : PlatformClock
{
  SoftwareClock( const Rate& rate )
  {
    mTick.rate = rate;
  }
  bool Init( void* ) override 
  {
    mTick.time = TimeUtils::MonotonicTime();
    return true;
  }
  bool GetLastVbl( Time& t ) const override
  {
    Time now = TimeUtils::MonotonicTime();
    t = mTick.time + mTick.rate.Period() * Floor( (now - mTick.time) * mTick.rate );
    return true;
  }
  bool WaitForVbl() const override
  {
    const int sleepsPerFrame = 4;
    Time vbl;
    GetLastVbl( vbl );
    Time nextVbl = vbl;
    while( nextVbl == vbl )
    {
      ThreadUtils::SleepFor( mTick.rate.Period() / sleepsPerFrame );
      GetLastVbl( nextVbl );
    }
    return true;
  }
};

#if _WIN32
// D3dkmthk.h
typedef UINT D3DKMT_HANDLE;
typedef UINT D3DDDI_VIDEO_PRESENT_SOURCE_ID;

NTSTATUS (APIENTRY *D3DKMTOpenAdapterFromHdc)( void* );
NTSTATUS (APIENTRY *D3DKMTCloseAdapter)( const void* );
NTSTATUS (APIENTRY *D3DKMTQueryAdapterInfo)( const void* );
NTSTATUS (APIENTRY *D3DKMTGetScanLine)( void* );
NTSTATUS (APIENTRY *D3DKMTWaitForVerticalBlankEvent)( const void* );

bool ImportD3DKMT()
{
  HMODULE dll = ::LoadLibraryA( "Gdi32.dll" );
  bool ok = dll;
#define _(x) ok = ok && ( *(void**)&x = ::GetProcAddress( dll, #x ) );
  _( D3DKMTOpenAdapterFromHdc )
  _( D3DKMTCloseAdapter )
  _( D3DKMTQueryAdapterInfo )
  _( D3DKMTGetScanLine )
  _( D3DKMTWaitForVerticalBlankEvent )
#undef _
  return ok;
}
bool sHaveD3DKMT = ImportD3DKMT();

struct Win32HardwareClock : PlatformClock
{
  int mScanlines;
  D3DKMT_HANDLE mAdapter;
  D3DDDI_VIDEO_PRESENT_SOURCE_ID mSourceId;

  Win32HardwareClock()
  : mScanlines( 0 ), mAdapter( NULL ), mSourceId( -1 )
  {
  }
  ~Win32HardwareClock()
  {
    if( mAdapter )
      D3DKMTCloseAdapter( &mAdapter );
  }
  bool Init( void* inWindow ) override
  {
    Assert( mAdapter == NULL );
    if( ::GetSystemMetrics( SM_REMOTESESSION ) )
      return false;

    if( !sHaveD3DKMT )
      return false;

    HMONITOR monitor = ::MonitorFromWindow( HWND( inWindow ), MONITOR_DEFAULTTONULL );
    MONITORINFOEXW info;
    info.cbSize = sizeof(info);
    if( !::GetMonitorInfoW( monitor, &info ) )
      return false;

    DEVMODEW mode = { 0 };
    mode.dmSize = sizeof(DEVMODEW);
    if( !::EnumDisplaySettingsW( info.szDevice, ENUM_CURRENT_SETTINGS, &mode ) )
      return false;
    if( mode.dmFields & DM_DISPLAYFREQUENCY )
      mTick.rate = Rate( int( mode.dmDisplayFrequency ), Time::OneSecond );
    else
      return false;
    mTick.type |= ClockType::Hardware;

    switch( mode.dmOrientation )
    {
      case DMORIENT_PORTRAIT:
        mScanlines = mode.dmPelsWidth;
        break;
      case DMORIENT_LANDSCAPE:
        mScanlines = mode.dmPelsHeight;
        break;
      default:
        mScanlines = std::min( mode.dmPelsHeight, mode.dmPelsWidth );
    }
    if( mScanlines < 1 )
      return false;

    HDC dc = ::CreateDCW( nullptr, info.szDevice, nullptr, nullptr );
    if( !dc )
      return false;
    struct
    {
      HDC hDc;
      D3DKMT_HANDLE hAdapter;
      LUID AdapterLuid;
      D3DDDI_VIDEO_PRESENT_SOURCE_ID VidPnSourceId;
    } args = { dc };
    NTSTATUS status = D3DKMTOpenAdapterFromHdc( &args );
    if( !SUCCEEDED( status ) )
    {
      SuggestDebugging << "D3DKMTOpenAdapterFromHdc: 0x" << std::hex << status;
    }
    else
    {
      mAdapter = args.hAdapter;
      mSourceId = args.VidPnSourceId;
    }
    ::DeleteDC( dc );
    return mAdapter;
  }
  bool GetLastVbl( Time& t ) const override
  {
    struct
    { D3DKMT_HANDLE hAdapter;
      D3DDDI_VIDEO_PRESENT_SOURCE_ID VidPnSourceId;
      BOOLEAN InVerticalBlank;
      UINT ScanLine;
    } args = { mAdapter, mSourceId };
    NTSTATUS status = D3DKMTGetScanLine( &args );
    Time now = TimeUtils::MonotonicTime();
    if( args.InVerticalBlank )
      args.ScanLine = 0;
    t = now - mTick.rate.Period() * Ratio( args.ScanLine, mScanlines );
    return SUCCEEDED( status );
  }

  bool WaitForVbl() const override
  {
    struct
    { D3DKMT_HANDLE hAdapter;
      D3DKMT_HANDLE hDevice;
      D3DDDI_VIDEO_PRESENT_SOURCE_ID VidPnSourceId;
    } args = { mAdapter, NULL, mSourceId };
    NTSTATUS status = D3DKMTWaitForVerticalBlankEvent( &args );
    return SUCCEEDED( status );
  }
};

#endif // _WIN32

} // namespace

namespace AV {

struct DisplayClock::DisplayClockPrivate
{
  PlatformClock* mpClock;
  DisplayClockPrivate() : mpClock( nullptr ) {}
  ~DisplayClockPrivate() { delete mpClock; }
};

// DisplayClock
DisplayClock::DisplayClock( void* inWindow )
: p( new DisplayClockPrivate )
{
  PlatformClock* pClock = 0;
  std::string type;
  EnvVariable::Get( "AV::DisplayClock::Type", type );
  type = StringUtils::ToLower( type );
#define TRY(x,y) { if( !x ) { x = new y; if( !x->Init( inWindow ) ) { delete x; x = 0; } } }
  if( type.find( "soft" ) == 0 )
  {
    float rate = ::atof( type.c_str() + 4 );
    if( rate <= 0 )
      rate = 15;
    TRY( pClock, SoftwareClock( Rate::FromHertz( rate ) ) );
  }
  else if( type.find( "hard" ) == 0 )
  {
#if _WIN32
    TRY( pClock, Win32HardwareClock );
#endif
  }
  else if( type.empty() )
  {
#if _WIN32
    TRY( pClock, Win32HardwareClock );
#else
    TRY( pClock, SoftwareClock( Rate::FromHertz( 30 ) ) );
#endif
    TRY( pClock, SoftwareClock( Rate::FromHertz( 15 ) ) ); // assume slow (e.g. remote) display
  }
  if( !pClock && !type.empty() )
    throw std_runtime_error << "Cannot use clock type: " << type;
  p->mpClock = pClock;
  Assert( p->mpClock );
}

DisplayClock::~DisplayClock()
{
  delete p;
}

ClockTick
DisplayClock::LastVbl() const
{
  ClockTick t = p->mpClock->mTick;
  if( !p->mpClock->GetLastVbl( t.time ) )
    SuggestDebugging << SysError().Message();
  return t;
}

const DisplayClock&
DisplayClock::WaitForVbl() const
{
  if( !p->mpClock->WaitForVbl() )
    SuggestDebugging << SysError().Message();
  return *this;
}

} // namespace AV

