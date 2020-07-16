/////////////////////////////////////////////////////////////////////////////
// $Id: TobiiNamespace.h 4536 2013-08-05 14:30:13Z mellinger $
// Authors: juergen.mellinger@neurotechcenter.org, kaleb.goering@gmail.com
// Description: It holds the namespace for tobii to use the tobii sdk 
//   without leveraging boost
//
// (C) 2000-2015, BCI2000 Project
// http://www.bci2000.org
/////////////////////////////////////////////////////////////////////////////
#ifndef TOBII_NAMESPACE_H
#define TOBII_NAMESPACE_H
#include <Windows.h>
#include <stdint.h>

#define TOBIISDK_API __declspec(dllimport)
#define TOBIISDK_CALL __cdecl
#define TOBIISDK_NONNULL(...)


#include "core.imports.h"
#include "core/discovery.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cassert>

namespace Tobii
{

class Error
{
public:
  Error() : mp( 0 ) {}
  bool IsFailure() const { return mp ? ::tobii_sdk_error_is_failure ( mp ) : false; }
  operator bool() const { return IsFailure(); }
  std::string Message() const { return mp ? ::tobii_sdk_error_to_string ( mp ) : ""; }
  struct Accessor
  {
    explicit Accessor( Error* p ) : p( p ) { p->mp = 0; }
    ~Accessor() { p->Report(); }
    operator tobii_sdk_error_t**() { return &p->mp; }
    Error* p;
  };
  Accessor operator&() { return Accessor( this ); }
private:
  void Report() const
  {
#if 1
    if( IsFailure() )
    {
      std::string s = "\nOperation failed: " + Message() + "\n";
      ::OutputDebugStringA( s.c_str() );
      ::DebugBreak();
    }
#endif
  };
  friend struct Accessor;
  tobii_sdk_error_t* mp;
};

class Mainloop
{
public:
  operator tobii_sdk_mainloop_t*() { return mp; }
  const Error& Error() const { return mError; }
  static Mainloop& Instance() { static Mainloop instance; return instance; }

private:
  Mainloop( const Mainloop& );
  Mainloop& operator=( const Mainloop& );

  Mainloop() : mp( 0 ), hEvent( 0 )
  {
    ::tobii_sdk_init( &mError );
    mp = ::tobii_sdk_mainloop_create( &mError );
    hEvent = ::CreateEvent( 0, 0, 0, 0 );
    if( !::CreateThread( 0, 0, &Thread, this, 0, 0 ) )
      std::cerr << "Could not create thread" << std::endl;
  }
  ~Mainloop()
  {
    ::tobii_sdk_mainloop_quit( mp, &mError );
    ::WaitForSingleObject( hEvent, -1 );
    ::CloseHandle( hEvent );
    ::tobii_sdk_mainloop_destroy( mp );
  }

  static DWORD __stdcall Thread( void* p )
  {
    Mainloop* pData = reinterpret_cast<Mainloop*>( p );
    ::tobii_sdk_mainloop_run( pData->mp, &pData->mError );
    ::SetEvent( pData->hEvent );
    return 0;
  }
  tobii_sdk_mainloop_t* mp;
  class Error mError;
  HANDLE hEvent;
};

struct Blob : std::string {};
struct Point2d { double x; double y; };
struct Point3d { double x; double y; double z; };

class Arguments
{
  Arguments( const Arguments& );
  Arguments& operator=( const Arguments& );
public:
  Arguments() : mpIn( ::tobii_sdk_param_stack_create( &mError ) ), mpOut( 0 ), mPos( 0 ) {}
  Arguments( const tobii_sdk_param_stack_t* p ) : mpIn( 0 ), mpOut( p ), mPos( 0 ) {}
  ~Arguments() { AssignFrom( 0 ); }
  const tobii_sdk_param_stack_t* In() const { return mpIn; }
  bool AssignFrom( const tobii_sdk_param_stack_t* p )
  {
    if( mpIn )
       ::tobii_sdk_param_stack_destroy( mpIn );
    if( p )
      mpIn = ::tobii_sdk_param_stack_clone( p, &mError );
    else
      mpIn = 0;
    mpOut = mpIn;
    mPos = 0;
    return !mError; 
  }

  Arguments& Put( uint32_t arg ) { ::tobii_sdk_param_stack_push_uint32( mpIn, arg, &mError ); return *this; }
  Arguments& Put( float arg ) { ::tobii_sdk_param_stack_push_float32_as_fixed_15x16( mpIn, arg, &mError ); return *this; }
  Arguments& Put( double arg ) { ::tobii_sdk_param_stack_push_float64_as_fixed_22x41( mpIn, arg, &mError ); return *this; }
  Arguments& Put( const char* arg, size_t len = 0 ) { if( !len ) len = ::strlen( arg ); ::tobii_sdk_param_stack_push_string( mpIn, arg, len, &mError ); return *this; }
  Arguments& Put( const std::string& arg ) { return Put( arg.c_str(), 0 ); }
  Arguments& Put( const Blob& arg ) { ::tobii_sdk_param_stack_push_blob( mpIn, reinterpret_cast<const unsigned char*>(arg.data()), arg.length(), &mError ); return *this; }
  Arguments& Put( const std::vector<uint32_t>& arg )
  {
    tobii_sdk_vector_t* p = ::tobii_sdk_vector_create( arg.size(), TOBII_SDK_PARAM_TYPE_UINT32, &mError );
    for( size_t i = 0; i < arg.size(); ++i )
      ::tobii_sdk_vector_set_uint32( p, i, arg[i], &mError );
    ::tobii_sdk_param_stack_push_vector( mpIn, p, &mError );
    ::tobii_sdk_vector_destroy( p );
    return *this;
  }

  struct NodeInfo { uint32_t type; uint16_t children; uint16_t id; };
  NodeInfo GetInfo()
  {
    NodeInfo info = { 0 };
    info.type = ::tobii_sdk_param_stack_get_type( mpOut, mPos, &mError );
    if( mError )
      info.type = TOBII_SDK_PARAM_TYPE_VOID;
    else if( info.type == TOBII_SDK_PARAM_TYPE_NODE_PROLOG )
    {
      uint32_t prolog = ::tobii_sdk_param_stack_get_node_prolog( mpOut, mPos, &mError );
      info.children = (prolog >> 16) & 0x0fff;
      info.id = prolog & 0xffff;
    }
    return info;
  }
  Arguments& Ignore( int n = 1 )
  {
    mPos += n;
    return *this;
  }

  Arguments& Get( int32_t& arg ) { arg = ::tobii_sdk_param_stack_get_int32( mpOut, mPos++, &mError ); return *this; }
  Arguments& Get( uint32_t& arg ) { arg = ::tobii_sdk_param_stack_get_uint32( mpOut, mPos++, &mError ); return *this; }
  Arguments& Get( int64_t& arg ) { arg = ::tobii_sdk_param_stack_get_int64( mpOut, mPos++, &mError ); return *this; }
  Arguments& Get( uint64_t& arg ) { arg = ::tobii_sdk_param_stack_get_uint64( mpOut, mPos++, &mError ); return *this; }
  Arguments& Get( float& arg ) { arg = ::tobii_sdk_param_stack_get_fixed_15x16_as_float32( mpOut, mPos++, &mError ); return *this; }
  Arguments& Get( double& arg ) { arg = ::tobii_sdk_param_stack_get_fixed_22x41_as_float64( mpOut, mPos++, &mError ); return *this; }
  Arguments& Get( std::string& arg )
  {
    const char* s = ::tobii_sdk_param_stack_get_string( mpOut, mPos++, &mError );
    if( s )
    {
      arg = s;
      ::tobii_sdk_free_string( s );
    }
    return *this;
  }
  Arguments& Get( Blob& arg )
  {
    const unsigned char* p = ::tobii_sdk_param_stack_get_blob( mpOut, mPos, &mError );
    uint32_t length = ::tobii_sdk_param_stack_get_blob_length( mpOut, mPos++, &mError);
    if( !p || !length )
      arg.clear();
    else
      arg.assign( reinterpret_cast<const char*>( p ), length );
    return *this;
  }
  Arguments& Get( std::vector<float>& arg )
  {
    tobii_sdk_vector_t* p = ::tobii_sdk_param_stack_get_vector( mpOut, mPos++, &mError );
    uint32_t size = ::tobii_sdk_vector_get_length( p, &mError );
    arg.resize( size );
    for( size_t i = 0; i < arg.size(); ++i )
      arg[i] = ::tobii_sdk_vector_get_fixed_15x16_as_float32( p, i, &mError );
    ::tobii_sdk_vector_destroy( p );
    return *this;
  }
  Arguments& Get( std::vector<std::string>& arg )
  {
    tobii_sdk_vector_t* p = ::tobii_sdk_param_stack_get_vector( mpOut, mPos++, &mError );
    uint32_t size = ::tobii_sdk_vector_get_length( p, &mError );
    arg.resize( size );
    for( size_t i = 0; i < arg.size(); ++i )
    {
      const char* s = ::tobii_sdk_vector_get_string( p, i, &mError );
      if( s )
      {
        arg[i] = s;
        ::tobii_sdk_free_string( s );
      }
    }
    ::tobii_sdk_vector_destroy( p );
    return *this;
  }
  Arguments& Get( Point2d& p ) { return Ignore().Get( p.x ).Get( p.y ); }
  Arguments& Get( Point3d& p ) { return Ignore().Get( p.x ).Get( p.y ).Get( p.z ); }

  int OutCount() const
  {
    uint32_t count = tobii_sdk_param_stack_get_size(mpOut, &mError);
    return mError ? -1 : count;
  }

  int InCount() const
  {
    uint32_t count = tobii_sdk_param_stack_get_size(mpIn, &mError);
    return mError ? -1 : count;
  }

private:
  mutable class Error mError;
  tobii_sdk_param_stack_t* mpIn;
  const tobii_sdk_param_stack_t* mpOut;
  uint32_t mPos;
};

struct TrackBox
{
  void Parse(Arguments& args);
};

struct GazeData
{
  enum
  {
     ROW_ID = 3000,
     COLUMN_ID = 3001,
  };

  int64_t timestamp;
  uint32_t triggerSignal;
  struct EyeData
  {
    uint32_t validity;
    Point3d eyePosition3d,
            eyePosition3dRelative,
            gazePoint3d;
    Point2d gazePoint2d;
    float pupilDiameter;
  } left, right;


  GazeData()
  {
    ::memset(this, 0, sizeof(*this));
  }
  GazeData( Arguments& args )
  {
    Parse( args );
  }
  void Parse( Arguments& args )
  {
    Arguments::NodeInfo level1 = args.GetInfo();
    args.Ignore();
    for( uint32_t i = 0; i < level1.children; ++i )
    {
      Arguments::NodeInfo level2 = args.GetInfo();
      args.Ignore();
      if( level1.id == ROW_ID && level2.id == COLUMN_ID && level2.children == 2 )
      {
        uint32_t column = 0;
        args.Get( column );
        switch( column )
        {
          #define FIELD( X, Y ) case Y: args.Get( X ); break;
          FIELD( timestamp, 1 )
          FIELD( left.eyePosition3d, 2 )
          FIELD( left.eyePosition3dRelative, 3 )
          FIELD( left.gazePoint3d, 4 )
          FIELD( left.gazePoint2d, 5 )
          FIELD( left.pupilDiameter, 6 )
          FIELD( left.validity, 7 )
          FIELD( right.eyePosition3d, 8 )
          FIELD( right.eyePosition3dRelative, 9 )
          FIELD( right.gazePoint3d, 10 )
          FIELD( right.gazePoint2d, 11 )
          FIELD( right.pupilDiameter, 12 )
          FIELD( right.validity, 13 )
          FIELD( triggerSignal, 14 )
          #undef FIELD
          default:
            Parse( args );
        }
      }
      else
        Parse( args );
    }
  }
};


enum
{
  // callbacks
  OnFrameRateChanged = 1640,
  OnCalibrationStarted = 1040,
  OnCalibrationStopped = 1050,
  OnGazeDataReceived = 1280,
  OnTrackBoxChanged = 1410,
  OnXConfigChanged = 1450,
  // requests
  GetAuthorizeChallenge = 1900,
  ValidateChallengeResponse = 1910,
  EnumerateFrameRates = 1630,
  SetFrameRate = 1620,
  GetFrameRate = 1610,	
  GetLowBlinkMode = 1920,
  SetLowBlinkMode = 1930,
  DumpImages = 1500,
  GetDiagnosticsReport = 1510,
  SetUnitName = 1710,
  GetUnitName = 1700,
  GetUnitInfo = 1420,
  GetPayPerUseInfo = 1600,
  StartCalibration = 1010,
  StopCalibration = 1020,
  ClearCalibration = 1060,
  AddCalibrationPoint = 1030,
  RemoveCalibrationPoint = 1080,
  ComputeCalibration = 1070,
  GetCalibration = 1100,
  SetCalibration = 1110,
  StartTracking = 1220, GazeDataChannel = 1280,
  StopTracking = 1230,
  GetTrackBox = 1400,
  EnableExtension = 1800,
  GetAvailableExtensions = 1810,
  GetEnabledExtensions = 1820,
  SetXConfiguration = 1440,
  GetXConfiguration = 1430,
  GetIlluminationMode = 2010,
  SetIlluminationMode = 2020,
  EnumerateIlluminationModes = 2030,
  // Clock synchronization
  ClockStatusUnsynchronized = TOBII_SDK_SYNC_STATUS_UNSYNCHRONIZED,
  ClockStatusStabilizing = TOBII_SDK_SYNC_STATUS_STABILIZING,
  ClockStatusSynchronized = TOBII_SDK_SYNC_STATUS_SYNCHRONIZED,
};

// These are just for documentation purposes right now.
#define CALLBACKS \
_( OnFrameRateChanged, (float framerate) ) \
_( OnCalibrationStarted, () ) \
_( OnCalibrationStopped, () ) \
_( OnGazeDataReceived, (GazeData) ) \
_( OnTrackBoxChanged, () ) \
_( OnXConfigChanged, (Point3d upperLeft, Point3d upperRight, Point3d lowerLeft) ) \
//

struct ExtensionInfo { uint32_t protocolVersion; uint32_t extensionId; std::string name; uint32_t realm; };
#define REQUESTS \
_( GetAuthorizeChallenge, (uint32_t realmId, std::vector<uint32_t> algorithms), (uint32_t realmId, uint32_t algorithm, Blob challenge ) ) \
_( ValidateChallengeResponse, (uint32_t realmId, uint32_t algorithm, Blob challenge ), () ) \
_( EnumerateFrameRates, (), (std::vector<float>) ) \
_( SetFrameRate, (float framerate), () ) \
_( GetFrameRate, (), (float) ) \
_( GetLowBlinkMode, (), (uint32_t) ) \
_( SetLowBlinkMode, (uint32_t), () ) \
_( DumpImages, (uint32_t count, uint32_t frequency), () ) \
_( GetDiagnosticsReport, (uint32_t includeImages), (Blob) ) \
_( SetUnitName, (std::string), () ) \
_( GetUnitName, (), (std::string) ) \
_( GetUnitInfo, (), (std::string serialNumber, std::string model, std::string generation, std::string firmwareVersion ) ) \
_( GetPayPerUseInfo, (), (uint32_t enabled, uint32_t realm, uint32_t authorized ) ) \
_( StartCalibration, (), () ) \
_( StopCalibration, (), () ) \
_( ClearCalibration, (), () ) \
_( AddCalibrationPoint, (double x, double y, uint32_t = 3), () ) \
_( RemoveCalibrationPoint, (double x, double y, uint32_t = 3), () ) \
_( ComputeCalibration, (), () ) \
_( GetCalibration, (), (Blob) ) \
_( SetCalibration, (Blob), () ) \
_( StartTracking, (uint32_t = 1280, std::vector<uint32> = std::vector<uint32>() ), () ) \
_( StopTracking, (uint32_t = 1280), () ) \
_( GetTrackBox, (), (Point3d[8]) ) \
_( EnableExtension, (uint32_t extensionId), () ) \
_( GetAvailableExtensions, (), (std::vector<ExtensionInfo>) ) \
_( GetEnabledExtensions, (),  (std::vector<ExtensionInfo>) ) \
_( SetXConfiguration, (Point3d upperLeft, Point3d upperRight, Point3d lowerLeft, std::vector<ToolData> = {}), () ) \
_( GetXConfiguration, (), (Point3d upperLeft, Point3d upperRight, Point3d lowerLeft) ) \
_( GetIlluminationMode, (), (std::string) ) \
_( SetIlluminationMode, (std::string), () ) \
_( EnumerateIlluminationModes, (), (std::vector<std::string>) ) \
//

class Device
{
private:
  Device( const Device& );
  Device& operator=( const Device& );

public:
  struct Info : std::map<std::string, std::string> {};

  Device( tobii_sdk_device_info_t* p )
    : mpDevice(nullptr), mpAddress(nullptr), mpConnection(nullptr),
      mpSyncManager(nullptr), mSyncState(TOBII_SDK_SYNC_STATUS_UNSYNCHRONIZED),
      mpResult(nullptr)
  {
    mpDevice = ::tobii_sdk_device_info_clone( p, &mError );
    mpAddress = ::tobii_sdk_device_info_get_factory_info( p, &mError );
    const char* pValue = ::tobii_sdk_device_info_get_product_id( p, &mError );
    if( pValue )
    {
      mInfo.insert( std::make_pair( "", pValue ) );
      ::tobii_sdk_free_string( pValue );
    }
    tobii_sdk_device_info_iterator_t* pIterator = ::tobii_sdk_device_info_get_iterator( p, &mError );
    while( !mError && ::tobii_sdk_device_info_iterator_move_next( pIterator, &mError ) )
    {
      const char* pKey = ::tobii_sdk_device_info_iterator_get_key_name( pIterator, &mError );
      pValue = ::tobii_sdk_device_info_get( p, pKey, &mError );
      if( pKey && pValue )
        mInfo.insert( std::make_pair( pKey, pValue ) );
      ::tobii_sdk_free_string( pKey );
      ::tobii_sdk_free_string( pValue );
    }
    ::tobii_sdk_device_info_iterator_destroy( pIterator );
  }
  virtual ~Device()
  {
    if( mpResult )
      ::tobii_sdk_error_destroy( mpResult );
    if(mpSyncManager)
      ::tobii_sdk_sync_manager_destroy(mpSyncManager);
    for( size_t i = 0; i < mCallbackRegs.size(); ++i )
      ::tobii_sdk_callback_connection_destroy( mCallbackRegs[i] );
    if( mpConnection )
      ::tobii_sdk_message_passer_destroy( mpConnection );
    ::tobii_sdk_factory_info_destroy( mpAddress );
    ::tobii_sdk_device_info_destroy( mpDevice );
  }
  const std::string Id() const { return (*this)[""]; }
  const std::string operator[]( const std::string& key ) const
  { Info::const_iterator i = mInfo.find( key ); return (i == mInfo.end()) ? "" : i->second; }
  const Info& Info() const { return mInfo; }
  const Error& Error() const { return mError; }

  bool Connect()
  {
    if( !mpDevice )
      return false;
    if( !mpConnection )
    {
      ConnectionRequest r = { this, ::CreateEventA(0, 0, 0, 0) };
      ::tobii_sdk_message_passer_get( mpAddress, Mainloop::Instance(), &OnConnect, &r, &mError );
      if( !mError )
        ::WaitForSingleObject(r.event, -1);
      if( mError )
        mpConnection = 0;
      else
      {
        tobii_sdk_callback_connection_t* pReg = ::tobii_sdk_message_passer_add_error_handler( mpConnection, &OnError, this, &mError );
        mCallbackRegs.push_back( pReg );
      }
      ::CloseHandle(r.event);
    }
    if(!mError && !mpSyncManager)
    {
      tobii_sdk_clock_t* pClock = ::tobii_sdk_clock_get_default(&mError);
      if(!mError)
        mpSyncManager = ::tobii_sdk_sync_manager_create(
          pClock, mpAddress, Mainloop::Instance(),
          &OnError, this,
          &OnSyncManagerStatusChanged, this,
          &mError
        );
    }

    return mpConnection;
  }

  typedef void (*Callback)( int, Arguments&, void* );
  bool RegisterCallback( int opcode, Callback fn, void* data )
  {
    tobii_sdk_callback_connection_t* pReg = ::tobii_sdk_message_passer_add_data_handler( mpConnection, opcode, &OnData, this, &mError );
    if( pReg )
    {
      CallbackDef def = { opcode, fn, data };
      mCallbacks.push_back( def );
      mCallbackRegs.push_back( pReg );
    }
    return pReg;
  }

  bool StartTracking()
  {
    Arguments args;
    args.Put( uint32_t( GazeDataChannel ) ).Put( std::vector<uint32_t>() );
    return SendRequest( Tobii::StartTracking, args );
  }
  bool StopTracking()
  {
    Arguments args;
    args.Put( uint32_t( GazeDataChannel ) );
    return SendRequest( Tobii::StopTracking, args );
  }

  bool SetUnitName(const std::string& name)
  {
    Arguments args;
    args.Put(name);
    return SendRequest(Tobii::SetUnitName, args);
  }
  bool GetUnitName(std::string& name)
  {
    Arguments args;
    bool b = SendRequest(Tobii::GetUnitName, args);
    if(b) args.Get(name);
    return b;
  }

  bool GetUnitInfo(std::string& serialNumber, std::string& model, std::string& generation, std::string& firmwareVersion)
  {
    Arguments args;
    bool b = SendRequest(Tobii::GetUnitInfo, args);
    if(b) args.Get(serialNumber).Get(model).Get(generation).Get(firmwareVersion);
    return b;
  }

  bool EnumerateFrameRates( std::vector<float>& rates )
  {
    Arguments args;
    bool b = SendRequest( Tobii::EnumerateFrameRates, args );
    if(b) args.Get(rates);
    return b;
  }
  bool SetFrameRate(float rate)
  {
    Arguments args;
    args.Put(rate);
    return SendRequest(Tobii::SetFrameRate, args);
  }
  bool GetFrameRate(float& rate)
  {
    Arguments args;
    bool b = SendRequest(Tobii::GetFrameRate, args);
    if(b) args.Get(rate);
    return b;
  }

  bool EnumerateIlluminationModes(std::vector<std::string>& modes)
  {
    Arguments args;
    bool b = SendRequest(Tobii::EnumerateIlluminationModes, args);
    if(b) args.Get(modes);
    return b;
  }
  bool SetIlluminationMode(const std::string& mode)
  {
    Arguments args;
    args.Put(mode);
    return SendRequest(Tobii::SetIlluminationMode, args);
  }
  bool GetIlluminationMode(std::string& mode)
  {
    Arguments args;
    if(SendRequest(Tobii::GetIlluminationMode, args) && args.OutCount() > 0)
    {
      args.Get(mode);
      return true;
    }
    return false;
  }

  bool SetLowBlinkMode(bool enabled)
  {
    Arguments args;
    args.Put(uint32_t(enabled));
    return SendRequest(Tobii::SetLowBlinkMode, args);
  }
  bool GetLowBlinkMode(bool& enabled)
  {
    Arguments args;
    if(SendRequest(Tobii::SetLowBlinkMode, args) && args.OutCount() > 0)
    {
      uint32_t i = 0;
      args.Get(i);
      enabled = i;
      return true;
    }
    return false;
  }

  bool GetTrackBox(TrackBox& box)
  {
    Arguments args;
    bool b = SendRequest(Tobii::GetTrackBox, args);
    if(b) box.Parse(args);
    return b;
  }

  bool GetXConfiguration(Point3d& upperLeft, Point3d& upperRight, Point3d& lowerLeft)
  {
    Arguments args;
    bool b = SendRequest(Tobii::GetXConfiguration, args);
    if(b) args.Get(upperLeft).Get(upperRight).Get(lowerLeft);
    return b;
  }

  int64_t ClockRemoteToLocal(int64_t timestamp)
  {
    return ::tobii_sdk_sync_manager_convert_from_remote_to_local(mpSyncManager, timestamp, &mError);
  }
  int ClockSyncState() const { return mSyncState; }

private:
  struct Request { tobii_sdk_opcode_t opcode; Arguments* args; HANDLE event; };
  bool SendRequest( int opcode, Arguments& args )
  {
    Request r = { opcode, &args, ::CreateEventA(0, 0, 0, 0) };
    bool wait = ::tobii_sdk_message_passer_execute_request( mpConnection, opcode, args.In(), &OnResponse, &r, &mError );
    if( wait )
      ::WaitForSingleObject(r.event, -1 );
    ::CloseHandle(r.event);
    return !mError;
  }

  struct ConnectionRequest { Device* device; HANDLE event; };
  static void TOBIISDK_CALL
  OnConnect( tobii_sdk_error_t* error, tobii_sdk_message_passer_t* mpi, void* p )
  {
    ConnectionRequest* request = reinterpret_cast<ConnectionRequest*>(p);
    ::ResetEvent(request->event);
    *(&request->device->mError) = error;
    request->device->mpConnection = mpi;
    ::SetEvent(request->event);
  }

  static void TOBIISDK_CALL
  OnError( tobii_sdk_error_code_t error, void* pData )
  {
    Device* this_ = reinterpret_cast<Device*>( pData );
    if( this_->mpResult )
    {
      ::tobii_sdk_error_destroy( this_->mpResult );
      this_->mpResult = 0;
    }
    if( error )
    {
      const char* s = ::tobii_sdk_error_convert_code_to_string( error );
      this_->mpResult = ::tobii_sdk_error_create_generic( error, s );
    }
    *(&this_->mError) = this_->mpResult;
  }

  static void TOBIISDK_CALL
  OnData( tobii_sdk_opcode_t opcode, tobii_sdk_error_code_t error, const tobii_sdk_param_stack_t* pArgs, void* pData )
  {
    OnError( error, pData );

    Device* this_ = reinterpret_cast<Device*>( pData );
    size_t i = 0;
    while( i < this_->mCallbacks.size() )
    {
      CallbackDef& d = this_->mCallbacks[i];
      if( d.opcode == opcode && d.fn )
      {
        Arguments args( pArgs );
        (d.fn)( opcode, args, d.data );
        break;
      }
    }
  }

  static void TOBIISDK_CALL
  OnResponse( tobii_sdk_opcode_t opcode, tobii_sdk_error_code_t error, const tobii_sdk_param_stack_t* pArgs, void* pData )
  {
    Request* pRequest = reinterpret_cast<Request*>( pData );
    assert( pRequest->opcode == opcode );
    pRequest->args->AssignFrom( pArgs );
    ::SetEvent( pRequest->event );
  }

  static void TOBIISDK_CALL
  OnSyncManagerStatusChanged(const tobii_sdk_sync_state_t* pState, void* pData)
  {
    Device* this_ = reinterpret_cast<Device*>(pData);
    this_->mSyncState = tobii_sdk_sync_state_get_sync_state_flag(pState, &this_->mError);
  }

  struct Info mInfo;
  class Error mError;
  tobii_sdk_error_t* mpResult;
  tobii_sdk_device_info_t* mpDevice;
  tobii_sdk_factory_info_t* mpAddress;
  tobii_sdk_message_passer_t* mpConnection;
  tobii_sdk_sync_manager_t* mpSyncManager;
  tobii_sdk_sync_state_flag_t mSyncState;
  std::vector<tobii_sdk_callback_connection_t*> mCallbackRegs;
  struct CallbackDef { tobii_sdk_opcode_t opcode; Callback fn; void* data; };
  std::vector<CallbackDef> mCallbacks;
};

class DeviceList
{
private:
  DeviceList( const DeviceList& );
  DeviceList& operator=( const DeviceList& );

public:
  DeviceList() { Update(); }
  ~DeviceList() { Clear(); }
  int Size() const { return mDevices.size(); }
  bool IsEmpty() const { return mDevices.empty(); }
  const Device* ByIndex( size_t i ) const { return mDevices[i]; }
  Device* ByIndex( size_t i ) { return mDevices[i]; }
  const Device& operator[]( size_t i ) const { return *ByIndex( i ); }
  Device& operator[]( size_t i ) { return *ByIndex( i ); }

  bool Update()
  {
    Clear();
    tobii_sdk_device_browser_t* pBrowser = ::tobii_sdk_device_browser_create( Mainloop::Instance(), &Callback, this, &mError );
    ::tobii_sdk_device_browser_destroy( pBrowser );
    return !mError;
  }
  void Clear()
  {
    for( size_t i = 0; i < mDevices.size(); ++i )
      delete mDevices[i];
    mDevices.clear();
  }

  const Error& Error() const { return mError; }

private:
  static void TOBIISDK_CALL Callback(
    tobii_sdk_on_device_browser_event_t event,
    tobii_sdk_device_info_t* pInfo, void* p )
  {
    DeviceList* this_ = reinterpret_cast<DeviceList*>( p );
    this_->mDevices.push_back( new Device( pInfo ) );
  }
  std::vector<Device*> mDevices;
  class Error mError;
};

} // namespace Tobii


#endif // TOBII_NAMESPACE_H





