////////////////////////////////////////////////////////////////////////////////
// $Id: VisTable.h 5145 2015-11-27 12:18:20Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A VisProperty class to represent a set of visualization
//   properties for a single visualization ID,
//   and a VisTable class to hold VisProperty sets for all existing
//   visualization IDs.
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
#ifndef VIS_TABLE_H
#define VIS_TABLE_H

#include <map>
#include <string>
#include <sstream>

#include "SignalProperties.h"
#include "defines.h"

typedef uint8_t IDType;

class VisProperties : public std::map<IDType, std::string>
{
 public:
  // The Kind property is one of "Graph", "Bitmap", or "Memo".
  std::string& Kind()
    { return mKind; }
  // Functions to Get/Put property values independent of type.
  template<typename T> bool Get( IDType, T& );
  template<typename T> bool Put( IDType, const T& );

 private:
  std::string mKind;
};

class VisTable : public std::map<std::string, VisProperties>
{
};


template<typename T>
bool
VisProperties::Get( IDType id, T& t )
{
  const_iterator i = find( id );
  if( i == end() )
    return false;
  std::istringstream is( i->second );
  if( is.str() != "" )
  {
    T value;
    if( is >> value )
      t = value;
  }
  return !is.fail();
}

template<typename T>
bool
VisProperties::Put( IDType id, const T& t )
{
  std::ostringstream os;
  os << t;
  ( *this )[ id ] = os.str();
  return !os.fail();
}

#endif // VIS_TABLE_H

