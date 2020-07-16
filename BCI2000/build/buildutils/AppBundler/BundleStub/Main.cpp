////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Main function for the AppBundler execution stub.
//   NOTE: This is not the actual stub entry function.
//   For the stub to be able to properly clean up after itself,
//   it needs to have a TLS callback entry in its PE header.
//   To avoid a maintenance nightmare, we do not use any of the
//   compiler/linker specific TLS support. Instead, we use an
//   initial main function which will save an appropriately
//   modified version of its own executable under a different
//   name, to be used as the actual execution stub.
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
////////////////////////////////////////////////////////////////////
#include "PEHeader.h"
#include "Utils.h"
#include "Stub.h"
#include "MiniCRT.h"

bool TransformToStub( const char* );

void WINAPI InitialEntryPoint()
{
  int argc = 0;
  char** argv = nullptr;
  MiniCRT_init( &argc, &argv );
  int err = 0;
  if( argc < 2 )
    err = -1;
  else if( !TransformToStub( argv[1] ) )
    err = ::GetLastError();
  ::ExitProcess(err);
}

static ULONG sTlsIndex = 0;
static PIMAGE_TLS_CALLBACK sTlsCallbacks[] = { &StubTlsCallback, 0 };
static IMAGE_TLS_DIRECTORY sTlsDirectory =
{
  0, 0,
  reinterpret_cast<size_t>( &sTlsIndex ), reinterpret_cast<size_t>( sTlsCallbacks ),
  0, 0
};

bool TransformToStub( const char* stubName )
{
  char buf[MAX_PATH + 1];
  if( !::GetModuleFileNameA( 0, buf, sizeof(buf)-1 ) )
    return false;
  if( !::CopyFileA( buf, stubName, false ) )
    return false;
  uint64_t size = 0;
  PEHeader stub( MapExistingFile( stubName, &size, readwrite ) );
  if( stub.Bits() != sizeof(void*) * 8 )
    return false;
  
  PEHeader self( ::GetModuleHandleA( nullptr ) );
  union { void(WINAPI *f)(); char* c; } entryPoint = { &StubEntryPoint };
  stub.OptionalHeader().pNative->AddressOfEntryPoint = entryPoint.c - self.BaseAddress();

  union { IMAGE_TLS_DIRECTORY* d; char* c; } tlsDirectory = { &sTlsDirectory };
  IMAGE_DATA_DIRECTORY* tlsEntry = stub.DirectoryEntry( IMAGE_DIRECTORY_ENTRY_TLS );
  tlsEntry->VirtualAddress = int( tlsDirectory.c - self.BaseAddress() );
  tlsEntry->Size = sizeof( sTlsDirectory );
  stub.FixupRedundantFields();
  ::FlushViewOfFile( stub.BaseAddress(), size );
  ::UnmapViewOfFile( stub.BaseAddress() );
  return true;
}

