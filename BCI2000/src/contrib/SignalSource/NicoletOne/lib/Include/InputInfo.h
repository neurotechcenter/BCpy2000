#ifndef __INPUTINFO_H_
#define __INPUTINFO_H_

#include <InfoPacket.h>
#include <InfoStructures.h>

class CInputInfo : public CInfoPacket<NOINFO, INPUTINFO>
{
public:
   CInputInfo();

public:
   void SetDeviceID(GUID guidDeviceID);
   GUID GetDeviceID() const;

   const INPUTINFO* GetInputInfoFromID(long lInputID) const;
   LONG FindInputFromIndex(LONG lInputIndex) const;
};

#endif
