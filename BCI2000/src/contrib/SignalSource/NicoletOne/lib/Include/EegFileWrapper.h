// EegFileWrapper.h: interface for the CEegFileWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EEGFILEWRAPPER_H__C1AE2C63_6CCC_48EC_9E52_330D9205C5A9__INCLUDED_)
#define AFX_EEGFILEWRAPPER_H__C1AE2C63_6CCC_48EC_9E52_330D9205C5A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef _ASSERT
#define _ASSERT(expr) ((void)0)
#endif
#ifndef _ASSERTE
#define _ASSERTE(expr) ((void)0)
#endif

#include "nrvdate.h"

#pragma warning (disable: 4786)
#include <map>

#include "hcEventTypes.h"
#include "infopacketguids.h"
#include "dbinfo.h"
#include "signalinfo.h"
#include "tsinfo.h"
#include "eventtypeinfo.h"
#include "datapacket.h"
#include "impedancemarker.h"
#include "sensorinfo.h"
#include "vsyncinfo.h"

#include "nrvtime.h"
#include "eventmarker.h"
#include "fileaccess3.h"


template <class T>
class CInfoHelper
{
public:
   CInfoHelper() {}
   virtual ~CInfoHelper() {}
   bool GetStaticInfo(GUID guid, IStorageMan* pStorage, T* const pt) const;
   bool GetDynamicInfo(GUID guid, IStorageMan* pStorage, long liSegment, T* const pt) const;
};

class CEegFileWrapper  
{
public:
	CEegFileWrapper();
	virtual ~CEegFileWrapper();
   bool Open(BSTR bstrFile, unsigned long ulMask);
   bool Close();
   bool GetSignalInfo(CSignalInfo* const psi) const;
   bool GetTSInfo(CTSInfo* const ptsi, long liSegment) const;
   bool GetSensorInfo(CSensorInfo* const psei) const;
   bool GetSyncInfo(CVSyncInfo* const psei) const;
   bool GetPatientInfo(CPatientInfo* const ppi) const;
   bool GetTestInfo(CTestInfo* const pti) const;
   bool GetStudyInfo(CStudyInfo* const psti) const;
   bool GetUsersInfo(CUsersInfo* const pui) const;
   long QuerySegmentCount() const;
   double QuerySegmentDuration(long liSegment) const;
   NRVDATE QuerySegmentStartTime(long liSegment) const;
   NRVDATE QuerySegmentEndTime(long liSegment) const;
   long QueryNrvDateToSegment(const NRVDATE& nrv) const;
   bool QueryRecSecToNrvDate(const double dRecSec, NRVDATE& nvr) const;
   bool QueryNrvDateToRecSec(const NRVDATE& nvr, double* pdRecSec) const;
   bool GetDataPacket(const NRVDATE& nrv, double dDur, CDataPacket* const pData) const;
   long GetEventCount() const;
   bool GetEvent(long liEvent, CEventMarker* const pEvent) const;
   bool GetEventTypeInfo(CEventTypeInfo* const peti) const;
   bool GetImpedance(CImpedanceMarker* const pim, const GUID& guid) const;
   bool InitializeStorageMan();

private:
   bool _CheckSegmentConstraints(const NRVDATE& nrv, double dDur) const;

public:
   CComPtr<IStorageManEx> m_pStorage;
};

#endif // !defined(AFX_EEGFILEWRAPPER_H__C1AE2C63_6CCC_48EC_9E52_330D9205C5A9__INCLUDED_)
