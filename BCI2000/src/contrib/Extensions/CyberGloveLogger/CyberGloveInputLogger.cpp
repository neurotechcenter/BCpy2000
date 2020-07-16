//////////////////////////////////////////////////////////////////////
// $Id: $
// Author: Sidhant Sharma (sidhant.sharma@gmail.com)
// Description: BCI2000 CyberGlove Input Logger v1.0
// 
// Version History
// 
//  03/21/2014 ssharma: Initial version v1.0
//                      Support for 18, 22 sensor gloves
//                      Maximum 2 gloves supported
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
///////////////////////////////////////////////////////////////////////

#include "PCHIncludes.h"
#pragma hdrstop

#include "BCIEvent.h"
#include "CyberGloveInputLogger.h"
#include <cstdio>
#include <iomanip>



Extension( CyberGloveInputLogger );

CyberGloveInputLogger::CyberGloveInputLogger()
{
  m_hinstLib                = NULL;
  cgsGetDefaultCall       = NULL;
  cgsConnectCall         = NULL;
  cgsDisconnectCall       = NULL;
  cgsGetConnectStatusCall  = NULL;
  cgsGetRawDataCall       = NULL;
  m_enabled                 = false;
  m_deriv                   = false;
  
  
  
}

CyberGloveInputLogger::~CyberGloveInputLogger()
{
	EmptyList(mGloves);

}

void
CyberGloveInputLogger::OpenInterface()
{
	m_hinstLib = NULL;
	m_hinstLib = LoadLibrary(TEXT("CGS_VirtualHandDevice.dll"));
	if (m_hinstLib == NULL) {
		if(m_enabled) bcierr << "Failed to find CGS_VirtualHandDevice.dll" << endl;
		return;
	}

	bool good = true;
	good &= ( cgsGetDefaultCall               = (CGSGETDEFAULT)           GetProcAddress(m_hinstLib, TEXT("?getDefault@vhtIOConn@@SAPAV1@PBD@Z")) ) != NULL;
	good &= ( cgsConnectCall                  = (CGSCONNECT)              GetProcAddress(m_hinstLib, TEXT("?connect@vhtCyberGlove@@UAE_NXZ")) ) != NULL;
	good &= ( cgsDisconnectCall               = (CGSDISCONNECT)           GetProcAddress(m_hinstLib, TEXT("?disconnect@vhtDevice@@UAE_NXZ")) ) != NULL;
	good &= ( cgsGetConnectStatusCall         = (CGSGETCONNECTSTATUS)     GetProcAddress(m_hinstLib, TEXT("?getConnectStatus@vhtDevice@@UAE?AW4Status@1@XZ")) ) != NULL;
	good &= ( cgsGetRawDataCall               = (CGSGETRAWDATA)           GetProcAddress(m_hinstLib, TEXT("?getRawData@vhtCyberGlove@@UAENW4Fingers@GHM@@W4Joints@3@@Z")) ) != NULL;
	
	

	if(!m_enabled) {
		if(CountGloves()) bciout << "One or more CyberGloves connected, but disabled. To enable acquisition, start the source module with --LogCyberGlove=1" << endl;
		return;
	}
	if(!good) {
		if(m_enabled) bcierr << "Not all the required functions were found in the dynamic link library CGS_VirtualHandDevice.dll" << endl;
		return;
	}
}


void DisplayMessage(const char *msg, vhtBaseException *e)
{
	char buf[200];
	strcpy(buf, msg);
	strcat(buf, e->getMessage());
	MessageBox(NULL, buf, "Message", 0);
	exit(0);
}

int checkKey(HKEY tree, const char *folder, char *key) {
	long lRet;
	HKEY hKey;
	char temp[150];
	DWORD dwBufLen;
	// Open location
	lRet = RegOpenKeyEx( tree, folder, 0, KEY_QUERY_VALUE, &hKey );
	if (lRet != ERROR_SUCCESS)
		return 0;
	// Get key
	dwBufLen = sizeof(temp);
	lRet = RegQueryValueEx( hKey, key, NULL, NULL, (BYTE*)&temp, &dwBufLen );
	if (lRet != ERROR_SUCCESS)
		return 0;
	printf("Key value: %s\n", temp);
	// Close key
	lRet = RegCloseKey( hKey );
	if (lRet != ERROR_SUCCESS)
		return 0;
	// Got this far, then key exists
	return 1;
}



int
CyberGloveInputLogger::CountGloves() const
{   
	// *Adding support for Maximum 2 gloves at the moment (Left and Right). 
	// *Naming conevntion for gloves in DCU:
	//                                      Right handed cyberglove: RightGlove
	//                                      Left handed cyberglove: LeftGlove
	
	//*18, 22 sensor gloves supported
	//*Wired as well as Wireless versions will work as long as they are live in the CyberGlove DCU


	int nGloves=0;
	
	//Check for CyberGlove (Win-32 bit) Drivers Installation
	if (checkKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\CyberGlove Systems\\VirtualHand Drivers", "Device Registry File"))
	 {//drivers installed
	 }
	else
	 {//drivers not installed
	  bcierr << "CyberGlove drivers not installed!! Please install CyberGloveSystems 32 bit drivers!!" <<endl;
	  return 0;
	 }
	

	//counting the number of gloves connected/active in the CyberGlove DCU interface. CyberGlove's DCU has to be running beforehand with gloves connected and live
	vhtIOConn *gloveConnRight;
	       
	gloveConnRight = vhtIOConn::getDefault( "RightGlove" );
			 
	if(gloveConnRight!=NULL)
			{ nGloves+=1;
				}
	

	vhtIOConn *gloveConnLeft;

	gloveConnLeft = vhtIOConn::getDefault( "LeftGlove" );
	if(gloveConnLeft!=NULL)
				{ nGloves+=1;
				}
	

		//if(nGloves==0) {nGloves=0; bciout << "Max 2 CyberGloves supported."<<endl<<" Naming Convention: RightGlove and LeftGlove in CyberGlove DCU accordingly." <<endl;}
	if(nGloves>2) {nGloves=0; bciout << "Max 2 CyberGloves supported." <<endl;}
	return nGloves;
}

void
CyberGloveInputLogger::GetGloves(CyberGloveThreadList & available) const
{
	EmptyList(available);
	int nGloves = CountGloves();
	
	for(int iGlove = 0; iGlove < nGloves; iGlove++) {
		
		CyberGloveThread *g = new CyberGloveThread(this,m_deriv,nGloves,iGlove);
		available.push_back(g);
		string err = g->GetError();
		if(err.size()) {
			EmptyList(available);
			bcierr << err.c_str() << endl;
			return;
		}
	}
}

void
CyberGloveInputLogger::AssignGloves(CyberGloveThreadList & assigned) const
{
	CyberGloveThreadList available;
	GetGloves(available);
	ParamRef handedness = Parameter("DataGloveHandedness"); 
	int nGlovesRequested = handedness->NumValues();
	for(int iRequest = 0; iRequest < nGlovesRequested; iRequest++) {
		string sWanted = handedness(iRequest);
		int hWanted;

		int hand=-5;

		CyberGloveThread *found = NULL;
		if(     sWanted == "L" || sWanted == "l" || sWanted == "left"  || sWanted == "LEFT"  || sWanted == "0") hWanted = 0;
		else if(sWanted == "R" || sWanted == "r" || sWanted == "right" || sWanted == "RIGHT" || sWanted == "1") hWanted = 1;
		else {
			EmptyList(available);
			EmptyList(assigned);
			bcierr << "unrecognized DataGloveHandedness string \"" << sWanted << "\"" << endl;
		}
		
		for(CyberGloveThreadList::iterator g=available.begin(); g != available.end(); g++) {
			
			if( (*g)->GetHandedness() == hWanted ) { found = *g;  hand = (*g)->GetHandedness(); break; }
		}

		if(found) {
			available.remove(found);
			assigned.push_back(found);
			found->SetIndex(iRequest+1);
		}
		else {
			EmptyList(available);
			EmptyList(assigned);
			bcierr << "could not find a match for requested cyberglove #" << iRequest+1 << " (handedness requested: " << sWanted << ")" <<endl;// 
		}
	}
	EmptyList(available);

}


void
CyberGloveInputLogger::EmptyList(CyberGloveThreadList & gloves) const
{
	while(gloves.size()) {delete gloves.back(); gloves.pop_back();}
}


void
CyberGloveInputLogger::Publish()
{
	char definition[32];

	m_enabled = ( ( int )OptionalParameter( "LogCyberGlove" ) != 0 );
	if( !m_enabled ) return;

	OpenInterface();
	int nGloves = CountGloves(); // needed in order to know how many states to declare.
	string h;
	if(nGloves == 1) { // if 1 glove is connected, since we have this info, helpfully set the "default" value of DataGloveHandedness to the correct value
		CyberGloveThreadList g;
		GetGloves(g);
		h = ((g.back()->GetHandedness() == 0) ? "L" : "R");
		EmptyList(g);
	}

	for(int iGlove = 0; iGlove < nGloves; iGlove++) {
		if(nGloves > 1)
			h = ((iGlove%2)?" R ":" L ") + h; // If 2 gloves are attached, the helpful default will end up as R L .   
		for(int iSensor = 0; iSensor < (MAX_SENSORS+1); iSensor++) {
			
			sprintf(definition, "CyberGlove%dSensor%02d %d 0 0 0", iGlove+1, iSensor+1, SENSOR_PRECISION);
			BEGIN_EVENT_DEFINITIONS
				definition,
			END_EVENT_DEFINITIONS
		}
	}
	sprintf(definition, "%d", nGloves);
	h = "Source:Log%20Input list    DataGloveHandedness= " + (definition+h) + " // DataGlove handedness: R or L for 1 glove. R L or L R for 2 gloves (order is not important)";
	BEGIN_PARAMETER_DEFINITIONS
		"Source:Log%20Input int     DataGloveDerivative=   0    0  0 1 // measure changes in glove signals?: 0: no - measure position, 1:yes - measure velocity (enumeration)",
		"Source:Log%20Input int     LogCyberGlove=          0    0  0 1 // record CyberGlove to states (boolean)",
		h.c_str(), // "Source:Log%20Input list    DataGloveHandedness= 1 L    R  % % // DataGlove handedness: L or R for each glove",
	END_PARAMETER_DEFINITIONS

}

void
CyberGloveInputLogger::Preflight( ) const
{
   bool enabled = ( ( int )OptionalParameter( "LogCyberGlove" ) != 0 );
	 if(enabled) {
		CyberGloveThreadList assigned;
		AssignGloves(assigned);
		EmptyList(assigned); 
	  }

}


void
CyberGloveInputLogger::Initialize()
{
  m_enabled = ( ( int )OptionalParameter( "LogCyberGlove" ) != 0 );
  m_deriv = m_enabled && ( (int)Parameter("DataGloveDerivative") != 0 ); 
}

void
CyberGloveInputLogger::StartRun()
{
	if(m_enabled) {
		AssignGloves(mGloves);
	}
	for(CyberGloveThreadList::iterator g = mGloves.begin(); g != mGloves.end(); g++)
		(*g)->Start();
}

void
CyberGloveInputLogger::StopRun()
{
	for(CyberGloveThreadList::iterator g = mGloves.begin(); g != mGloves.end(); g++)
		(*g)->Terminate();
	EmptyList(mGloves);
}

void
CyberGloveInputLogger::Halt()
{
	StopRun();
}


//DATAGLOVE THREAD CLASS

CyberGloveThread::CyberGloveThread(const CyberGloveInputLogger *parent, bool deriv, int nOfGloves, int iGlove)
{
	m_err.str("");
	for(int i = 0; i < (MAX_SENSORS+1); i++) mPreviousReading[i] = mPreviousOutput[i] = 0;
	
	mpLogger       = parent;
	m_index        = 0;
	m_deriv        = deriv;


	// for 1 glove
	if(nOfGloves==1){
	try
	  {
		// Get the default glove connection object
		gloveDict = vhtIOConn::getDefault( "RightGlove" );
		m_hand = 1;
		if(gloveDict==NULL)
		{gloveDict = vhtIOConn::getDefault( "LeftGlove" );
		m_hand = 0;}
		glove = new vhtCyberGlove(gloveDict);
	    glove->connect();
	    
	  }
	catch ( vhtBaseException * e )
	  {
		m_hand = -1;
		m_sensors = -1;
		m_err << "failed to connect to cyberglove";
		DisplayMessage(" Error with CyberGlove Connection or Name. For 1 glove, reconnect as RightGlove or LeftGlove. For 2 gloves, reconnect as RightGlove and LeftGlove respectively. ", e);
		
	  }
	
	}

	//for 2 gloves
	else if(nOfGloves==2){
		if(iGlove==0){
	try
	{
		// Get the default glove connection object
		gloveDict = vhtIOConn::getDefault( "RightGlove" );

		glove = new vhtCyberGlove(gloveDict);
	    glove->connect();
	    m_hand = 1;
	}
	catch ( vhtBaseException * e )
	{
		m_hand = -1;
		m_sensors = -1;
		m_err << "failed to connect to cyberglove";
		DisplayMessage(" Error with RH CyberGlove Connection or Name. If RH glove connects in DCU, rename as RightGlove. ", e);
		
	}
	}
		if(iGlove==1){
	try
	{
		// Get the default glove connection object
		gloveDict = vhtIOConn::getDefault( "LeftGlove" );

		glove = new vhtCyberGlove(gloveDict);
	    glove->connect();
	    m_hand = 0;
	}
	catch ( vhtBaseException * e )
	{
		m_hand = -1;
		m_sensors = -1;
		m_err << "failed to connect to cyberglove";
		DisplayMessage(" Error with LH CyberGlove Connection or Name. If LH glove connects in DCU, rename as LeftGlove. ", e);
		
	}
	}
	}
	
	else
	{m_hand = -1;
	 m_sensors = -1;}

}




void
CyberGloveThread::Cleanup()
{
	if(glove != NULL && mpLogger != NULL && (glove->getConnectStatus()==1)){ 
		glove->disconnect();
		gloveDict= NULL;
		glove = NULL;
	}
	m_hand = -1;
	m_sensors = -1;
	
	
}

CyberGloveThread::~CyberGloveThread()
{
	Cleanup();
}

void
CyberGloveThread::SetIndex(int i)
{
	m_index = i;
}

int
CyberGloveThread::GetHandedness()
{
	return m_hand;
}

string
CyberGloveThread::GetError()
{
	return m_err.str();
}

int
CyberGloveThread::OnExecute()
{
	
	double currentData[MAX_SENSORS+1];
	while( !Terminating() ) {
		
		for( int i = 0; i < (MAX_SENSORS+1); i++ )
		{currentData[i]=0;
		}

		for( int finger = 0; finger < GHM::nbrFingers; finger++ )
		{

			// For every joint on this finger
			for( int joint = 0; joint < GHM::nbrJoints; joint++ )
			{
				// Get the joint angle
				currentData[(4*finger)+joint] = glove->getRawData( (GHM::Fingers)finger, (GHM::Joints)joint );
			}

			// Get the abduction value
			currentData[(4*finger)+3] = glove->getRawData( (GHM::Fingers)finger, GHM::abduct );

			
		}

		// Get PA, WF and WA
		currentData[20]= glove->getRawData( GHM::palm, GHM::palmArch );
		currentData[21]= glove->getRawData( GHM::palm, GHM::wristFlexion );
		currentData[22]= glove->getRawData( GHM::palm, GHM::wristAbduction );


		if(m_deriv) {
			for(int i = 0; i < (MAX_SENSORS+1); i++) {
				double diff = (double)currentData[i] - (double)mPreviousReading[i];
				mPreviousReading[i] = currentData[i];
				long min = -(1L << (SENSOR_PRECISION-1));
				long max =  (1L << (SENSOR_PRECISION-1)) - 1;
				diff = (diff < min ? min : diff > max ? max : diff);
				currentData[i] = (unsigned short)(diff - min);
			}
		}

		for(int i = 0; i < (MAX_SENSORS+1); i++) { 
			if(currentData[i] != mPreviousOutput[i])
				bcievent << "CyberGlove" << m_index << "Sensor" << setfill('0') << setw(2) << (i+1) << " " << currentData[i];
			mPreviousOutput[i] = currentData[i];
		}
		::Sleep(1);
	}
	::Sleep(10);
	return 0;
}
