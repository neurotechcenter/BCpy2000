//////////////////////////////////////////////////////////////////////
// $Id: FileUtils.h 5724 2018-03-17 18:33:49Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: File-system related utility functions.
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
///////////////////////////////////////////////////////////////////////
#ifndef TINY_FILE_UTILS_H
#define TINY_FILE_UTILS_H

#include <string>
#include <vector>
#include <fstream>
#include "SharedPointer.h"
#include "TimeUtils.h"

#undef RemoveDirectory

namespace Tiny
{

namespace FileUtils
{
  std::string EnsureSeparator( const std::string&, const std::string& = "" );
  std::string StripSeparator( const std::string& );
  std::string ExecutablePath();
  std::string ApplicationTitle();

  std::string InstallationDirectory();
  inline std::string InstallationDirectoryS() { return StripSeparator( InstallationDirectory() ); }

  // FileUtils::WorkingDirectory is a per-thread variable used to transform relative
  // into absolute paths, but is not the actual process working directory, and is not
  // applied outside the Tiny library.
  std::string WorkingDirectory();
  inline std::string WorkingDirectoryS() { return StripSeparator( WorkingDirectory() ); }

  // This sets the current working directory.
  bool ChangeDirectory( const std::string& );

  // This transforms relative paths into absolute paths, using the
  // per-thread working directory as a reference.
  std::string AbsolutePath( const std::string& );

  // Normalize a path by moving any relative elements to the beginning, and by
  // using "/" as a directory separator.
  // Will return empty string for empty input only, "./" or "." otherwise.
  // With case-insensitive but case-preserving file systems, case is adapted
  // to be consistent with directory listing.
  // On Windows, short file names are replaced with long ones.
  // Determining the normalized form of a path may be expensive to compute.
  std::string NormalizedPath( const std::string& );

  // This returns a path free of relative elements, without symlinks, and suited to uniquely identify objects,
  // as defined by the operating system.
  // Result will be empty if the path does not point to an existing object.
  // Result will contain a trailing directory separator if, and only if, it is a directory.
  std::string CanonicalPath( const std::string& );

  // Canonical path to an executable, using path environment variable to resolve.
  std::string ExecutableCanonicalPath( const std::string& );

  // POSIX realpath() workalike: Normalized form of canonical path.
  std::string RealPath( const std::string& );

  // This returns the parent of the specified object (file or directory).
  std::string ParentDirectory( const std::string& );
  inline std::string ParentDirectoryS( const std::string& s ) { return StripSeparator( ParentDirectory( s ) ); }

  // A list of entries in the specified directory.
  typedef std::vector<std::string> List;
  bool ListDirectory( const std::string&, List& );

  // These return the directory portion, file name, base name portion, and extension of a path.
  std::string ExtractDirectory( const std::string& );
  inline std::string ExtractDirectoryS( const std::string& s ) { return StripSeparator( ExtractDirectory( s ) ); }
  std::string ExtractName( const std::string& );
  inline std::string ExtractFile( const std::string& s ) { return ExtractName( s ); }
  std::string ExtractBase( const std::string& );
  std::string ExtractExtension( const std::string& );
  inline std::string StripDirectory( const std::string& s ) { return ExtractFile( s ); }
  inline std::string StripExtension( const std::string& s ) { return ExtractDirectory( s ) + ExtractBase( s ); }

  bool IsFile( const std::string& );
  bool IsDirectory( const std::string& );
  bool IsSymbolicLink( const std::string& );
  bool Exists( const std::string& );

  bool IsWritableFile( const std::string& );
  bool IsWritableDirectory( const std::string& );
  bool IsWritableLocation( const std::string& );

  Time ModificationTime( const std::string& );
  int64_t Length( const std::string& );

  bool Rename( const std::string&, const std::string& );
  bool MakeDirectory( const std::string&, bool force = false );
  bool RemoveDirectory( const std::string&, bool force = false );
  bool RemoveFile( const std::string& );
  bool Copy( const std::string&, const std::string& );

  std::string MakeUniqueDirectory( const std::string& );

  std::string TemporaryDirectory();
  class TemporaryFile : public std::fstream
  {
   public:
    TemporaryFile( const std::string& pattern = "" );
    ~TemporaryFile() { Close(); }
    const std::string& Name() const { return mpFile->name; }
    bool Open();
    void Close() { std::fstream::close(); }
    bool IsOpen() const { return std::fstream::is_open(); }
    bool Flush() { Close(); return Open(); }
    bool CopyTo( const std::string& s ) const { return Copy( Name(), s ); }

    static std::string GenerateName();

   private:
    struct File
    {
      ~File() { RemoveFile( name ); }
      std::string name;
    };
    SharedPointer<File> mpFile;
  };

  std::string SearchURL( const std::string& searchTerms = "" );

} // namespace FileUtils

} // namespace Tiny

namespace FileUtils = Tiny::FileUtils;

#endif // TINY_FILE_UTILS_H
