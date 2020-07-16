/////////////////////////////////////////////////////////////////////////////
//
// File: PresView.h
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

#ifndef PRES_VIEW_H
#define PRES_VIEW_H

#include "PresErrors.h"
#include "PresBroadcasting.h"
#include "SWGUI.h"

class ParamList;

class TPresView : public TPresListener
{
  public:
                        TPresView( ParamList *inParamList )
                        : curParamList( inParamList ) {}
    virtual             ~TPresView() {}
    virtual TPresError  Initialize(       ParamList *inParamList,
                                    const TGUIRect  &inRect ) = 0;

  protected:
            ParamList   *curParamList;
};

#endif // PRES_VIEW_H
