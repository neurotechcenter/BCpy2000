/////////////////////////////////////////////////////////////////////////////
//
// File: PresErrors.h
//
// Date: Oct 15, 2001
//
// Author: Juergen Mellinger
//
// Description:
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

#ifndef PRESERRORSH
#define PRESERRORSH

#include <sstream>

typedef enum TPresError
{
    firstPresError = 0,
    presNoError = 0,
    presGenError,
    presParamInaccessibleError,
    presParamOutOfRangeError,
    presFileOpeningError,
    presIllegalSpellerTreeError,
    presResNotFoundError,

    presNumErrors
} TPresError;

class TGUITextDialog;

class TErrorReport : public std::ostringstream
{
  public:
            TErrorReport();
            TErrorReport(   const char  *inWindowTitle );
            ~TErrorReport();

    void    AddError(   TPresError  inError,
                        const char  *inTextArgument );
    void    AddError(   TPresError  inError,
                        const char  *inTextArgument,
                        float       inNumArgument );

    void    DisplayErrors();

  private:
    std::string     windowTitle;
    TGUITextDialog  *errorDialog;
};

extern  TErrorReport    gPresErrors;

#endif // PRESERRORSH
