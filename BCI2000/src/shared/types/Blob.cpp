////////////////////////////////////////////////////////////////////////////////
// $Id: Blob.cpp 5529 2016-09-14 18:47:45Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A type for binary large objects (BLOBs) which may be embedded
//   into BCI2000 data streams.
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
#include "PCHIncludes.h"
#pragma hdrstop

#include "Blob.h"
#include "FileUtils.h"
#include "StringUtils.h"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

using namespace std;

static void NormalizeType( string& type )
{
  if( type.find( "." ) == 0 )
    type = type.substr( 1 );
}



Blob::Blob()
: mpData( 0 ), mLength( 0 ),
  mpTempFile( 0 )
{
}

Blob::Blob( const string& inFileName )
: mFileName( FileUtils::AbsolutePath( inFileName ) ),
  mpData( 0 ), mLength( 0 ),
  mType( FileUtils::ExtractExtension( inFileName ) ),
  mpTempFile( 0 )
{
  NormalizeType( mType );
  ifstream in( inFileName.c_str(), ios::binary | ios::in );
  if( !in.is_open() )
    throw runtime_error( "Could not open file for reading: " + inFileName );
  in.seekg( 0, ios::end );
  mLength = static_cast<size_t>( in.tellg() );
}

Blob::Blob( const bci::Resource& inResource )
: mpData( inResource.data ), mLength( inResource.length ), mType( inResource.type ),
  mpTempFile( 0 )
{
  NormalizeType( mType );
}

Blob::Blob( const char* data, size_t length, const string& type )
: mpData( data ), mLength( length ),
  mType( type ),
  mpTempFile( 0 )
{
  NormalizeType( mType );
}

Blob::~Blob()
{
  delete mpTempFile;
}

const string&
Blob::Type() const
{
  return mType;
}

void
Blob::GetStringData() const
{
  if( mpData )
  {
    if( mpData != mStringData.data() )
    {
      mStringData = string( mpData, mLength );
      mpData = mStringData.data();
    }
  }
  else if( mStringData.empty() )
  {
    mStringData.reserve( mLength );
    ifstream is( mFileName.c_str(), ios::binary | ios::in );
    mStringData.assign( istreambuf_iterator<char>( is ), istreambuf_iterator<char>() );
  }
}

bool
Blob::SaveAs( const string& inFileName ) const
{
  string newFile = FileUtils::AbsolutePath( inFileName );
  if( FileUtils::ExtractExtension( newFile ) != "." + mType )
    newFile += "." + mType;

  ofstream out( newFile.c_str(), ios::binary | ios::out );
  if( !out.is_open() )
    return false;
  if( Write( out ) )
    mFileName = newFile;
  return !!out;
}

bool
Blob::SaveAsTemp() const
{
  bool result = true;
  if( !mpTempFile )
  {
    mpTempFile = new FileUtils::TemporaryFile( "." + mType );
    result = !!Write( *mpTempFile );
    mpTempFile->Close();
  }
  if( result )
    mFileName = mpTempFile->Name();
  return result;
}

const string&
Blob::FileName() const
{
  return mFileName;
}

ostream&
Blob::WriteAsResource( ostream& os ) const
{
  istream is( 0 );
  if( mpData )
    is.rdbuf( new stringbuf( string( mpData, mLength ), ios::in ) );
  else
  {
    filebuf* p = new filebuf;
    p->open( mFileName.c_str(), ios::in | ios::binary );
    is.rdbuf( p );
  }
  string indent( static_cast<size_t>( os.width() ), os.fill() );
  ios::fmtflags format = os.flags();
  os << indent << dec;
  int n = 0;
  char c = 0;
  while( is.get( c ) )
  {
    if( !( ++n %= 40 ) )
      os << "\n" << indent;
    os << static_cast<unsigned int>( c ) << ",";
  }
  os.flags( format );
  delete is.rdbuf( 0 );
  return os;
}

ostream&
Blob::Write( ostream& os ) const
{
  if( mpData )
    os.write( mpData, mLength );
  else if( mStringData.length() == mLength )
    os.write( mStringData.data(), mLength );
  else if( !mFileName.empty() )
  {
    ifstream in( mFileName.c_str(), ios::binary | ios::in );
    if( !in.is_open() )
      os.setstate( ios::failbit );
    os << in.rdbuf();
  }
  else
    os.setstate( ios::failbit );
  return os;
}

ostream&
Blob::InsertInto( ostream& os ) const
{
  os << mType << ":";
  GetStringData();
  return StringUtils::WriteAsBase64( os, mStringData );
}

istream&
Blob::ExtractFrom( istream& is )
{
  getline( is, mType, ':' );
  mpData = 0;
  delete mpTempFile;
  mpTempFile = 0;
  mFileName.clear();
  if( StringUtils::ReadAsBase64( is, mStringData, &::isspace ) )
  {
    mpData = mStringData.data();
    mLength = mStringData.size();
  }
  return is;
}
