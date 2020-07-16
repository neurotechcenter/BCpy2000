// SpikeEvent.h: interface for the CSpikeEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPIKEEVENT_H__997CF2C5_A026_11D3_BF8E_00105A9AFD56__INCLUDED_)
#define AFX_SPIKEEVENT_H__997CF2C5_A026_11D3_BF8E_00105A9AFD56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <EventMarker.h>

struct SPIKEEVENTLAYOUT
{
   EVENTPACKET sCommon;
   EVENTSpike sSpike[1]; // Actually unlimited;
};

class CSpikeEvent : public CEventMarker  
{
public:
	CSpikeEvent();
	virtual ~CSpikeEvent();

   void PreparePacket(LONG lNoOfEvents);
   void AddEvent (EVENTSpike stNewSpike);
	BOOL HasEvent();
	BOOL MoveToNextEvent();
	EVENTSpike* GetCurrentEvent();
	BOOL MoveToFirstEvent();
   long GetSpikeCount() const;

   virtual bool CheckPacket(PACKET pPacket) const;


protected:
   LONG m_lOffSet;
   LONG m_lcSpike;
};

#endif // !defined(AFX_SPIKEEVENT_H__997CF2C5_A026_11D3_BF8E_00105A9AFD56__INCLUDED_)
