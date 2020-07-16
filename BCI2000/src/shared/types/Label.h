////////////////////////////////////////////////////////////////////////////////
// $Id: Label.h 4731 2014-07-03 15:18:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A class that centralizes handling of visualization labels.
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
#ifndef LABEL_H
#define LABEL_H

#include <iostream>
#include <string>
#include <vector>
#include "EncodedString.h"
#include "ValueList.h"

class Label
{
  public:
    Label()
      : mAddress( 0 )
      {}
    Label( int address, const std::string& text )
      : mAddress( address ),
        mText( text )
      {}
    virtual ~Label()
      {}

    int                Address() const
                       { return mAddress; }
    const std::string& Text() const
                       { return mText; }

    std::ostream&      InsertInto( std::ostream& ) const;
    std::istream&      ExtractFrom( std::istream& );

  private:
    int           mAddress;
    EncodedString mText;
};

typedef ValueList<Label> LabelList;

inline
std::ostream& operator<<( std::ostream& s, const Label& l )
{ return l.InsertInto( s ); }

inline
std::istream& operator>>( std::istream& s, Label& l )
{ return l.ExtractFrom( s ); }

#endif // LABEL_H

