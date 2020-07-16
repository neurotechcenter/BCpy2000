////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Generalized interface for reading data from sockets, files,
//   pipes, etc.
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
#include "IOObjects.h"
#include "StaticObject.h"
#include "Files.h"
#include "Streambuf.h"
#include "Exception.h"

namespace Tiny
{

BasicIOObject::~BasicIOObject()
{
  if( mpStreambuf )
    SuggestDebugging << "Still attached to a Streambuf, call OnDestruct() from subclass destructors to fix";
}

Streambuf*
BasicIOObject::AttachTo( Streambuf* sb )
{
  Streambuf* p = AttachedTo();
  if( mBeingAttached )
    return p;
  mBeingAttached = true;
  if( p != sb )
    OnAttachTo( sb );
  mpStreambuf = sb;
  mBeingAttached = false;
  return p;
}

void
BasicIOObject::OnAttachTo( Streambuf* )
{
  throw std_logic_error << "This function should never be called";
}

const char*
BasicIOObject::OnDescribeIOState() const
{
  if( Good() )
    return "Good";
  if( Bad() )
    return "Unknown error state";
  if( Eof() && Failed() )
    return "Operation failed at end-of-file";
  if( Eof() )
    return "End-of-file reached";
  if( Failed() )
    return "Operation failed";
  return "State unknown";
}

InputObject&
InputObject::Default()
{
  static StaticObject<File> file;
  static bool initialized;
  if( !initialized )
    file->AttachToFd( 0 );
  return *file;
}

int64_t
InputObject::Read( void* p, int64_t n )
{
  ScopedLock( this );
  if( Bad() )
    return -1;
  int64_t r = OnRead( static_cast<char*>( p ), n );
  if( r == 0 )
    SetIOStateBits( std::ios::eofbit );
  else if( r < 0 )
    SetIOStateBits( std::ios::badbit );
  return r;
}

OutputObject&
OutputObject::Default()
{
  static StaticObject<File> file;
  static bool initialized;
  if( !initialized )
    file->AttachToFd( 1 );
  return *file;
}

void
InputObject::OnAttachTo( Streambuf* sb )
{
  Streambuf* prev = AttachedTo();
  if( prev )
  {
    Assert( prev->Input() == this );
    prev->SetInput( 0 );
  }
  if( sb )
    sb->SetInput( this );
}

void
OutputObject::OnAttachTo( Streambuf* sb )
{
  Streambuf* prev = AttachedTo();
  if( prev )
  {
    Assert( prev->Output() == this );
    prev->SetOutput( 0 );
  }
  if( sb )
    sb->SetOutput( this );
}

int64_t
OutputObject::Write( const void* p, int64_t n )
{
  ScopedLock( this );
  if( Bad() )
    return -1;
  int64_t r = OnWrite( static_cast<const char*>( p ), n );
  if( r == 0 )
    SetIOStateBits( std::ios::eofbit );
  else if( r < 0 )
    SetIOStateBits( std::ios::badbit );
  return r;
}


} // namespace
