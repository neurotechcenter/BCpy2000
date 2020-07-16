////////////////////////////////////////////////////////////////////////////////
// $Id: NicoletThread.cpp 4536 2013-08-05 14:30:13Z mellinger $
// Author: griffin.milsap@gmail.com
// Description: NicoletOne ADC Acquisition Thread
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
#include "StdAfx.h"
#include "NicoletThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Declare that this is an implementation of a windows thread.
IMPLEMENT_DYNCREATE( CNicoletThread, CWinThread )

// Constructor
CNicoletThread::CNicoletThread() :
	mDr( NULL ),
	mClive( NULL ),
	mConnected( false )
{
}

// Deconstructor
CNicoletThread::~CNicoletThread()
{
}

// Initialize the thread and communications
BOOL CNicoletThread::InitInstance()
{
	// Initialize communications
	mClive = new CLiveData();
	mDr = new CSocketImplDR();
	CString location = "192.168.101.2";
	mClive->SetSocket(mDr);
	BSTR bstr = location.AllocSysString();
	mConnected = mClive->ConnectToComputer(bstr, 6050) ? 0 : 1;
	return mConnected;
}

int CNicoletThread::ExitInstance()
{
	// Cleanup
	if( mClive ) delete mClive;
	mClive = NULL;
	if( mDr ) delete mDr; // This closes the socket automatically
	mDr = NULL;

	// Return OK
	return CWinThread::ExitInstance();
}

// Mutex Locked Extraction of Data from thread
bool CNicoletThread::ExtractData( int Channels, int DataBlockSize, float** DataBlock, bool &check )
{
	// Extract data from the object
	if( !mConnected )
		return false;

	// Grab the data
	return mClive->GetData( Channels, DataBlockSize, DataBlock, check );
}

// Expose CLiveData functions
void CNicoletThread::ClearBufferedData()
{
	mClive->ClearBufferedData();
}

bool CNicoletThread::GetSampleRate( double *rate )
{
	return mClive->GetSampleRate( rate );
}

bool CNicoletThread::GetNumChannels( int *channels )
{
	return mClive->GetNumChannels( channels );
}

bool CNicoletThread::GetDCOffset( double*& offset )
{
	return mClive->GetDCOffset( offset );
}

bool CNicoletThread::GetDCResolution( double*& resolution )
{
	return mClive->GetDCResolution( resolution );
}

void CNicoletThread::StartLogging()
{
	mClive->StartLogging();
}

void CNicoletThread::StopLogging()
{
	mClive->StopLogging();
}

BEGIN_MESSAGE_MAP(CNicoletThread, CWinThread)
	//{{AFX_MSG_MAP(CNicoletThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()





