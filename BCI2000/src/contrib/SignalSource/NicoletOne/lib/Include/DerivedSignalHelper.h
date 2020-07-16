// DerivedSignalHelper.h: interface for the CDerivedSignalHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DERIVEDSIGNALHELPER_H__2C2879BE_B4C3_4327_B2BC_56ACBA58EB67__INCLUDED_)
#define AFX_DERIVEDSIGNALHELPER_H__2C2879BE_B4C3_4327_B2BC_56ACBA58EB67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <InfoStructures.h>

class CDerivedSignalInfo;
class CDerivedSignalHelper;

class CDerivedSignal
{
   friend class CDerivedSignalHelper;

public:
   explicit CDerivedSignal(const WCHAR* pszName = L"");

public:
   const WCHAR* GetSignalName() const;

   ULONG GetSourceCount() const;
   const WCHAR* GetSourceName(ULONG liSource) const;
   double GetSourceWeight(ULONG liSource) const;

private:
   WCHAR m_szSignalName[LABELSIZE];

   std::vector<DERIVEDSIGNALINFO> m_vecSources;
};

class CDerivedSignalHelper
{
public:
   explicit CDerivedSignalHelper(const CDerivedSignalInfo& cDerivedSignalInfo);

public:
   ULONG GetDerivedSignalCount() const;
   const CDerivedSignal& operator[](ULONG liSignal) const;

private:
   std::vector<CDerivedSignal> m_vecSignals;
};

//////////////////////////////////////////////////////////////////////
// Inline function definitions
//////////////////////////////////////////////////////////////////////

inline const WCHAR* CDerivedSignal::GetSignalName() const
{
   return m_szSignalName;
}

inline ULONG CDerivedSignal::GetSourceCount() const
{
   return m_vecSources.size();
}

inline const WCHAR* CDerivedSignal::GetSourceName(ULONG liSource) const
{
   return m_vecSources[liSource].szSourceSignalName;
}

inline double CDerivedSignal::GetSourceWeight(ULONG liSource) const
{
   _ASSERTE(liSource < m_vecSources.size());
   return m_vecSources[liSource].dWeight;
}

inline const CDerivedSignal& CDerivedSignalHelper::operator[](ULONG liSignal) const
{
   _ASSERTE(liSignal < m_vecSignals.size());
   return m_vecSignals[liSignal];
}

inline ULONG CDerivedSignalHelper::GetDerivedSignalCount() const
{
   return m_vecSignals.size();
}

#endif // !defined(AFX_DERIVEDSIGNALHELPER_H__2C2879BE_B4C3_4327_B2BC_56ACBA58EB67__INCLUDED_)
