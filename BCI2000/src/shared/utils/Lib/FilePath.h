//////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: A class for parsing file paths.
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
#ifndef TINY_FILE_PATH_H
#define TINY_FILE_PATH_H

#include <string>
#include <vector>

namespace Tiny
{

class FilePath
{
public:
#if _WIN32
  static const char  DirSeparator = '\\';
  static const char  DriveSeparator = ':';
  static const char  PathSeparator = ';';
  inline static const char* SeparatorSet() { return "\\/"; }
#else
  static const char  DirSeparator = '/';
  static const char  DriveSeparator = '/';
  static const char  PathSeparator = ':';
  inline static const char* SeparatorSet() { return "/"; }
#endif

  static bool IsDirSeparator( char );
  static bool IsDriveSeparator( char );

  FilePath( const std::string& );
  FilePath& ToAbsolute( const std::string& wd );
  FilePath& Simplify();
  
  enum { unknown, relative, absolute, ambiguous };
  int Kind() const { return mKind; }

  bool HasRoot() const { return mRootLength >= 0; }
  bool RootIsDrive() const;
  std::string Root() const;
  FilePath& SetRoot( const std::string& );

  int ElementCount() const;
  bool IsRelativeElement( int ) const;
  std::string Element( int ) const;
  FilePath& SetElement( int, const std::string& );

  const std::string& NativeSeparator() const;
  std::string ToString( const std::string& = "" ) const;
  std::string Left( int, const std::string& = "" ) const;
  std::string Truncated( int inTruncateTo = 70, const std::string& inEllipsis = "...", const std::string& = "" ) const;

private:
  void IdentifyRoot();
  void IdentifyElements() const;
  void EnsureParsed() const;

  mutable bool mParsed;
  int mKind, mRootLength;
  std::string mData;
  mutable std::string mNativeSeparator;
  mutable std::vector<std::string> mElements;
};

}

using Tiny::FilePath;

#endif // TINY_FILE_PATH_H
