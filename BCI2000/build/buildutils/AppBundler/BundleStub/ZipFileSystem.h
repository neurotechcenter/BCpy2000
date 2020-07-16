//////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Glue code that allows NtHookFs to access the contents of a
//   zip archive.
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
#ifndef ZIP_FILE_SYSTEM_H
#define ZIP_FILE_SYSTEM_H

#include "NtHookFs.h"
#include "MinizArchive.h"

class ZipFileSystem : public NtHookFs
{
public:
  ZipFileSystem( const wchar_t* );
  ~ZipFileSystem();

protected:
  struct ZipFileStream;

  bool StatByName( const wchar_t*, const wchar_t*, FILE_BOTH_DIR_INFORMATION* );
  bool StatByIndex( int, wchar_t**, FILE_BOTH_DIR_INFORMATION* );
  NtHookFs::Stream* OpenByName( const wchar_t*, const wchar_t* );
  NtHookFs::Stream* OpenByIndex( int );
  void* TemplateHandle() { return mTemplateHandle; }

  static bool StatByName_( NtHookFs* pInstance, const wchar_t* name, const wchar_t* end, FILE_BOTH_DIR_INFORMATION* stat )
  { return reinterpret_cast<ZipFileSystem*>( pInstance )->StatByName( name, end, stat ); }
  static bool StatByIndex_( NtHookFs* pInstance, int idx, wchar_t** name, FILE_BOTH_DIR_INFORMATION* stat )
  { return reinterpret_cast<ZipFileSystem*>( pInstance )->StatByIndex( idx, name, stat ); }
  static NtHookFs::Stream* OpenByName_( NtHookFs* pInstance, const wchar_t* name, const wchar_t* end, int mode)
  { return reinterpret_cast<ZipFileSystem*>( pInstance )->OpenByName( name, end ); }
  static NtHookFs::Stream* OpenByIndex_( NtHookFs* pInstance, int index, int mode )
  { return reinterpret_cast<ZipFileSystem*>( pInstance )->OpenByIndex( index ); }
  static void* TemplateHandle_( NtHookFs* pInstance )
  { return reinterpret_cast<ZipFileSystem*>( pInstance )->TemplateHandle(); }

private:
  void TranslateStat( const mz_zip_archive_file_stat*, FILE_BOTH_DIR_INFORMATION*, bool );
  wchar_t* mpArchiveName;
  void* mTemplateHandle;
  MinizArchive mArchive;
};

#endif // ZIP_FILE_SYSTEM_H
