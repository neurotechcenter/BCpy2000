////////////////////////////////////////////////////////////////////////////////
// $Id: HierarchicalLabel.h 4731 2014-07-03 15:18:03Z mellinger $
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
#ifndef HIERARCHICAL_LABEL_H
#define HIERARCHICAL_LABEL_H

#include <iostream>
#include <vector>
#include "EncodedString.h"

class HierarchicalLabel : public std::vector<EncodedString>
{
  public:
    static const char cLevelDelimiter = ':';

    HierarchicalLabel()
      : std::vector<EncodedString>( 1, "" )
      {}
    bool operator<( const HierarchicalLabel& ) const;
    // Stream I/O.
    std::ostream& InsertInto( std::ostream& ) const;
    std::istream& ExtractFrom( std::istream& );
};

inline
std::ostream& operator<<( std::ostream& s, const HierarchicalLabel& h )
{ return h.InsertInto( s ); }

inline
std::istream& operator>>( std::istream& s, HierarchicalLabel& h )
{ return h.ExtractFrom( s ); }

#endif // HIERARCHICAL_LABEL_H
