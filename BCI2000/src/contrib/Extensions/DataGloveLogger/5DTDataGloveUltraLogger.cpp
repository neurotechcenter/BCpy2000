//////////////////////////////////////////////////////////////////////
// $Id: 5DTDataGloveUltraLogger.cpp 5818 2018-11-09 15:50:15Z mellinger $
// Authors: Peter Brunner (pbrunner@wadsworth.org), Jeremy Hill (jezhill@gmail.com)
// Description: BCI2000 v3.0+ Logger for 5DT DataGlove Ultra
// 
// Version History
// 
//  05/17/2007 pbrunner: Initial version (as Filter, not Logger);
//  01/14/2009 pbrunner: Added support for the 14 sensor data glove.
//  01/20/2011 jhill:    Revamped into 5DTDataGloveUltraLogger.cpp
//                       Converted from Filter to Logger
//                       Made v.3.0-compatible
//                       Added support for multiple gloves
//                          (NB: This entailed changes to Parameter and State names) 
//  08/17/2015 pbrunner: Made compatible to new platform independent dynamic linking
//                       Update to 5DTDataGloveUltra SDK v2.3
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
#include "5DTDataGloveUltraLogger.h"
#include <cstdio>
#include <iomanip>

Extension( DataGloveLogger );

DataGloveLogger::DataGloveLogger()
{
	m_enabled                 = false;
	m_deriv                   = false;
}
DataGloveLogger::~DataGloveLogger()
{
	EmptyList(mGloves);
}
void
DataGloveLogger::OpenInterface()
{
	if(!m_enabled) {
		if(CountGloves()) bciout << "One or more datagloves are connected, but disabled. To enable acquisition, start the source module with --LogDataGlove=1" << endl;
		return;
	}
}

int
DataGloveLogger::CountGloves() const
{
	unsigned short aPID[5];
	int nGloves, nNumMax = 5;
	nGloves = fdScanUSB(aPID, nNumMax);
	return nGloves;
}

void
DataGloveLogger::GetGloves(DataGloveThreadList & available) const
{
	EmptyList(available);
	int nGloves = CountGloves();
	for(int iGlove = 0; iGlove < nGloves; iGlove++) {
		DataGloveThread *g = new DataGloveThread(this, iGlove, m_deriv);
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
DataGloveLogger::AssignGloves(DataGloveThreadList & assigned) const
{
	DataGloveThreadList available;
	GetGloves(available);
	ParamRef handedness = Parameter("DataGloveHandedness"); 
	int nGlovesRequested = handedness->NumValues();
	for(int iRequest = 0; iRequest < nGlovesRequested; iRequest++) {
		string sWanted = handedness(iRequest);
		int hWanted;
		DataGloveThread *found = NULL;
		if(     sWanted == "L" || sWanted == "l" || sWanted == "left"  || sWanted == "LEFT"  || sWanted == "0") hWanted = FD_HAND_LEFT;
		else if(sWanted == "R" || sWanted == "r" || sWanted == "right" || sWanted == "RIGHT" || sWanted == "1") hWanted = FD_HAND_RIGHT;
		else {
			EmptyList(available);
			EmptyList(assigned);
			bcierr << "unrecognized DataGloveHandedness string \"" << sWanted << "\"" << endl;
		}
		for(DataGloveThreadList::iterator g=available.begin(); g != available.end(); g++) {
			if( (*g)->GetHandedness() == hWanted ) { found = *g; break; }
		}
		if(found) {
			available.remove(found);
			assigned.push_back(found);
			found->SetIndex(iRequest+1);
		}
		else {
			EmptyList(available);
			EmptyList(assigned);
			bcierr << "could not find a match for requested glove #" << iRequest+1 << " (handedness requested: " << sWanted << ")" << endl; 
		}
	}
	EmptyList(available);
}

void
DataGloveLogger::EmptyList(DataGloveThreadList & gloves) const
{
	while(gloves.size()) {delete gloves.back(); gloves.pop_back();}
}

void
DataGloveLogger::Publish()
{
	char definition[32];

	m_enabled = ( ( int )OptionalParameter( "LogDataGlove" ) != 0 );
	if( !m_enabled ) return;

	OpenInterface();
	int nGloves = CountGloves(); // needed in order to know how many states to declare.
	string h;
	if(nGloves == 1) { // if 1 glove is connected, since we have this info, helpfully set the "default" value of DataGloveHandedness to the correct value
		DataGloveThreadList g;
		GetGloves(g);
		h = ((g.back()->GetHandedness() == FD_HAND_LEFT) ? "L" : "R");
		EmptyList(g);
	}

	for(int iGlove = 0; iGlove < nGloves; iGlove++) {
		if(nGloves > 1)
			h = ((iGlove%2)?" L ":" R ") + h; // If an even number of gloves is attached, the helpful default will end up as L R L R... If an odd number of gloves > 1 is attached, it will end up as R L R .... 
		for(int iSensor = 0; iSensor < MAX_SENSORS; iSensor++) {
			sprintf(definition, "Glove%dSensor%02d %d 0 0 0", iGlove+1, iSensor+1, SENSOR_PRECISION);
			BEGIN_EVENT_DEFINITIONS
				definition,
			END_EVENT_DEFINITIONS
		}
	}
	sprintf(definition, "%d", nGloves);
	h = "Source:Log%20Input list    DataGloveHandedness= " + (definition+h) + " // DataGlove handedness: L or R for each glove";
	BEGIN_PARAMETER_DEFINITIONS
		"Source:Log%20Input int     DataGloveDerivative=   0    0  0 1 // measure changes in glove signals?: 0: no - measure position, 1:yes - measure velocity (enumeration)",
		"Source:Log%20Input int     LogDataGlove=          0    0  0 1 // record DataGlove to states (boolean)",
		h.c_str(), // "Source:Log%20Input list    DataGloveHandedness= 1 L    R  % % // DataGlove handedness: L or R for each glove",
	END_PARAMETER_DEFINITIONS
}

void
DataGloveLogger::Preflight() const
{
	bool enabled = ( ( int )OptionalParameter( "LogDataGlove" ) != 0 );
	if(enabled) {
		DataGloveThreadList assigned;
		AssignGloves(assigned);
		EmptyList(assigned); // <sigh>
	}

}

void
DataGloveLogger::Initialize()
{
	m_enabled = ( ( int )OptionalParameter( "LogDataGlove" ) != 0 );
	m_deriv = m_enabled && ( (int)Parameter("DataGloveDerivative") != 0 ); 
}

void
DataGloveLogger::StartRun()
{
	if(m_enabled) {
		AssignGloves(mGloves);
	}
	for(DataGloveThreadList::iterator g = mGloves.begin(); g != mGloves.end(); g++)
		(*g)->Start();
}

void
DataGloveLogger::StopRun()
{
	for(DataGloveThreadList::iterator g = mGloves.begin(); g != mGloves.end(); g++)
		(*g)->Terminate();
	EmptyList(mGloves);
}

void
DataGloveLogger::Halt()
{
	StopRun();
}



DataGloveThread::DataGloveThread(const DataGloveLogger *parent, int usbPortNumber, bool deriv)
{
	m_err.str("");
	for(int i = 0; i < MAX_SENSORS; i++) mPreviousReading[i] = mPreviousOutput[i] = 0;
	
	char ps[10];
	sprintf(ps, "USB%i", usbPortNumber); 
	
	mpLogger       = parent;
	m_portNumber   = usbPortNumber;
	m_portString   = ps;
	m_index        = 0;
	mpGlove        = fdOpen(ps);
	m_deriv        = deriv;
	if(mpGlove == NULL) {
		m_type = FD_GLOVENONE;
		m_hand = -1;
		m_sensors = -1;
		m_err << "failed to find a data glove on " << m_portString;
	}
	else {
		m_type         = fdGetGloveType(mpGlove);
		m_hand         = fdGetGloveHand(mpGlove);
		m_sensors     = fdGetNumSensors(mpGlove);
		if (m_type != FD_GLOVE5U_USB && m_type != FD_GLOVE14U_USB) {
			m_err << "unrecognized data glove type on " << m_portString << endl;
		}
	}
}

void
DataGloveThread::Cleanup()
{
	if(mpGlove != NULL && mpLogger != NULL) {
		fdClose(mpGlove);
		mpGlove = NULL;
	}
	m_type = FD_GLOVENONE;
	m_hand = -1;
	m_sensors = -1;
}

DataGloveThread::~DataGloveThread()
{
	Cleanup();
}

void
DataGloveThread::SetIndex(int i)
{
	m_index = i;
}

int
DataGloveThread::GetHandedness()
{
	return m_hand;
}

string
DataGloveThread::GetError()
{
	return m_err.str();
}

int
DataGloveThread::OnExecute()
{
	unsigned short currentData[MAX_SENSORS];
	while( !Terminating() ) {
		for(int i = 0; i < MAX_SENSORS; i++) currentData[i] = 0;

		currentData[0]  = fdGetSensorRaw(mpGlove, FD_THUMBNEAR);
		currentData[1]  = fdGetSensorRaw(mpGlove, FD_INDEXNEAR);
		currentData[2]  = fdGetSensorRaw(mpGlove, FD_MIDDLENEAR);
		currentData[3]  = fdGetSensorRaw(mpGlove, FD_RINGNEAR);
		currentData[4]  = fdGetSensorRaw(mpGlove, FD_LITTLENEAR);

		if (m_type == FD_GLOVE14U_USB) {

			currentData[5]  = fdGetSensorRaw(mpGlove, FD_THUMBFAR);
			currentData[6]  = fdGetSensorRaw(mpGlove, FD_INDEXFAR);
			currentData[7]  = fdGetSensorRaw(mpGlove, FD_MIDDLEFAR);
			currentData[8]  = fdGetSensorRaw(mpGlove, FD_RINGFAR);
			currentData[9]  = fdGetSensorRaw(mpGlove, FD_LITTLEFAR);
			currentData[10] = fdGetSensorRaw(mpGlove, FD_THUMBINDEX);
			currentData[11] = fdGetSensorRaw(mpGlove, FD_INDEXMIDDLE);
			currentData[12] = fdGetSensorRaw(mpGlove, FD_MIDDLERING);
			currentData[13] = fdGetSensorRaw(mpGlove, FD_RINGLITTLE);
		} 
		if(m_deriv) {
			for(int i = 0; i < MAX_SENSORS; i++) {
				double diff = (double)currentData[i] - (double)mPreviousReading[i];
				mPreviousReading[i] = currentData[i];
				long min = -(1L << (SENSOR_PRECISION-1));
				long max =  (1L << (SENSOR_PRECISION-1)) - 1;
				diff = (diff < min ? min : diff > max ? max : diff);
				currentData[i] = (unsigned short)(diff - min);
			}
		}

		for(int i = 0; i < MAX_SENSORS; i++) {
			if(currentData[i] != mPreviousOutput[i])
				bcievent << "Glove" << m_index << "Sensor" << setfill('0') << setw(2) << (i+1) << " " << currentData[i];
			mPreviousOutput[i] = currentData[i];
		}
		::Sleep(1);
	}
	::Sleep(10);
	return 0;
}
