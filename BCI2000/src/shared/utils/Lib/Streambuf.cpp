////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Classes for buffered and unbuffered stream IO.
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
#include "Streambuf.h"
#include "StaticObject.h"

#include <algorithm>

using namespace Tiny;

Streambuf& Tiny::DefaultInput()
{
  static StaticObject<BufferedIO> sb;
  static bool initialized;
  if( !initialized )
    sb->SetInput( &InputObject::Default() );
  return *sb;
}

Streambuf& Tiny::DefaultOutput()
{
  static StaticObject<BufferedIO> sb;
  static bool initialized;
  if( !initialized )
    sb->SetOutput( &OutputObject::Default() );
  return *sb;
}

Streambuf::Streambuf( 
  char** getbuf, size_t* getbufSize,
  char** putbuf, size_t* putbufSize
)
: fdio::streambuf( getbuf, getbufSize, putbuf, putbufSize ),
  mpInput( 0 ), mpOutput( 0 ), mBytesToExpect( 0 )
{
}

Streambuf::~Streambuf()
{
  SetInput( 0 );
  SetOutput( 0 );
}

Streambuf&
Streambuf::PredictInputCount( std::streamsize n )
{
  std::streamsize known = ( egptr() - gptr() ) + mBytesToExpect,
                  diff = n - known;
  if( diff > 0 )
    mBytesToExpect += diff;
  return *this;
}

Streambuf&
Streambuf::ClearInputPrediction()
{
  mBytesToExpect = 0;
  return *this;
}

Streambuf&
Streambuf::SetInput( InputObject* input )
{
  if( mpInput == input )
    return *this;

  if( mpInput )
    mpInput->AttachTo( 0 );
  mpInput = input;
  if( mpInput )
    input->AttachTo( this );

  pubsync();
  ClearInputPrediction();
  return *this;
}

Streambuf&
Streambuf::SetOutput( OutputObject* output )
{
  if( mpOutput == output )
    return *this;

  pubsync();
  if( mpOutput )
    mpOutput->AttachTo( 0 );
  mpOutput = output;
  if( mpOutput )
    mpOutput->AttachTo( this );
  return *this;
}

Streambuf&
Streambuf::SetIO( IOObject* io )
{
  SetInput( io );
  SetOutput( io ? &io->Output() : 0 );
  return *this;
}

std::streamsize
Streambuf::on_read( char* p, std::streamsize n )
{
  if( !mpInput )
    return -1;
  if( n > mBytesToExpect && mBytesToExpect > 0 )
    n = std::min( n, showmanyc() );
  std::streamsize r = mpInput->Read( p, n );
  mBytesToExpect -= r;
  return r;
}

std::streamsize
Streambuf::on_write( const char* p, std::streamsize n )
{
  return mpOutput ? mpOutput->Write( p, n ) : -1;
}

std::streampos
Streambuf::on_seek_to(std::streampos pos )
{
  if( mpInput && mpOutput )
  {
    std::streampos inpos = mpInput->SeekTo( pos ),
                   outpos = mpOutput->SeekTo( pos );
    return inpos == outpos ? inpos : std::streampos(-1);
  }
  if( mpInput )
    return mpInput->SeekTo( pos );
  if( mpOutput )
    return mpOutput->SeekTo( pos );
  return -1;
}

std::streamsize
Streambuf::showmanyc()
{
  if( mBytesToExpect < 1 )
    return mpInput ? mpInput->Available() : 0;
  return mBytesToExpect;
}

BufferedIO::BufferedIO( size_t inbufSize, size_t outbufSize )
: Streambuf( &mpInbuf, &mInbufSize, &mpOutbuf, &mOutbufSize ),
  mpInbuf( 0 ), mpOutbuf( 0 ), mInbufSize( inbufSize ), mOutbufSize( outbufSize )
{
  if( mInbufSize )
    mpInbuf = new char[mInbufSize];
  if( mOutbufSize )
    mpOutbuf = new char[mOutbufSize];
}

BufferedIO::~BufferedIO()
{
  sync(); 
  delete[] mpInbuf;
  delete[] mpOutbuf;
}
