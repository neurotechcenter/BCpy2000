////////////////////////////////////////////////////////////////////////////////
// $Id: Lock.h 4536 2013-08-05 14:30:13Z mellinger $
// Author: telemacher - griffin.milsap@gmail.com
// Description: A Simple MFC Mutex Class - Modified from
//              http://www.codeproject.com/KP/threads/threadex.aspx
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

#if !defined(AFX_LOCK_H_INCLUDED_)
#define AFX_LOCK_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>

// A Simple Mutex Lock
class CLock
{
public:
	CLock(const CString & name);
	virtual ~CLock();
	bool Unlock();
	bool Lock(DWORD timeout = INFINITE);

private:
	HANDLE mutex;
};

#endif // AFX_LOCK_H_INCLUDED_
