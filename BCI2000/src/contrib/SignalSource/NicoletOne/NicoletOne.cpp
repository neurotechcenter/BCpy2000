// NicoletOne.cpp
//

#include "StdAfx.h"
#include "NicoletOne.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNicoletOne

BEGIN_MESSAGE_MAP(CNicoletOne, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNicoletOne Constructor

CNicoletOne::CNicoletOne()
{
}

/////////////////////////////////////////////////////////////////////////////
// The only CNicoletOne Object

CNicoletOne theApp;

/////////////////////////////////////////////////////////////////////////////
// CNicoletOne InitInstance

BOOL CNicoletOne::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
		return FALSE;
	return TRUE;
}
