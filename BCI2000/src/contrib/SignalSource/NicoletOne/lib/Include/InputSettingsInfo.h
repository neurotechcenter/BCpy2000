#ifndef __INPUTSETTINGSINFO_H_
#define __INPUTSETTINGSINFO_H_

#include <InfoPacket.h>
#include <InfoStructures.h>

class CInputSettingsInfo : public CInfoPacket<NOINFO, INPUTSETTINGSINFO>
{
public:
   CInputSettingsInfo();

public:
   const INPUTSETTINGSINFO* GetInputSettingsInfoFromID(long lInputID) const;
};

#endif
