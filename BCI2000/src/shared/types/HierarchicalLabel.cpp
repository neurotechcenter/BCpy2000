////////////////////////////////////////////////////////////////////////////////
// $Id: HierarchicalLabel.cpp 4731 2014-07-03 15:18:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that handles hierarchical labels, such as a parameter's
//   section label, or a signal's channel labels.
//   Hierarchy levels are separated by a single ':' character.
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

#include "HierarchicalLabel.h"
#include <sstream>

using namespace std;

bool
HierarchicalLabel::operator<( const HierarchicalLabel& s ) const
{
  for( size_t i = 0; i < size() && i < s.size(); ++i )
    if( ( *this )[ i ] < s[ i ] )
      return true;
    else if( s[ i ] < ( *this )[ i ] )
      return false;
  return size() < s.size();
}

istream&
HierarchicalLabel::ExtractFrom( istream& is )
{
  clear();
  EncodedString value;
  is >> value;
  istringstream iss( value );
  while( getline( iss, value, cLevelDelimiter ) )
    push_back( value );
  return is;
}

ostream&
HierarchicalLabel::InsertInto( ostream& os ) const
{
  if( empty() )
    os << EncodedString( "" );
  else
    os << ( *this )[ 0 ];
  for( size_t i = 1; i < size(); ++i )
    os << cLevelDelimiter << ( *this )[ i ];
  return os;
}
