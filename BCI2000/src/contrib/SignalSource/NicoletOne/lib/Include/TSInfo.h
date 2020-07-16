#ifndef __TSINFO_H_
#define __TSINFO_H_

#include <InfoPacket.h>
#include <InfoStructures.h>

class CTSInfo : public CInfoPacket<TSHEADER, TSINFO>
{
public:
   CTSInfo();

public:
   double GetNotchValue() const;
   void SetNotchValue(double dNotch);
   double GetMaxSamplingRate() const;

   WCHAR* GetOriginalLabel(long liIndex, WCHAR* pszLabel) const;

   LONG GetIndexFromLabel(const WCHAR* pszLabel) const;
   LONG FindTsIndex(const WCHAR* pszActiveSensor) const;
   LONG FindIndex(const WCHAR* pszString) const;
};

#endif
