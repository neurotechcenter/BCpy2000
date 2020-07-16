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
#include "FilePath.h"
#include "Debugging.h"
#include <algorithm>

namespace Tiny {

bool
FilePath::IsDirSeparator( char c )
{
  static const std::string dirSeparators = FilePath::SeparatorSet();
  return dirSeparators.find( c ) != std::string::npos;
}

bool
FilePath::IsDriveSeparator( char c )
{
  return c == DriveSeparator;
}

FilePath::FilePath( const std::string& s )
: mData( s ), mKind( unknown ), mRootLength( -1 ), mParsed( false )
{
  IdentifyRoot();
}

void
FilePath::EnsureParsed() const
{
  if( !mParsed )
  {
    IdentifyElements();
    mParsed = true;
  }
}

void
FilePath::IdentifyRoot()
{
  const std::string& s = mData;
  mKind = relative;
  mRootLength = -1;
  int end = 0;
  if( s.find( "\\\\" ) == 0 && s.length() > 2 && !IsDirSeparator( s[2] ) )
  { // unc path
    end = 2;
    while( end < s.length() && !IsDirSeparator( s[end] ) )
      ++end;
    int sep = end;
    while( sep < s.length() && IsDirSeparator( s[sep] ) )
      ++sep;
    if( sep < s.length() )
    {
      mKind = absolute;
      mRootLength = end;
      mNativeSeparator = "\\";
    }
  }
  else
  {
    while( end < s.length() && IsDirSeparator( s[end] ) )
      ++end;
    if( end == 0 )
    {
      while( end < s.length() && !IsDirSeparator( s[end] ) )
        ++end;
      if( end > 0 && s[end-1] == DriveSeparator )
      { // dos absolute path with drive
        mKind = absolute;
        mRootLength = end;
        mNativeSeparator = "\\";
      }
    }
    else
    { // absolute posix path, or drive-relative dos path
      mKind = ambiguous;
      mRootLength = 0;
      mNativeSeparator = s.substr( 0, 1 );
    }
  }
}

const std::string&
FilePath::NativeSeparator() const
{
  if( mNativeSeparator.empty() )
  {
    size_t pos = mData.find_first_of( FilePath::SeparatorSet() );
    if( pos != std::string::npos )
      mNativeSeparator = mData[pos];
    else
      mNativeSeparator = "/";
  }
  return mNativeSeparator;
}

void
FilePath::IdentifyElements() const
{
  const std::string& s = mData;
  mElements.clear();
  int end = std::max( mRootLength, 0 );
  while(end < s.length())
  {
    int begin = end;
    while(end < s.length() && !IsDirSeparator( s[end] ))
      ++end;
    if( end > begin )
      mElements.push_back( s.substr( begin, end - begin ) );
    if(end < s.length())
    {
      if(mNativeSeparator.empty())
        mNativeSeparator = s[end];
      begin = end + 1;
      while( begin < s.length() && IsDirSeparator( s[begin] ) )
        ++begin;
      end = begin;
    }
  }
}

bool
FilePath::RootIsDrive() const
{
  return HasRoot() && mData[mRootLength - 1] == DriveSeparator;
}

std::string
FilePath::Root() const
{
  return mData.substr( 0, std::max( mRootLength, 0 ) );
}

FilePath&
FilePath::SetRoot( const std::string& s )
{
  EnsureParsed();
  mRootLength = s.length();
  mData = s;
  return *this;
}

int
FilePath::ElementCount() const
{
  EnsureParsed();
  return mElements.size();
}

bool
FilePath::IsRelativeElement( int i ) const
{
  return Element( i ) == ".." || Element( i ) == ".";
}

std::string
FilePath::Element( int i ) const
{
  EnsureParsed();
  if( i >= 0 && i < mElements.size() )
    return mElements[i];
  if( i < 0 && -i <= mElements.size() )
    return mElements[mElements.size() + i];
  return std::string();
}

FilePath&
FilePath::SetElement( int i, const std::string& s )
{
  EnsureParsed();
  if( i >= 0 && i < mElements.size() )
    mElements[i] = s;
  else if( i < 0 && -i <= mElements.size() )
    mElements[mElements.size() + i] = s;
  return *this;
}

std::string
FilePath::ToString( const std::string& sep ) const
{
  EnsureParsed();
  return Left( ElementCount(), sep );
}

std::string
FilePath::Left( int inCount, const std::string& inSep ) const
{
  EnsureParsed();
  const std::string& sep = inSep.empty() ? NativeSeparator() : inSep;
  std::string s;
  if( HasRoot() )
  {
    s += Root();
    s += sep;
  }
  int n = std::min<int>( inCount, mElements.size() );
  for( int i = 0; i < n - 1; ++i )
  {
    s += mElements[i];
    s += sep;
  }
  if( n > 0 )
    s += mElements[n - 1];
  if( s.empty() )
    s = ".";
  return s;
}

std::string
FilePath::Truncated( int inTruncateTo, const std::string& inEllipsis, const std::string& inSep ) const
{
  EnsureParsed();
  const std::string& sep = inSep.empty() ? NativeSeparator() : inSep;
  int maxLength = std::max<int>( inTruncateTo, inEllipsis.length() );
  int leftEnd = 0, rightBegin = mElements.size(), totalLength = inEllipsis.length();
  if( HasRoot() )
    totalLength += mRootLength + sep.length();
  while( leftEnd < rightBegin && totalLength <= maxLength )
  {
    totalLength += mElements[--rightBegin].length() + sep.length();
    if( leftEnd < rightBegin && totalLength <= maxLength )
      totalLength += mElements[leftEnd++].length() + sep.length();
  }
  std::string truncated;
  for( int i = 0; i < leftEnd; ++i )
    truncated += mElements[i] + sep;
  if( leftEnd != rightBegin )
  {
    if( !truncated.empty() )
      truncated.pop_back();
    truncated += inEllipsis;
  }
  for( int i = rightBegin; i < mElements.size(); ++i )
    truncated += mElements[i] + sep;
  if( truncated.empty() )
    truncated = "./";
  return truncated.substr( 0, truncated.length() - 1 );
}

FilePath&
FilePath::ToAbsolute( const std::string& wd )
{
  Assert( !wd.empty() );
  if( mKind == ambiguous )
  {
    FilePath wdpath( wd );
    if( wdpath.Kind() != ambiguous )
      SetRoot( wdpath.Root() );
  }
  if( mKind == relative )
  {
    std::string full = wd;
    if( !IsDirSeparator( wd.back() ) )
      full += NativeSeparator();
    full += ToString();
    *this = FilePath( full );
  }
  mKind = absolute;
  return *this;
}

FilePath&
FilePath::Simplify()
{
  EnsureParsed();
  std::vector<std::string> elements;
  int i = 0;
  while( i < mElements.size() )
  {
    std::string s = mElements[i];
    if( s == ".." )
    {
      if( !elements.empty() && elements.back() != ".." )
        elements.pop_back();
      else if( !HasRoot() )
        elements.push_back( mElements[i] );
    }
    else if( s != "." )
      elements.push_back( mElements[i] );
    ++i;
  }
  mElements = elements;
  return *this;
}

} // namespace
