////////////////////////////////////////////////////////////////////////////////
// $Id: LabelIndex.cpp 5530 2016-09-14 19:42:39Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A helper class that represents string labels for indexing
//   matrix and list parameters, and signals.
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
#include "LabelIndex.h"
#include "Brackets.h"
#include "PhysicalUnit.h"
#include "Debugging.h"

using namespace std;

LabelIndex::LabelIndex()
: mNeedSync( false ),
  mKnownToBeTrivial( false ),
  mSize( 0 )
{
  Reset();
}

LabelIndex::LabelIndex( const PhysicalUnit& inP )
: mNeedSync( false ),
  mKnownToBeTrivial( false ),
  mSize( 0 )
{
  Reset();
  PhysicalUnit::ValueType range = inP.RawMax() - inP.RawMin();
  mReverseIndex.resize( static_cast<size_t>( ::fabs( range ) + 1 ) );
  if( range >= 0 )
  {
    for( int i = 0; i < range + 1; ++i )
      mReverseIndex[i] = inP.RawToPhysical( inP.RawMin() + i );
  }
  else
  {
    range = -range;
    for( int i = 0; i < range + 1; ++i )
      mReverseIndex[i] = inP.RawToPhysical( inP.RawMin() - i );
  }
  mSize = mReverseIndex.size();
  mNeedSync = true;
  mKnownToBeTrivial = false;
}

void
LabelIndex::Reset()
{
  Shrink();
  Resize( 1 );
}

double
LabelIndex::AddressToIndex( const string& inAddress,
                            const PhysicalUnit& inUnit ) const
{
  double result = -1;
  // Does the address match an existing label?
  if( Exists( inAddress ) )
    result = ( *this )[inAddress];
  // Is it a value in physical units?
  else if( inUnit.IsPhysical( inAddress ) )
    result = inUnit.PhysicalToRaw( inAddress );
  // If neither, then we interpret it as a 1-based index.
  else
  {
    istringstream is( inAddress );
    double number;
    if( is >> number && is.eof() )
    {
      number -= 1;
      if( number >= 0 && number < Size() )
        result = number;
    }
  }
  return result;
}

// **************************************************************************
// Function:   operator[]
// Purpose:    Maps string labels to numerical indices.
// Parameters: String label.
// Returns:    Numerical index associated with the label.
// **************************************************************************
int
LabelIndex::operator[]( const string& inLabel ) const
{
  if( mKnownToBeTrivial )
  {
    int idx = TrivialToIndex( inLabel );
    if( idx < 0 || idx >= mSize )
      return 0;
    return idx;
  }
  Sync();
  int retIndex = 0;
  IndexBase::iterator i = mForwardIndex.find( inLabel );
  if( i != mForwardIndex.end() )
    retIndex = i->second;
  return retIndex;
}

// **************************************************************************
// Function:   Exists
// Purpose:    Checks whether a given textual label exists in the index.
//             Needed because operator[] always returns a valid index.
// Parameters: String label.
// Returns:    Boolean value that indicates the existence of the argument.
// **************************************************************************
bool
LabelIndex::Exists( const string& inLabel ) const
{
  if( mKnownToBeTrivial )
  {
    int idx = TrivialToIndex( inLabel );
    return idx >= 0 && idx < mSize;
  }
  Sync();
  return mForwardIndex.find( inLabel ) != mForwardIndex.end();
}

// **************************************************************************
// Function:   operator[]
// Purpose:    Maps numerical indices to string labels.
// Parameters: String label.
// Returns:    String label associated with the index.
// **************************************************************************
string
LabelIndex::operator[]( size_t inIndex ) const
{
  Assert( inIndex < mSize );
  if( inIndex < mReverseIndex.size() )
     return mReverseIndex[inIndex];
  return TrivialLabel( inIndex );
}

LabelIndex::LabelProxy
LabelIndex::operator[]( size_t inIndex )
{
  Assert( inIndex < mSize );
  return LabelProxy( this, inIndex );
}

LabelIndex::LabelProxy::LabelProxy( LabelIndex* p, size_t idx )
: std::string( const_cast<const LabelIndex*>( p )->operator[]( idx ) ),
  mParent( p ),
  mIndex( idx )
{
}

LabelIndex::LabelProxy&
LabelIndex::LabelProxy::operator=( const std::string& s )
{
  Assert( mIndex < mParent->mSize );
  bool changed = ( s != *this );
  if( mParent->mKnownToBeTrivial && changed )
    mParent->Expand();
  if( changed )
  {
    mParent->mReverseIndex[mIndex] = s;
    mParent->mNeedSync = true;
  }
  return *this;
}

// **************************************************************************
// Function:   Resize
// Purpose:    Changes the number of labels managed by this Index.
// Parameters: New size.
// Returns:    N/A
// **************************************************************************
LabelIndex&
LabelIndex::Resize( size_t inNewSize )
{
  mSize = inNewSize;
  if( mKnownToBeTrivial )
    return *this;
  if( mForwardIndex.size() > 0 && mSize > mReverseIndex.size() )
    mNeedSync = true;
  mReverseIndex.reserve( mSize );
  while( mReverseIndex.size() < mSize )
    mReverseIndex.push_back( TrivialLabel( mReverseIndex.size() ) );
  mReverseIndex.resize( mSize );
  return *this;
}

// **************************************************************************
// Function:   Sync
// Purpose:    Rebuilds the forward index if the needSync flag is set.
// Parameters: N/A
// Returns:    N/A
// **************************************************************************
void
LabelIndex::Sync() const
{
  if( mNeedSync )
  {
    mForwardIndex.clear();
    for( size_t i = 0; i < mReverseIndex.size(); ++i )
      mForwardIndex[mReverseIndex[i]] = i;
    mNeedSync = false;
  }
}

// **************************************************************************
// Function:   TrivialLabel
// Purpose:    Return a trivial label associated with a given numerical index.
// Parameters: Numerical Index.
// Returns:    Label.
// **************************************************************************
static const int sTrivialBase = 1; // Channels are counted from 1,
                                   // so trivial labels should start with 1 to avoid
                                   // user confusion.
string
LabelIndex::TrivialLabel( size_t index )
{
  ostringstream oss;
  oss << index + sTrivialBase;
  return oss.str();
}

int
LabelIndex::TrivialToIndex( const std::string& s )
{
  istringstream iss( s );
  int i;
  if( iss >> i && iss.tellg() == streampos( s.length() ) )
    return i - sTrivialBase;
  return -1;
}

// **************************************************************************
// Function:   IsTrivial
// Purpose:    Test if labels actually contain information.
// Parameters: N/A
// Returns:    bool
// **************************************************************************
bool
LabelIndex::IsTrivial() const
{
  if( mKnownToBeTrivial )
    return true;
  Sync();
  bool trivial = true;
  for( size_t i = 0; trivial && i < mReverseIndex.size(); ++i )
    trivial &= ( mReverseIndex[i] == TrivialLabel( i ) );
  return trivial;
}

void
LabelIndex::Shrink()
{
  mKnownToBeTrivial = true;
  mReverseIndex.clear();
  mForwardIndex.clear();
}

void
LabelIndex::Expand()
{
  mKnownToBeTrivial = false;
  Resize( mSize );
}

// **************************************************************************
// Function:   operator*=
// Purpose:    Perform outer multiplication with another LabelIndex.
// Parameters: LabelIndex to multiply with.
// Returns:    Result of multiplication.
// **************************************************************************
LabelIndex&
LabelIndex::operator*=( const LabelIndex& inL )
{
  Expand();
  IndexReverse newLabels( mReverseIndex.size() * inL.Size() );
  for( size_t i = 0; i < mReverseIndex.size(); ++i )
    for( size_t j = 0; j < inL.Size(); ++j )
      newLabels[mReverseIndex.size() * i + j] = mReverseIndex[i] + '&' + inL[j];
  mReverseIndex = newLabels;
  mSize = mReverseIndex.size();
  mNeedSync = true;
  return *this;
}

bool
LabelIndex::operator==( const LabelIndex& other ) const
{
  if( mSize != other.mSize )
    return false;
  if( mKnownToBeTrivial )
    return other.IsTrivial();
  if( other.mKnownToBeTrivial )
    return this->IsTrivial();
  return mReverseIndex == other.mReverseIndex;
}

// **************************************************************************
// Function:   ExtractFrom
// Purpose:    Member function for formatted stream input of a single
//             label Index.
//             All formatted input functions are, for consistency's sake,
//             supposed to use this function.
// Parameters: Input stream to read from.
// Returns:    Input stream read from.
// **************************************************************************
istream&
LabelIndex::ExtractFrom( istream& is )
{
  Reset();
  if( is >> ws )
  {
    // Check if the first character is an opening bracket.
    char endDelimiter = Brackets::ClosingMatch( is.peek() );
    if( endDelimiter != '\0' )
    { // The first character is an opening bracket,
      // Get the line up to the matching closing bracket.
      is.get();
      string labelsList;
      if( getline( is, labelsList, endDelimiter ) )
      {
        mReverseIndex.clear();
        istringstream labels( labelsList );
        IndexBase::key_type currentToken;
        while( labels >> currentToken )
          mReverseIndex.push_back( currentToken );
        mSize = mReverseIndex.size();
        mKnownToBeTrivial = false;
        mNeedSync = true;
        if( IsTrivial() )
          Shrink();
      }
    }
    else
    { // There is no bracket, so let's read a plain number.
      size_t size;
      if( is >> size )
        Resize( size );
    }
  }
  return is;
}

// **************************************************************************
// Function:   InsertInto
// Purpose:    Member function for formatted stream output of a single
//             label Index.
//             All formatted output functions are, for consistency's sake,
//             supposed to use this function.
// Parameters: Output stream to write into.
// Returns:    Output stream written into.
// **************************************************************************
ostream&
LabelIndex::InsertInto( ostream& os ) const
{
  static const char disallowedCharacters[] = { Brackets::ClosingDefault, '\0' };
  if( IsTrivial() )
    os << Size();
  else
  {
    os << Brackets::OpeningDefault << ' ';
    for( size_t i = 0; i < mReverseIndex.size(); ++i )
    {
      mReverseIndex[i].InsertInto( os, disallowedCharacters );
      os << ' ';
    }
    os << Brackets::ClosingDefault;
  }
  return os;
}

