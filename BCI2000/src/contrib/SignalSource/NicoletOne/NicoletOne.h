// NicoletOne.h
// 

#if !defined(AFX_NICOLETONE_H_INCLUDED)
#define AFX_NICOLETONE_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// Symbols

class CNicoletOne : public CWinApp
{
  public:
	CNicoletOne();
	virtual BOOL InitInstance();

  public:
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_NICOLETONE_H_INCLUDED)
