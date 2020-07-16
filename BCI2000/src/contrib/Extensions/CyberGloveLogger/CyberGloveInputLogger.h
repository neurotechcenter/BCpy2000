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

#ifndef CyberGloveInputLoggerH
#define CyberGloveInputLoggerH

#include "GenericFilter.h"
#include "Environment.h"
#include "Thread.h"

#include <vhtBase.h>

#include <vhtCyberGlove.h>
#include <vhtIOConn.h>
#include <vhtBaseException.h>

#include <list>
#include <string>
using namespace std;

typedef vhtIOConn*        (*CGSGETDEFAULT)                   (const char    *key);
typedef bool              (*CGSCONNECT)                       ();
typedef bool              (*CGSDISCONNECT)                    ();
typedef int              (*CGSGETCONNECTSTATUS)               ();
typedef double           (*CGSGETRAWDATA)                   (GHM::HandParts handPart, GHM::Joints aJoint);

const int MAX_SENSORS = 22;
const int SENSOR_PRECISION = 8;



class CyberGloveInputLogger;
class CyberGloveThread;

class CyberGloveThread : public Thread
{
	public:
				            CyberGloveThread(const CyberGloveInputLogger *logger, bool deriv, int nOfGloves, int iGlove);
		virtual             ~CyberGloveThread();
		virtual int         OnExecute() override;

		virtual std::string GetError();
		virtual int         GetHandedness();
		virtual void        SetIndex(int i);
		virtual void        Cleanup();


	private:
		const CyberGloveInputLogger      *mpLogger;
		vhtIOConn						  *gloveDict;
		vhtCyberGlove					  *glove;
		std::stringstream                 m_err;
		int                         m_hand;
		int                         m_sensors;
		int                         m_index;
		bool                        m_deriv;
		double					mPreviousReading[23];
	    double					mPreviousOutput[23];
	    //uint8_t				mPreviousReading[23];
	    //uint8_t				mPreviousOutput[23];

		
};
typedef std::list<CyberGloveThread*> CyberGloveThreadList;
void DisplayMessage(const char *msg, vhtBaseException *e);
int checkKey(HKEY tree, const char *folder, char *key);

class CyberGloveInputLogger : public EnvironmentExtension
{
	public:
		        CyberGloveInputLogger();
		virtual ~CyberGloveInputLogger();
		virtual void Publish();
		virtual void Preflight() const;
		virtual void Initialize();
		virtual void StartRun();
		virtual void StopRun();
		virtual void Halt();

		virtual void OpenInterface();
		virtual int  CountGloves() const;
		virtual void GetGloves(CyberGloveThreadList & available) const;
		virtual void AssignGloves(CyberGloveThreadList & assigned) const;
		virtual void EmptyList(CyberGloveThreadList & gloves) const;

		

		

	private:

		HINSTANCE						   m_hinstLib;
		CGSGETDEFAULT                     cgsGetDefaultCall;
		CGSCONNECT                        cgsConnectCall;
		CGSDISCONNECT                     cgsDisconnectCall;
		CGSGETCONNECTSTATUS               cgsGetConnectStatusCall;
		CGSGETRAWDATA                     cgsGetRawDataCall;

		bool                        m_enabled;
		bool                        m_deriv;
		CyberGloveThreadList         mGloves;

		

	friend class CyberGloveThread;
};

#endif // CyberGloveInputLoggerH
