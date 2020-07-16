// MFCdemo.h : Haupt-Header-Datei für die Anwendung MFCDEMO
//

#if !defined(AFX_MFCDEMO_H__84343563_3F97_4804_B2CD_924D5BDF6726__INCLUDED_)
#define AFX_MFCDEMO_H__84343563_3F97_4804_B2CD_924D5BDF6726__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// Hauptsymbole

/////////////////////////////////////////////////////////////////////////////
// CMFCdemoApp:
// Siehe MFCdemo.cpp für die Implementierung dieser Klasse
//

class CMFCdemoApp : public CWinApp
{
public:
	CMFCdemoApp();

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CMFCdemoApp)
	public:
	//}}AFX_VIRTUAL

// Implementierung

	//{{AFX_MSG(CMFCdemoApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


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
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_MFCDEMO_H__84343563_3F97_4804_B2CD_924D5BDF6726__INCLUDED_)
