////////////////////////////////////////////////////////////////////////////////
// $Id: LabelIndex.h 4970 2015-08-21 16:46:07Z mellinger $
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
#ifndef LABEL_INDEX_H
#define LABEL_INDEX_H

#include <iostream>
#include <map>
#include <vector>
#include "EncodedString.h"
#include "PhysicalUnit.h"

class LabelIndex
{
  struct Comp
  { bool operator()( const std::string& a, const std::string& b ) const
    { return stricmp( a.c_str(), b.c_str() ) < 0; }
  };
  class LabelProxy : public std::string
  {
  private:
    LabelProxy( LabelIndex*, size_t );
    LabelIndex* mParent;
    int mIndex;
    friend class LabelIndex;
  public:
    LabelProxy& operator=( const std::string& );
  };

  typedef std::map<EncodedString, int, Comp> IndexBase;
  typedef std::vector<IndexBase::key_type> IndexReverse;

 public:
  LabelIndex();
  LabelIndex( const PhysicalUnit& );
  ~LabelIndex() {}

  // Forward lookup.
  int operator[]( const std::string& ) const;
  // We need an additional Exists() function because operator[] must return 0
  // for nonexisting labels.
  bool Exists( const std::string& ) const;
  // A reverse lookup operator.
  std::string operator[]( size_t ) const;
  LabelProxy operator[]( size_t );
  // More complex mapping.
  double AddressToIndex( const std::string&, const PhysicalUnit& = PhysicalUnit() ) const;

  bool operator==( const LabelIndex& ) const;

  bool IsTrivial() const;
  static std::string TrivialLabel( size_t );

  // Stream I/O.
  std::ostream& InsertInto( std::ostream& ) const;
  std::istream& ExtractFrom( std::istream& );

  LabelIndex& Clear() { return Resize( 0 ); }
  LabelIndex& Resize( size_t );
  int Size() const { return mSize; }
  LabelIndex& operator*=( const LabelIndex& );

 private:
  static int TrivialToIndex( const std::string& );
  void Expand();
  void Shrink();
  void Sync() const;
  void Reset();

 private:
  bool mKnownToBeTrivial;
  int mSize;
  // This is the maintained index.
  IndexReverse mReverseIndex;
  // This is a cache for the more probable lookup direction.
  mutable bool mNeedSync;
  mutable IndexBase mForwardIndex;
};

inline
std::ostream& operator<<( std::ostream& os, const LabelIndex& l )
{ return l.InsertInto( os ); }

inline
std::istream& operator>>( std::istream& is, LabelIndex& l )
{ return l.ExtractFrom( is ); }

#endif // LABEL_INDEX_H

