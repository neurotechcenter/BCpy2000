// RCEvent.h: interface for the CRCEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCEVENT_H__2FF771BC_7AA5_48E9_AAF9_683026BA0A7E__INCLUDED_)
#define AFX_RCEVENT_H__2FF771BC_7AA5_48E9_AAF9_683026BA0A7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <EventMarker.h>
#include <NrvTime.h>

//////////////////////////////////////////////////////////////////////
// CRCEvent
//
// A reference counted event class
//////////////////////////////////////////////////////////////////////
class CRCEvent;
bool operator<(const CRCEvent& e1, const CRCEvent& e2);
bool operator<(const CNDateTime& ndt, const CRCEvent& event);
bool operator==(const CRCEvent& e1, const CRCEvent& e2);

class CRCEvent
{
public:
   CRCEvent(const CRCEvent& event);
   explicit CRCEvent(PACKET packet);
   ~CRCEvent();
   CRCEvent& operator=(const CRCEvent& event);

public:
   PACKET GetPacket();
   
   const CNDateTime& GetStartTime() const;
   const CNDateTime& GetEndTime() const;
   const GUID& GetGuid() const;

   void CopyOverlappedEvents(const CRCEvent& eventPrev);
   void AddOverlappedEvent(const CRCEvent& event);
   void RemoveOverlappedEvent(const CRCEvent& event);
   const CRCEvent& GetFirstOverlappedEvent() const;

private:
   class CData;

private:
   CData* m_pData;
};

class CRCEvent::CData
{
public:
   typedef std::multiset<CRCEvent> SetEvents;

public:
   CData() : m_dwRef(0) { }

   void AddRef() { ++m_dwRef; }
   void Release() { if (--m_dwRef == 0) _Destroy(); }

public:
   CEventMarker m_cEvent; // The event itself
   SetEvents m_setOverlapped; // Events that this one overlaps

   // Start time and end time are buffered for speed and ease
   CNDateTime m_ndtStartTime;
   CNDateTime m_ndtEndTime;

private:
   void _Destroy();

private:
   DWORD m_dwRef;

private:
   CData(const CData&);
   CData& operator=(const CData&);
};

//////////////////////////////////////////////////////////////////////
// CRCEvent inline functions
//////////////////////////////////////////////////////////////////////
inline bool operator<(const CRCEvent& e1, const CRCEvent& e2)
{
   return e1.GetStartTime() < e2.GetStartTime();
}

inline bool operator<(const CNDateTime& ndt, const CRCEvent& event)
{
   return ndt < event.GetStartTime();
}

inline bool operator==(const CRCEvent& e1, const CRCEvent& e2)
{
   return InlineIsEqualGUID(e1.GetGuid(), e2.GetGuid()) != false;
}

inline CRCEvent::CRCEvent(const CRCEvent& event)
   : m_pData(event.m_pData)
{
   _ASSERTE(m_pData != NULL);
   m_pData->AddRef();
}

inline CRCEvent::~CRCEvent()
{
   _ASSERTE(m_pData != NULL);
   m_pData->Release();
}

inline const CNDateTime& CRCEvent::GetStartTime() const
{
   _ASSERTE(m_pData->m_ndtStartTime == m_pData->m_cEvent.GetTimeStamp());
   return m_pData->m_ndtStartTime;
}

inline const CNDateTime& CRCEvent::GetEndTime() const
{
   return m_pData->m_ndtEndTime;
}

inline PACKET CRCEvent::GetPacket()
{
   _ASSERTE(m_pData != NULL);
   return m_pData->m_cEvent.GetPacket();
}


#endif // !defined(AFX_RCEVENT_H__2FF771BC_7AA5_48E9_AAF9_683026BA0A7E__INCLUDED_)
