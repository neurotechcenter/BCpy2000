//////////////////////////////////////////////////////////////////////
// $Id: ParsedComment.cpp 5602 2017-01-17 20:34:05Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that handles parsing a parameter's comment
//       for display purposes.
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
#include "ParsedComment.h"

#include "Param.h"
#include "Debugging.h"
#include <sstream>
#include <cstdlib>

using namespace std;

static const char sSeparator = '/';

ParsedComment::ParsedComment( const Param& p )
: mName( p.Name() ),
  mComment( p.Comment() ),
  mKind( unknown ),
  mIndexBase( 0 )
{
  size_t sep = mName.rfind('/');
  if(sep != std::string::npos)
    mName = "." + mName.substr(sep);

  if( !p.Sections().empty() )
    mHelpContext = *p.Sections().rbegin();

  // Look for "interpretation hints" in the comment.
  // Unknown hints will show up uninterpreted in the operator's comment display.
  static const struct
  {
    string keyword;
    Kind_type kind;
  } hints[] =
  {
#define HINT(x) { "(" #x ")", x }
    HINT( enumeration ),
    HINT( boolean ),
    HINT( inputfile ),
    HINT( outputfile ),
    HINT( directory ),
    HINT( color ),
    HINT( noedit ),
    HINT( readonly ),
#undef HINT
  };
  for( size_t i = 0; i < sizeof( hints ) / sizeof( *hints ); ++i )
  {
    size_t hintPos = mComment.find( hints[i].keyword );
    if( hintPos != string::npos )
    {
      mComment = mComment.erase( hintPos, hints[i].keyword.length() );
      if( (mKind & hints[i].kind) == 0 )
        mKind |= hints[i].kind;
    }
  }
  string paramType = p.Type();
  if( paramType.find( "matrix" ) != string::npos )
    mKind |= matrix;
  else if( paramType.find( "list" ) != string::npos )
    mKind |= list;
  else
    mKind |= singleEntry;

  if( paramType.find( "blob" ) != string::npos )
    mKind |= blob;

  switch( mKind )
  {
    case singleEntry | enumeration:
      if( !ExtractEnumValues( p ) )
        mKind = singleEntry;
      break;

    case singleEntry | boolean:
      if( ExtractEnumValues( p ) && !IsBooleanEnum() )
        mKind = singleEntry;
      break;
  }
}

bool
ParsedComment::ExtractEnumValues( const Param& p )
{
  // Only int type parameters can be enumerations or booleans.
  const string enumParamType = "int";
  if( p.Type() != enumParamType )
    return false;

  // Enumerations need a finite range.
  int lowRange = ::atoi( p.LowRange().c_str() ),
      highRange = ::atoi( p.HighRange().c_str() ),
      paramValue = ::atoi( p.Value().c_str() );
  if( lowRange != 0 && lowRange != 1
      || highRange <= lowRange
      || paramValue < lowRange
      || paramValue > highRange )
    return false;

  // Examine the comment: Does it contain an enumeration of all possible values?
  string comment = mComment;
  // Replace all punctuation marks with white space.
  const string cPunctuationChars = ",;:=()[]";
  size_t punctuationPos = comment.find_first_of( cPunctuationChars );
  while( punctuationPos != string::npos )
  {
    comment[ punctuationPos ] = ' ';
    punctuationPos = comment.find_first_of( cPunctuationChars );
  }

  map<int, int> histogram;
  istringstream is( comment );
  string        value,
                modifiedComment,
        *       currentLabel = &modifiedComment;
  while( is >> value )
  {
    // Using the >> operator for an int would accept "+" and similar strings as numbers.
    // We are only interested in groups of decimal digits.
    bool isNum = true;
    int numValue = 0;
    for( string::iterator i = value.begin(); isNum && i != value.end(); ++i )
    {
      if( *i < '0' || *i > '9' )
        isNum = false;
      else
      {
        numValue *= 10;
        numValue += *i - '0';
      }
    }
    if( isNum )
    {
      unsigned int index = numValue - lowRange;
      histogram[ index ]++;
      if( mValues.size() <= index )
        mValues.resize( index + 1 );
      currentLabel = &mValues[ index ];
    }
    else
    {
      if( !currentLabel->empty() )
        *currentLabel += "%20";
      *currentLabel += value;
    }
  }
  // Decode URL-encoded characters in values.
  for( size_t i = 0; i < mValues.size(); ++i )
  {
    EncodedString value;
    istringstream iss( mValues[ i ] );
    iss >> value;
    mValues[ i ] = value;
  }

  {
    EncodedString value;
    istringstream iss( modifiedComment );
    iss >> value;
    modifiedComment = value;
  }

  bool isEnum = is.eof();

  // Each non-null value must be explained in the comment, thus appear exactly
  // once -- if in doubt, let's better return.
  for( int i = 1; isEnum && i < static_cast<int>( mValues.size() ); ++i )
    if( histogram[ i ] != 1 )
      isEnum = false;

  // We consider this a boolean parameter.
  if( isEnum && lowRange == 0 && highRange == 1
      && histogram[ 0 ] == 0 && histogram[ 1 ] == 1 )
  {
    if( mValues.size() > 1 )
      modifiedComment = mValues[ 1 ];
    mValues.resize( 2 );
    mValues[ 0 ] = "no";
    mValues[ 1 ] = "yes";
  }

  if( mValues.size() != size_t( highRange - lowRange + 1 ) )
    isEnum = false;

  if( isEnum && mValues.size() > 0 && mValues[ 0 ] == "" )
    mValues[ 0 ] = "none";

  // Each of the other labels must now be non-empty.
  for( size_t i = 1; isEnum && i < mValues.size(); ++i )
    if( mValues[ i ].empty() )
      isEnum = false;

  if( isEnum )
  {
    mIndexBase = lowRange;
    mComment = modifiedComment;
  }
  else
    mValues.clear();
  return isEnum;
}

bool
ParsedComment::IsBooleanEnum() const
{
  if( mIndexBase != 0 )
    return false;
  if( mValues.size() != 2 )
    return false;
  if( mValues[ 0 ] != "no" && mValues[ 0 ] != "No" )
    return false;
  if( mValues[ 1 ] != "yes" && mValues[ 1 ] != "Yes" )
    return false;
  return true;
}

