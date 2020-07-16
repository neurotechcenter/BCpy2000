// SignalInfo.h: interface for the CSignalInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIGNALINFO_H__1938E4C5_89B7_4838_B7EE_4CB1E4C62087__INCLUDED_)
#define AFX_SIGNALINFO_H__1938E4C5_89B7_4838_B7EE_4CB1E4C62087__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <InfoPacket.h>

class CSignalLookupCache;

class CSignalInfo : public CInfoPacket<NOINFO, SIGNALINFO>
{
public:
	CSignalInfo();
	virtual ~CSignalInfo();

public:
   const SIGNALINFO* GetSignalInfoFromName(const WCHAR* pszSignal) const;
   LONG FindSignal(const WCHAR* pszSignal) const;
   GUID GetSignalTypeID(const WCHAR* pszSignal) const;
   bool Union(PACKET pPacket);

private:
   CSignalLookupCache* const m_pCache;
};

inline GUID CSignalInfo::GetSignalTypeID(const WCHAR* pszSignal) const
{
   long liSignal = FindSignal(pszSignal);
   if (liSignal < 0)
      return GUID_NULL;

   return GetElement(liSignal).guidSignalType;
}

inline const SIGNALINFO* CSignalInfo::GetSignalInfoFromName(const WCHAR* pszSignal) const
{
   long liSignal = FindSignal(pszSignal);
   if (liSignal < 0)
      return NULL;

   return &GetElement(liSignal);
}

#endif // !defined(AFX_SIGNALINFO_H__1938E4C5_89B7_4838_B7EE_4CB1E4C62087__INCLUDED_)
