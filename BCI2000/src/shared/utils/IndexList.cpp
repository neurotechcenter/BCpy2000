////////////////////////////////////////////////////////////////////////////////
// $Id: IndexList.cpp 4536 2013-08-05 14:30:13Z mellinger $
// Authors: juergen.mellinger@uni-tuebingen.de
// Description: A class that represents a list of indices, and is able to
//   parse itself from a list of user-defined expressions that represent
//   label patterns, or ranges.
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

#include "IndexList.h"
#include "WildcardMatch.h"
#include <sstream>

using namespace std;
using namespace bci;

bool
IndexList::Parse( const std::string& inList, const LabelIndex& inLabels, const PhysicalUnit& inUnit )
{
  this->clear();
  mErrors.clear();

  istringstream iss( inList );
  string entry;
  while( iss >> ws >> entry )
  {
    bool negated = ( entry[0] == '!' );
    size_t pos = entry.find_first_of( ":-" );
    if( negated || pos == string::npos )
    { // single address entry, use pattern matching
      int matches = 0;
      for( int idx = 0; idx < inLabels.Size(); ++idx )
      {
        if( WildcardMatch( entry, inLabels[idx] ) != negated )
        {
          this->push_back( idx );
          ++matches;
        }
      }
      if( matches == 0 )
      {
        double idx = inLabels.AddressToIndex( entry, inUnit );
        if( idx >= 0 )
          this->push_back( static_cast<int>( idx ) );
      }
    }
    else
    { // range of addresses, no pattern matching
      string addr1 = entry.substr( 0, pos ),
             addr2 = entry.substr( pos + 1 );
      double idx1 = addr1.empty() ? 0 : inLabels.AddressToIndex( addr1, inUnit ),
             idx2 = addr2.empty() ? inLabels.Size() - 1 : inLabels.AddressToIndex( addr2, inUnit );
      if( idx1 < 0 )
        mErrors += addr1 + ": not a valid index or label\n";
      if( idx2 < 0 )
        mErrors += addr2 + ": not a valid index or label\n";
      if( idx1 >= 0 && idx2 >= 0 )
        for( int idx = static_cast<int>( idx1 ); idx <= static_cast<int>( idx2 ); ++idx )
          this->push_back( idx );
    }
  }
  return mErrors.empty();
}

