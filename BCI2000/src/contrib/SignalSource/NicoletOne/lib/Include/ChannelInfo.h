#ifndef __CHANNELINFO_H_
#define __CHANNELINFO_H_

#include <InfoPacket.h>
#include <InfoStructures.h>

class CSignalInfo;

class CChannelInfo : public CInfoPacket<CHANNELHEADER, CHANNELINFO>
{
public:
   void SetDeviceID(GUID guidDeviceID);
   GUID GetDeviceID() const;

   double GetMaxSamplingRate() const;
   ULONG CountChannelsOn() const;

   const CHANNELINFO* GetChannelInfoFromInputID(LONG lInputID) const;
   long FindChannelInfoFromSensor(const WCHAR* pszSensor) const;

   ULONG CountChannelsOfType(const GUID& guidType, const CSignalInfo& cSignalInfo) const;
};

#endif
