// SeizureEvent.h: interface for the CSeizureEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEIZUREEVENT_H__CCCE109A_7BCB_4079_AB41_8711C10B3392__INCLUDED_)
#define AFX_SEIZUREEVENT_H__CCCE109A_7BCB_4079_AB41_8711C10B3392__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "BaseEvent.h"
#include "EventMarker.h"

class CSeizureEvent : public CEventMarker  
{
public:
	CSeizureEvent();
	virtual ~CSeizureEvent();

//   BOOL SetPacket(const PACKET pPacket);
//   BOOL SetPacketPermanent(const PACKET pPacket);
   void PreparePacket();
   
   PWCHAR GetActive() const;
   void SetActive(PCWCH pwcActive);
   PWCHAR GetReference() const;
   void SetReference(PCWCH pwcRef);
   SHORT GetIsLast() const;
   void SetIsLast(SHORT sIsLast);
	void SetStart(BOOL bStart);
	BOOL IsStart();

private:
	BOOL m_bStartEvent;
};

#endif // !defined(AFX_SEIZUREEVENT_H__CCCE109A_7BCB_4079_AB41_8711C10B3392__INCLUDED_)
