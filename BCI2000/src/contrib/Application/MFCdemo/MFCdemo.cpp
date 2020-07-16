// MFCdemo.cpp : Legt das Klassenverhalten für die Anwendung fest.
//

#include "stdafx.h"
#include "MFCdemo.h"
#include "MFCdemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFCdemoApp

BEGIN_MESSAGE_MAP(CMFCdemoApp, CWinApp)
	//{{AFX_MSG_MAP(CMFCdemoApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCdemoApp Konstruktion

CMFCdemoApp::CMFCdemoApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// Das einzige CMFCdemoApp-Objekt

CMFCdemoApp theApp;
