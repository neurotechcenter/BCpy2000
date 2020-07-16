/////////////////////////////////////////////////////////////////////////////
//
// File: TextProperties.h
//
// Date: Jan 7, 2002
//
// Author: Juergen Mellinger
//
// Description: A mix-in class for views that handle text properties.
//
// Changes:
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
//////////////////////////////////////////////////////////////////////////////

#ifndef TEXT_PROPERTIES_H
#define TEXT_PROPERTIES_H

#include <string>

#include "PresErrors.h"
#include "GUITextProperties.h"

class   Param;

typedef std::string TInputSequence;
typedef std::string TDisplaySequence;

class TTextProperties : protected TGUITextProperties
{
  protected:
                TTextProperties();
                ~TTextProperties() {}

  public:
    // Use this for setting font properties from a
    // parameter string.
    TPresError  SetTextProperties( const Param  *inParamPtr );

    // Use this to convert strings between input and display sequence
    // for right-to-left and bidirectional languages.
    const TInputSequence&   DisplayToInput( const TDisplaySequence& ) const;
    const TDisplaySequence& InputToDisplay( const TInputSequence& ) const;

  private:
    TPresError  ParseTextProperties( const char* inProperties );
};

#endif // TEXT_PROPERTIES_H
