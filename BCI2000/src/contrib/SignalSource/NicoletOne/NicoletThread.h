////////////////////////////////////////////////////////////////////////////////
// $Id: NicoletThread.h 4536 2013-08-05 14:30:13Z mellinger $
// Author: griffin.milsap@gmail.com
// Description: MFC based Threaded object for NicoletOne Signal Acquisition
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
#if !defined(AFX_NICOLETTHREAD_H_INCLUDED)
#define AFX_NICOLETTHREAD_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "CommonDecl.h"
#include "SocketImplDR.h"
#include "resource.h"		// Symbols

class CNicoletThread : public CWinThread
{
  public:
    // We must declare this as a dynamically created object for MFC
    DECLARE_DYNCREATE( CNicoletThread )

	CNicoletThread();
	~CNicoletThread();

	// Virtual Interface
	virtual int ExitInstance();
	virtual BOOL InitInstance();

	// Interface for LiveData
	void ClearBufferedData();
	bool GetSampleRate( double *rate );
	bool GetNumChannels( int *channels );
	bool GetDCOffset( double*& offset );
	bool GetDCResolution( double*& resolution );
	bool GetSampleBlockSize( int &size );
	void StartLogging();
	void StopLogging();

	// Mutex locked method for extraction of data from thread
	bool ExtractData( int Channels, int DataBlockSize, float** DataBlock, bool &check );
	bool Connected() { return mDr->IsConnected(); }

  private:
	// Private Member Variables
	CSocketImplDR *mDr;
	CLiveData *mClive;
	bool mConnected;

  protected:
    // We might have a message map.
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_NICOLETONE_H_INCLUDED
