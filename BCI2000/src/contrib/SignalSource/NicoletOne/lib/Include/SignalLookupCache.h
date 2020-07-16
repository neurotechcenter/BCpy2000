// SignalLookupCache.h: interface for the CSignalLookupCache class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIGNALLOOKUPCACHE_H__1E5B33B1_8644_4E3B_8E67_35C87F3DF8ED__INCLUDED_)
#define AFX_SIGNALLOOKUPCACHE_H__1E5B33B1_8644_4E3B_8E67_35C87F3DF8ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SignalName.h>

class CSignalLookupCache  
{
public:
	CSignalLookupCache();
	~CSignalLookupCache();

public:
   ULONG Lookup(const WCHAR* pszSignal) const;
   bool Check(const WCHAR* pszSignal) const;

   void Set(const WCHAR* pszSignal, ULONG lIndex);

   void Clear();
   void Erase(const WCHAR* pszSignal);

private:
   std::map<CSignalName, ULONG> m_mapCache;
};

#endif // !defined(AFX_SIGNALLOOKUPCACHE_H__1E5B33B1_8644_4E3B_8E67_35C87F3DF8ED__INCLUDED_)
