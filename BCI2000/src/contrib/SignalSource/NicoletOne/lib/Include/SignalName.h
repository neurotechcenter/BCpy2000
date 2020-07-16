#ifndef __SIGNALNAME_H_INCLUDED__
#define __SIGNALNAME_H_INCLUDED__

#include <TGCopy.h>
#include <InfoStructureSizes.h>

struct CSignalName
{
   WCHAR wcName[LABELSIZE];

   CSignalName(const WCHAR* pszSignal);

   operator const WCHAR*() const;
};

inline CSignalName::CSignalName(const WCHAR* pwcSensor)
{
   wszncpy(wcName, pwcSensor, LABELSIZE);
   _wcslwr(wcName);
}

inline CSignalName::operator const WCHAR*() const
{
   return wcName;
}

inline bool operator<(const CSignalName& sn1, const CSignalName& sn2)
{
   return wcsncmp(sn1, sn2, LABELSIZE) < 0;
}

inline bool operator==(const CSignalName& sn1, const CSignalName& sn2)
{
   return wcsncmp(sn1, sn2, LABELSIZE) == 0;
}

#endif
