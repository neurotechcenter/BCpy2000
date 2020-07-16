#ifndef __SENSORINFO_H_
#define __SENSORINFO_H_

#include <InfoPacket.h>

class CSignalLookupCache;

class CSensorInfo : public CInfoPacket<NOINFO, SENSORINFO>
{
public:
   CSensorInfo();
   virtual ~CSensorInfo();

public:
   LONG FindSensor(const WCHAR* pszSensor) const;
   const SENSORINFO* GetSensorInfoFromName(const WCHAR* pszName) const;
   bool Union(PACKET pPacket);

private:
   CSignalLookupCache* const m_pCache;
};

inline const SENSORINFO* CSensorInfo::GetSensorInfoFromName(const WCHAR* pwcSensor) const
{
   long liSensor = FindSensor(pwcSensor);
   if (liSensor > -1)
      return &GetElement(liSensor);

   return NULL;
}

#endif
