#ifndef __IMPEDANCEMARKER_H_
#define __IMPEDANCEMARKER_H_

#include <EventMarker.h>

// THINK: should be in register - usersettings-nervus3.0-recorder-imptest
extern const ULONG THRESHOLDLEVELS[];
extern const ULONG NUMTHRESHOLDS;
ULONG ThresholdToIndex(ULONG iLevel);

struct IMPEDANCEMARKERLAYOUT
{
   EVENTPACKET em;
   IMPTESTHEADER header;
   IMPTESTEVENTINFO elems[1]; // Actually unlimited
};

// The elements in the impedancemarker should correspond one-to-one to
// the inputs on the device

//////////////////////////////////////////////////////////////////////
// CImpedanceMarker
//////////////////////////////////////////////////////////////////////
class CImpedanceMarker : public CEventMarker
{
public:
   virtual bool CheckPacket(PACKET pPacket) const;
   virtual void SetText(const WCHAR* wcText);

   void PreparePacket(GUID guidDeviceID, ULONG lcInputs);
   ULONG GetInputCount() const;
   ULONG CountActiveInputs() const;

   GUID GetDeviceID() const;

   ULONG GetThreshold() const;
   void SetThreshold(ULONG lThreshold);

   void SetImpTestInfo(ULONG liInput, const IMPTESTEVENTINFO& val);
   const IMPTESTEVENTINFO& GetImpTestInfo(ULONG liInput) const;

   IMPTESTEVENTINFO& operator[](ULONG liInput);
   const IMPTESTEVENTINFO& operator[](ULONG liInput) const;

private:
   IMPEDANCEMARKERLAYOUT* _GetImpLayout();
   const IMPEDANCEMARKERLAYOUT* _GetImpLayout() const;
};


#endif
