////////////////////////////////////////////////////////////////////////////////
// $Id: WINMAIN.CPP 4536 2013-08-05 14:30:13Z mellinger $
// Author:  juergen.mellinger@uni-tuebingen.de
// Description: A modified WinMain.cpp for MFC projects that implement a BCI2000
//   application module.
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
#include "stdafx.h"
#include "CoreModule.h"

#ifdef AFX_CORE1_SEG
#pragma code_seg(AFX_CORE1_SEG)
#endif

// Derive a CoreModule subclass that implements ProcessGUIMessages()
// as appropriate for MFC.
class CoreModuleMFC : public CoreModule
{
 public:
	CoreModuleMFC() {}
	virtual ~CoreModuleMFC() {}

 private:
	CoreModuleMFC( const CoreModuleMFC& );
	CoreModuleMFC& operator=( const CoreModuleMFC& );

  virtual bool GUIMessagesPending()
  {
	return ::GetQueueStatus( QS_ALLINPUT );
  }
  
  virtual void ProcessGUIMessages()
  {
    // Adapted from MFC's CWinThread::Run() (thrdcore.cpp).
	_AFX_THREAD_STATE* pState = AfxGetThreadState();
	CWinApp* pApp = AfxGetApp();
    BOOL bIdle = TRUE;
    LONG lIdleCount = 0;
    // phase1: check to see if we can do idle work
    while (bIdle &&
        !::PeekMessage(&pState->m_msgCur, NULL, NULL, NULL, PM_NOREMOVE))
    {
      // call OnIdle while in bIdle state
      if (!pApp->OnIdle(lIdleCount++))
        bIdle = FALSE; // assume "no idle" state
    }
    // phase2: pump messages while available
    while (::PeekMessage(&pState->m_msgCur, NULL, NULL, NULL, PM_NOREMOVE))
    {
      // pump message, but quit on WM_QUIT
      if (!pApp->PumpMessage())
      {
        Terminate();
        pApp->ExitInstance();
      }
    }
    ::Sleep(0);
  }
};

/////////////////////////////////////////////////////////////////////////////
// Modified from standard MFC AfxWinMain() implementation.
// Basically, the modified version replaces the call to CWinThread::Run()
// by a call to CoreModuleMFC::Run().
//

int AFXAPI AfxWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine, int nCmdShow)
{
	ASSERT(hPrevInstance == NULL);

	int nReturnCode = -1;
	CWinThread* pThread = AfxGetThread();
	CWinApp* pApp = AfxGetApp();

	// AFX internal initialization
	bool InitSuccess = AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    // App global initializations (rare)
	if (pApp != NULL)
      InitSuccess = InitSuccess && pApp->InitApplication();

	// Perform specific initializations
	InitSuccess = InitSuccess && pThread->InitInstance();

    // Run the BCI2000 core module.
	if( InitSuccess )
	{
		// Start BCI2000
		CoreModuleMFC coreModule;
        nReturnCode = ( coreModule.Run( __argc, __argv ) ? 0 : -1 );
	}
	else
    {
		if (pThread->m_pMainWnd != NULL)
		{
			TRACE0("Warning: Destroying non-NULL m_pMainWnd\n");
			pThread->m_pMainWnd->DestroyWindow();
		}
	}
    pThread->ExitInstance();

#ifdef _DEBUG
	// Check for missing AfxLockTempMap calls
	if (AfxGetModuleThreadState()->m_nTempMapLock != 0)
	{
		TRACE1("Warning: Temp map lock count non-zero (%ld).\n",
			AfxGetModuleThreadState()->m_nTempMapLock);
	}
	AfxLockTempMaps();
	AfxUnlockTempMaps(-1);
#endif

	AfxWinTerm();
	return nReturnCode;
}

/////////////////////////////////////////////////////////////////////////////
