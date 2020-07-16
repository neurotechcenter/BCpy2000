////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class representing an IEEE 1284 parallel port
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
#include "ParallelPort.h"
#include "BCIStream.h"
#include <Windows.h>
#include <cfgmgr32.h>
#include <algorithm>

namespace
{

const char* GetCMErrorString( CONFIGRET r )
{
  const char* s = nullptr;
#define CR(x) if( r == x ) s = #x; else
  CR( CR_SUCCESS )
  CR( CR_DEFAULT )
  CR( CR_OUT_OF_MEMORY )
  CR( CR_INVALID_POINTER )
  CR( CR_INVALID_FLAG )
  CR( CR_INVALID_DEVNODE )
  CR( CR_INVALID_DEVINST )
  CR( CR_INVALID_RES_DES )
  CR( CR_INVALID_LOG_CONF )
  CR( CR_INVALID_ARBITRATOR )
  CR( CR_INVALID_NODELIST )
  CR( CR_DEVNODE_HAS_REQS )
  CR( CR_DEVINST_HAS_REQS )
  CR( CR_INVALID_RESOURCEID )
  CR( CR_DLVXD_NOT_FOUND )
  CR( CR_NO_SUCH_DEVNODE )
  CR( CR_NO_SUCH_DEVINST )
  CR( CR_NO_MORE_LOG_CONF )
  CR( CR_NO_MORE_RES_DES )
  CR( CR_ALREADY_SUCH_DEVNODE )
  CR( CR_ALREADY_SUCH_DEVINST )
  CR( CR_INVALID_RANGE_LIST )
  CR( CR_INVALID_RANGE )
  CR( CR_FAILURE )
  CR( CR_NO_SUCH_LOGICAL_DEV )
  CR( CR_CREATE_BLOCKED )
  CR( CR_NOT_SYSTEM_VM )
  CR( CR_REMOVE_VETOED )
  CR( CR_APM_VETOED )
  CR( CR_INVALID_LOAD_TYPE )
  CR( CR_BUFFER_SMALL )
  CR( CR_NO_ARBITRATOR )
  CR( CR_NO_REGISTRY_HANDLE )
  CR( CR_REGISTRY_ERROR )
  CR( CR_INVALID_DEVICE_ID )
  CR( CR_INVALID_DATA )
  CR( CR_INVALID_API )
  CR( CR_DEVLOADER_NOT_READY )
  CR( CR_NEED_RESTART )
  CR( CR_NO_MORE_HW_PROFILES )
  CR( CR_DEVICE_NOT_THERE )
  CR( CR_NO_SUCH_VALUE )
  CR( CR_WRONG_TYPE )
  CR( CR_INVALID_PRIORITY )
  CR( CR_NOT_DISABLEABLE )
  CR( CR_FREE_RESOURCES )
  CR( CR_QUERY_VETOED )
  CR( CR_CANT_SHARE_IRQ )
  CR( CR_NO_DEPENDENT )
  CR( CR_SAME_RESOURCES )
  CR( CR_NO_SUCH_REGISTRY_KEY )
  CR( CR_INVALID_MACHINENAME )
  CR( CR_REMOTE_COMM_FAILURE )
  CR( CR_MACHINE_UNAVAILABLE )
  CR( CR_NO_CM_SERVICES )
  CR( CR_ACCESS_DENIED )
  CR( CR_CALL_NOT_IMPLEMENTED )
  CR( CR_INVALID_PROPERTY )
  CR( CR_DEVICE_INTERFACE_ACTIVE )
  CR( CR_NO_SUCH_DEVICE_INTERFACE )
  CR( CR_INVALID_REFERENCE_STRING )
  CR( CR_INVALID_CONFLICT_LIST )
  CR( CR_INVALID_INDEX )
  CR( CR_INVALID_STRUCTURE_SIZE )
#undef CR
  s = "Unknown error";
  return s;
}

class InpOut32
{
private:
  static const int32_t IOCTL_READ_PORT_UCHAR = -1673519100;
  static const int32_t IOCTL_WRITE_PORT_UCHAR = -1673519096;
  HANDLE mHandle;

  static HANDLE OpenDeviceFile( const char* file )
  {
    return ::CreateFileA(
      file, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
      nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
    );
  }

public:
  InpOut32() : mHandle( INVALID_HANDLE_VALUE ) {}
  ~InpOut32() { Close(); }
  bool Read( uint16_t address, uint8_t& value )
  {
    DWORD ignored = 0;
    return ::DeviceIoControl( mHandle, IOCTL_READ_PORT_UCHAR, &address, sizeof(address), &value, sizeof(value), &ignored, nullptr );
  }
  bool Write( short address, uint8_t value )
  {
    union { uint16_t address; uint8_t bytes[3]; } arg = { address };
    arg.bytes[2] = value;
    DWORD ignored = 0;
    return ::DeviceIoControl( mHandle, IOCTL_WRITE_PORT_UCHAR, &arg, sizeof(arg), nullptr, 0, &ignored, nullptr );
  }
  bool Open()
  {
    Close();
    mHandle = OpenDeviceFile( "\\\\.\\inpout32" );
    if( mHandle == INVALID_HANDLE_VALUE && ::GetLastError() == ERROR_FILE_NOT_FOUND )
      mHandle = OpenDeviceFile( "\\\\.\\inpoutx64" );
    if( mHandle == INVALID_HANDLE_VALUE )
    {
      if( ::GetLastError() == ERROR_FILE_NOT_FOUND )
        bcierr << "InpOut32 driver is required for parallel port IO, but is not present on your system.\n"
               << "An installation package is available from\n"
               << "http://www.bci2000.org/downloads/bin/InpOutBinaries_1501.zip\n"
               << "Use your BCI2000 account for downloading, unzip the file, and run InstallDriver.exe "
               << "from the win32 subdirectory, even on 64-bit systems.";
      else
        bcierr << "Could not open InpOut32 IO driver: " << SysError().Message();
    }
    return IsOpen();
  }
  bool IsOpen() const
  {
    return mHandle != INVALID_HANDLE_VALUE;
  }
  void Close()
  {
    if( IsOpen() )
    {
      ::CloseHandle( mHandle );
      mHandle = INVALID_HANDLE_VALUE;
    }
  }
} sInpout32;

std::string
GetDevnodeProperty( DEVNODE device, ULONG property )
{
  bool ok = true;
  std::string result;
  ULONG size = 0;
  ok = ok && CR_BUFFER_SMALL == ::CM_Get_DevNode_Registry_PropertyA( device, property, nullptr, nullptr, &size, 0 );
  std::vector<char> buf( size );
  ok = ok && CR_SUCCESS == ::CM_Get_DevNode_Registry_PropertyA( device, property, nullptr, buf.data(), &size, 0 );
  if( ok )
    result = buf.data();
  return result;
}

}

ParallelPort::Ptr
ParallelPort::List::Find( const std::string& name )
{
  auto port = std::find_if( begin(), end(), [name]( const ParallelPort::Ptr p ) { return p->Name() == name; } );
  return ( port == end() ? nullptr : *port );
}

#define CRCALL(x) ok = ok && CR_SUCCESS == ( configret = (x) ), ok

bool
ParallelPort::Enumerate( List& outPorts )
{
  outPorts.clear();
  bool ok = true;
  CONFIGRET configret = CR_SUCCESS;
  ULONG size = 0;
  // LPTENUM enumerates "Printer port logical interface" devices, which are hidden children of the port hardware devices themselves.
  const char* filter = "LPTENUM";
  CRCALL( ::CM_Get_Device_ID_List_SizeA( &size, filter, CM_GETIDLIST_FILTER_ENUMERATOR ) );
  std::vector<char> deviceList( size );
  CRCALL( ::CM_Get_Device_ID_ListA( filter, deviceList.data(), deviceList.size(), CM_GETIDLIST_FILTER_ENUMERATOR ) );
  for( const char* deviceId = deviceList.data(); ok && *deviceId; deviceId += ::strlen( deviceId ) + 1 )
  {
    DEVINST device;
    if( CR_SUCCESS == ::CM_Locate_DevInstA( &device, const_cast<char*>( deviceId ), CM_LOCATE_DEVNODE_NORMAL ) )
    {
      // Get device location information: "LPTx"
      std::string location = GetDevnodeProperty( device, CM_DRP_LOCATION_INFORMATION );
      // Get port hardware device node, and its IO config
      std::vector<IORange> ioRanges;
      DEVINST parent;
      CRCALL( ::CM_Get_Parent( &parent, device, 0 ) );
      LOG_CONF conf;
      CRCALL( ::CM_Get_First_Log_Conf( &conf, parent, ALLOC_LOG_CONF ) );
      if (configret == CR_NO_MORE_LOG_CONF)
      {
        bciwarn << "incompatible LPT device found";
        configret = CR_SUCCESS;
        ok = true;
      }
      else
      {
        std::string manufacturer = GetDevnodeProperty( parent, CM_DRP_MFG );

        RES_DES res;
        CONFIGRET ret = ::CM_Get_Next_Res_Des( &res, conf, ResType_IO, nullptr, 0 );
        while( ret == CR_SUCCESS )
        {
          ULONG size = 0;
          ok = ok && CR_SUCCESS == ::CM_Get_Res_Des_Data_Size( &size, res, 0 );
          std::vector<BYTE> buf( size );
          CRCALL( ::CM_Get_Res_Des_Data( res, buf.data(), buf.size(), 0 ) );
          if( ok )
          {
            const IO_RESOURCE* pIO = reinterpret_cast<IO_RESOURCE*>( buf.data() );
            ok = ok && ( pIO->IO_Header.IOD_DesFlags & fIOD_IO );
            if( ok )
            {
              IORange range =
              {
                pIO->IO_Header.IOD_Alloc_Base,
                pIO->IO_Header.IOD_Alloc_End + 1 - pIO->IO_Header.IOD_Alloc_Base
              };
              ioRanges.push_back( range );
            }
          }
          RES_DES next;
          ret = ::CM_Get_Next_Res_Des( &next, res, ResType_IO, nullptr, 0 );
          ::CM_Free_Res_Des_Handle( res );
          res = next;
        }
        if( ok && ret != CR_NO_MORE_RES_DES )
        {
          ok = false;
          configret = ret;
        }
        ::CM_Free_Log_Conf_Handle( conf );
      }
      if( ok )
      {
        Ptr p( new ParallelPort );
        p->mName = location;
        p->mIORanges = ioRanges;
        outPorts.push_back( p );
      }
    }
  }
  if( !ok )
  {
    std::string msg;
    BOOL isWow64 = ::IsWow64Process( ::GetCurrentProcess(), &isWow64 ) && isWow64;
    switch( configret )
    {
    case CR_NO_SUCH_VALUE:
      ok = true; // no LPT ports to enumerate
      break;
    case CR_ACCESS_DENIED:
      msg = "Access to hardware information is denied";
      break;
    case CR_CALL_NOT_IMPLEMENTED:
      if( isWow64 )
        msg = "32-bit process is denied access to hardware information on 64-bit Windows";
      break;
    }
    if( !ok && !msg.empty() )
      bciwarn << "Could not enumerate parallel ports: " << msg;
    else if( !ok )
      bcierr << "Unexpected error when enumerating parallel ports: " << GetCMErrorString( configret );
  }
  return ok;
}

ParallelPort::ParallelPort()
: mHandle( INVALID_HANDLE_VALUE )
{
}

ParallelPort::~ParallelPort()
{
  Close();
}

void
ParallelPort::Close()
{
  if( mHandle != INVALID_HANDLE_VALUE )
  {
    ::CloseHandle( mHandle );
    mHandle = INVALID_HANDLE_VALUE;
  }
}

bool
ParallelPort::Open()
{
  if( !sInpout32.IsOpen() )
    sInpout32.Open();
  if( IsOpen() )
    Close();
  if( mIORanges.empty() )
    bcierr << "Device " << mName << " is not an internal LPT port, don't know how to access hardware registers";
  else
  {
    // Having a handle open in exclusive mode makes sure that no one else is using the port.
    mHandle = ::CreateFileA( mName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    if( mHandle == INVALID_HANDLE_VALUE )
    {
      if( ::GetLastError() == ERROR_SHARING_VIOLATION )
        bcierr << "Device " << mName << " is in use for another purpose";
      else if( ::GetLastError() == ERROR_ACCESS_DENIED )
        bcierr << "Device " << mName << ": Access denied, most likely in use for another purpose";
      else
        bcierr << "Could not open device " << mName << ": " << SysError().Message();
    }
    else
    {
      std::ostringstream oss;
      oss << std::hex << std::uppercase;
      for( const auto& r : mIORanges )
        for( uint16_t i = 0; i < r.size; ++i )
        {
          uint16_t address = r.base + i;
          uint8_t value = 0;
          sInpout32.Read( address, value );
          oss << address << ":" << int(value) << "\n";
        }
      bcidbg << mName << " register values:\n" << oss.str();
    }
  }
  if( IsOpen() && mIORanges.size() > 1 )
  { // ECP hardware, switch to byte mode (aka PS/2 mode)
    bool ok = true;
    ok = ok && Write( ECR, 0x20 );
    uint8_t value = 0;
    ok = ok && Read( ECR, value );
    ok = ok && ( value & 0x60 ) == 0x20;
    if( !ok )
      bcierr << "Device " << mName << ": Could not switch to byte mode";
  }
  return IsOpen();
}

bool
ParallelPort::IsOpen() const
{
  return mHandle != INVALID_HANDLE_VALUE;
}

uint16_t
ParallelPort::Address( Register r ) const
{
  int range = ( r & CRA ) ? 1 : 0;
  return range >= mIORanges.size() ? 0 : mIORanges[range].base + ( r & ~CRA );
}

bool
ParallelPort::Write( Register reg, uint8_t value )
{
  uint16_t address = Address( reg );
  return address ? sInpout32.Write( address, value ) : false;
}

bool
ParallelPort::Read( Register reg, uint8_t& value )
{
  uint16_t address = Address( reg );
  return address ? sInpout32.Read( address, value ) : false;
}

std::ostream&
ParallelPort::WriteToStream( std::ostream& os ) const
{
  os << mName;
  if( !mIORanges.empty() )
    os << " @" << std::hex << std::uppercase << mIORanges.front().base;
  return os;
}

