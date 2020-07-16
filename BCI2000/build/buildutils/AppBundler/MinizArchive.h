////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Wrapper class for Miniz archive.
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
#ifndef MINIZ_ARCHIVE_H
#define MINIZ_ARCHIVE_H

#include "miniz.h"

class MinizArchive
{
 public:
  MinizArchive();
  ~MinizArchive();
  bool Create( const wchar_t* );
  bool Open( const wchar_t* );
  void Close();
  bool IsOpen() const;
  void* FileHandle() const { return mFile; }
  bool Writable() const { return mWritable; }
  mz_int64 ArchiveSize() const;
  bool Stat( const char*, mz_zip_archive_file_stat*, mz_uint64* = 0 );
  bool Stat( int, mz_zip_archive_file_stat*, mz_uint64* = 0, mz_bool* = 0 );


  typedef void (*Callback)( size_t );
  Callback SetWriteCallback( Callback );

  mz_zip_archive* operator&() { return &mAr; }
  const mz_zip_archive* operator&() const { return &mAr; }

 private:
  size_t OnRead( mz_uint64, void*, size_t );
  size_t OnWrite( mz_uint64, const void*, size_t );
  static size_t Read( void* handle, mz_uint64 file_ofs, void *pBuf, size_t n );
  static size_t Write( void* handle, mz_uint64 file_ofs, const void *pBuf, size_t n );
  static void* Malloc( void* handle, size_t count, size_t size );
  static void Free( void* handle, void* p );
  static void* Realloc( void* handle, void* p, size_t count, size_t size );

  mz_zip_archive mAr;
  Callback mWriteCallback;
  void* mFile, *mMutex;
  bool mWritable;
};

#endif // MINIZ_ARCHIVE_H
