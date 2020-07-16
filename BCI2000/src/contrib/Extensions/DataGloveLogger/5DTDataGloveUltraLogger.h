//////////////////////////////////////////////////////////////////////
// $Id: 5DTDataGloveUltraLogger.h 5818 2018-11-09 15:50:15Z mellinger $
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

#ifndef DataGloveLoggerH
#define DataGloveLoggerH

#include <Windows.h>
#include "Environment.h"
#include "Thread.h"

//#include "fglove.h"
#include "fglove.imports.h"

#include <list>
#include <string>
using namespace std;

/*
typedef fdGlove*        (*FDOPEN)                   (char    *pPort);
typedef int             (*FDCLOSE)                  (fdGlove *pFG  );
typedef int             (*FDGETGLOVETYPE)           (fdGlove *pFG  );
typedef int             (*FDGETNUMSENSORS)          (fdGlove *pFG  );
typedef int             (*FDGETGLOVEHAND)           (fdGlove *pFG  );
typedef int             (*FDSCANUSB)                (unsigned short *aPID,  int &nNumMax);
typedef void            (*FDGETSENSORRAWALL)        (fdGlove *pFG,          unsigned short *pData);
typedef unsigned short  (*FDGETSENSORRAW)           (fdGlove *pFG,          int nSensor);
*/

const int MAX_SENSORS = 14;
const int SENSOR_PRECISION = 12;

class DataGloveLogger;
class DataGloveThread;

class DataGloveThread : public Thread
{
	public:
				            DataGloveThread(const DataGloveLogger *logger, int usbPortNumber, bool deriv);
		virtual             ~DataGloveThread();
		virtual int         OnExecute() override;

		virtual std::string GetError();
		virtual int         GetHandedness();
		virtual void        SetIndex(int i);
		virtual void        Cleanup();

	private:
		const DataGloveLogger      *mpLogger;
		fdGlove                    *mpGlove;
		std::stringstream           m_err;
		std::string                 m_portString;
		int                         m_portNumber;
		int                         m_type;
		int                         m_hand;
		int                         m_sensors;
		int                         m_index;
		bool                        m_deriv;
		unsigned short              mPreviousReading[MAX_SENSORS];
		unsigned short              mPreviousOutput[MAX_SENSORS];
};
typedef std::list<DataGloveThread*> DataGloveThreadList;

class DataGloveLogger : public EnvironmentExtension
{
	public:
		        DataGloveLogger();
		virtual ~DataGloveLogger();
		virtual void Publish();
		virtual void Preflight() const;
		virtual void Initialize();
		virtual void StartRun();
		virtual void StopRun();
		virtual void Halt();

		virtual void OpenInterface();
		virtual int  CountGloves() const;
		virtual void GetGloves(DataGloveThreadList & available) const;
		virtual void AssignGloves(DataGloveThreadList & assigned) const;
		virtual void EmptyList(DataGloveThreadList & gloves) const;

	private:
/*
		HINSTANCE                   m_hinstLib;
		FDOPEN                      m_fdOpenCall;
		FDCLOSE                     m_fdCloseCall;
		FDSCANUSB                   m_fdScanUSBCall;
		FDGETGLOVETYPE              m_fdGetGloveTypeCall;
		FDGETGLOVEHAND              m_fdGetGloveHandCall;
		FDGETNUMSENSORS             m_fdGetNumSensorsCall;
		FDGETSENSORRAWALL           m_fdGetSensorRawAllCall;
		FDGETSENSORRAW              m_fdGetSensorRawCall;
*/
		bool                        m_enabled;
		bool                        m_deriv;
		DataGloveThreadList         mGloves;

	friend class DataGloveThread;
};

#endif // DataGloveLoggerH
