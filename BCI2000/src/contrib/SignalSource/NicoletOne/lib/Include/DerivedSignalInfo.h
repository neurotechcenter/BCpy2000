// DerivedSignalInfo.h: interface for the CDerivedSignalInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DERIVEDSIGNALINFO_H__085988A4_2DC8_4723_94B5_8E6E042A7A25__INCLUDED_)
#define AFX_DERIVEDSIGNALINFO_H__085988A4_2DC8_4723_94B5_8E6E042A7A25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <InfoPacket.h>

class CDerivedSignalInfo : public CInfoPacket<NOINFO, DERIVEDSIGNALINFO>
{
public:
	CDerivedSignalInfo();
	virtual ~CDerivedSignalInfo();

public:
   bool Union(PACKET pPacket);
};


#endif // !defined(AFX_DERIVEDSIGNALINFO_H__085988A4_2DC8_4723_94B5_8E6E042A7A25__INCLUDED_)
