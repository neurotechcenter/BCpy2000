////////////////////////////////////////////////////////////////////////////////
// $Id: TaskFilter.h 3798 2012-02-01 18:07:06Z mellinger $
// Author:  juergen.mellinger@uni-tuebingen.de
// Description: A simple task filter that illustrates how to implement a
//   BCI2000 application module based on Microsoft's MFC library.
//
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
#ifndef TASK_FILTER_H
#define TASK_FILTER_H

#include "StdAfx.h"
#include "GenericFilter.h"
#include "MFCdemo.h"
#include "MFCdemoDlg.h"

class TaskFilter : public GenericFilter
{
  public:
            TaskFilter();
    virtual ~TaskFilter();

    virtual void Preflight( const SignalProperties&, SignalProperties& ) const;
    virtual void Initialize( const SignalProperties&, const SignalProperties& );
    virtual void Process( const GenericSignal& Input, GenericSignal& Output );
    virtual void StartRun();
    virtual void StopRun();
	virtual bool AllowsVisualization() const { return false; }

  private:
    double      mCursorSpeed;
    CMFCdemoDlg mWindow;
} ;

#endif // TASK_FILTER_H
