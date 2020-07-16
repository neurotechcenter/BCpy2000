////////////////////////////////////////////////////////////////////////////////
// $Id: Blob.h 4870 2015-06-25 19:35:42Z mellinger $
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
#ifndef BLOB_H
#define BLOB_H

#include <string>
#include <iostream>
#include "utils/Lib/FileUtils.h"
#include "utils/Resource.h"

class Blob
{
 public:
  Blob();
  Blob( const bci::Resource& );
  Blob( const std::string& fileName );
  Blob( const char* data, size_t length, const std::string& type );
  ~Blob();

  const std::string& Type() const;

  bool SaveAs( const std::string& fileName ) const;
  bool SaveAsTemp() const;
  const std::string& FileName() const;

  std::ostream& WriteAsResource( std::ostream& ) const;
  std::ostream& Write( std::ostream& ) const;

  std::ostream& InsertInto( std::ostream& ) const; // formatted i/o in Base64 encoding
  std::istream& ExtractFrom( std::istream& );

 private:
  Blob( const Blob& );
  Blob& operator=( const Blob& );
  void GetStringData() const;

  size_t mLength;
  std::string mType;
  mutable const char* mpData;
  mutable std::string mStringData, mFileName;
  mutable FileUtils::TemporaryFile* mpTempFile;
};

inline
std::ostream& operator<<( std::ostream& os, const Blob& b )
{
  return b.InsertInto( os );
}

inline
std::istream& operator>>( std::istream& is, Blob& b )
{
  return b.ExtractFrom( is );
}

#endif // BLOB_H
