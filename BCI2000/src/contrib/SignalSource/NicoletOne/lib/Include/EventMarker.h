// EventMarker.h: interface for the CEventMarker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTMARKER_H__AA293023_86CB_11D3_BF75_00105A9AFD56__INCLUDED_)
#define AFX_EVENTMARKER_H__AA293023_86CB_11D3_BF75_00105A9AFD56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <NPacket.h>
#include <AtlantisEvents.h>
#include <NrvTime.h>     

class CEventMarker : public CNPacket  
{
public:
	CEventMarker();
	virtual ~CEventMarker();

public:
   void PreparePacket(const GUID& guidEventType, LONG lTextSize);

   // Create a non-duration event
   GUID CreateEvent(NRVDATE ndt, const GUID& guidEventType, const WCHAR* wcText = NULL);

   // Create a duration event
   GUID OpenEvent(NRVDATE ndt, const GUID& guidEventType, const WCHAR* wcText = NULL);
   void CloseEvent(NRVDATE ndt);

   bool IsOpen() const;

   virtual void SetText(const WCHAR* wcText);
   const WCHAR* GetText() const;

   static const GUID& GetEventType(PACKET packet);
   const GUID& GetEventType() const;

   void SetLabel(const WCHAR* wcLabel);
   const WCHAR* GetLabel() const;

   void SetEventID(long dwID);
   long GetID() const;
   void SetTimeStamp(CNDateTime ndtStamp);
   CNDateTime GetTimeStamp() const;
   void SetDuration(double dDuration);
   double GetDuration() const;

   const GUID& GetUniqueGuid() const;
   void SetUniqueGuid(const GUID& guidUnique);
   const GUID& CreateUniqueGuid();

   const WCHAR* GetUser() const;
   double GetCreationDate() const;
   BOOL GetSystemDetected() const;

protected:
   EVENTPACKET* _GetEventPacket();
   const EVENTPACKET* _GetEventPacket() const;

   EVENTMARKER* _GetEventMarker();
   const EVENTMARKER* _GetEventMarker() const;

private:
   GUID _CreateEvent(NRVDATE ndt, double dDuration, const GUID& guidEventType, const WCHAR* wcText);
};

//////////////////////////////////////////////////////////////////////
// Inline functions
//////////////////////////////////////////////////////////////////////
inline CEventMarker::CEventMarker()
   : CNPacket(EVENTGUID)
{
}

inline CEventMarker::~CEventMarker()
{
}

inline EVENTPACKET* CEventMarker::_GetEventPacket()
{
   return (EVENTPACKET*)m_pPacket;
}

inline const EVENTPACKET* CEventMarker::_GetEventPacket() const
{
   return (const EVENTPACKET*)m_pPacket;
}

inline EVENTMARKER* CEventMarker::_GetEventMarker()
{
   return &_GetEventPacket()->eventMarker;
}

inline const EVENTMARKER* CEventMarker::_GetEventMarker() const
{
   return &_GetEventPacket()->eventMarker;
}

inline const GUID& CEventMarker::GetUniqueGuid() const
{
   return _GetEventMarker()->guidUnique;
}

inline CNDateTime CEventMarker::GetTimeStamp() const
{
   return _GetEventMarker()->ndtTimeStamp;
}

inline double CEventMarker::GetDuration() const
{
   return _GetEventMarker()->dDuration;
}

inline const GUID& CEventMarker::GetEventType(PACKET packet)
{
   const EVENTPACKET* pEventPacket = (const EVENTPACKET*)packet;
   _ASSERTE(pEventPacket->header.guid == EVENTGUID);

   return pEventPacket->eventMarker.guidEventType;
}

inline const GUID& CEventMarker::GetEventType() const
{
   return _GetEventMarker()->guidEventType;
}

inline const WCHAR* CEventMarker::GetLabel() const
{
   return _GetEventMarker()->wcLabel;
}

#endif // !defined(AFX_EVENTMARKER_H__AA293023_86CB_11D3_BF75_00105A9AFD56__INCLUDED_)
