//////////////////////////////////////////////////////////////////////
// EventArray.h: interface for the EventArray class.
// Purpose: Stores the events and allows efficient access
//
// Copyright 2001 by Taugagreining hf. - All rights reserved.
// THIS CODE IS CONFIDENTIAL
// Created: 23.10.2001 PER
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTARRAY_H__EEDD3DBC_36B6_4D73_9F57_C7B7BCE4A65C__INCLUDED_)
#define AFX_EVENTARRAY_H__EEDD3DBC_36B6_4D73_9F57_C7B7BCE4A65C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <RCEvent.h>
#include <NGuid.h>

//////////////////////////////////////////////////////////////////////
// CEventArray
//////////////////////////////////////////////////////////////////////
class CEventArray
{
public:
	CEventArray();
	virtual ~CEventArray();

public:
   HRESULT InsertEvent(PACKET packet); 
   HRESULT ModifyEvent(PACKET packet);
   HRESULT DeleteEvent(PACKET packet);
   void Reset();

   PACKET GetEvent(const GUID& guid);
   PACKET GetEvent(ULONG liEvent);
   LONG Seek(const CNDateTime& ndtTimeStamp) const;
   ULONG GetSize() const;
   bool IsEvent(const GUID& guid) const;

private:
   typedef std::vector<CRCEvent> VecEvents;
   typedef std::map<CNGuid, CRCEvent> MapGuidToEvent;

private:
   void _DeleteEvent(VecEvents::iterator iterEvent);
   VecEvents::iterator _FindEvent(const CRCEvent& event);

private:
   VecEvents m_vecEvents;  // Holds the events sorted by timestamp
   MapGuidToEvent m_mapGuidToEvent;

private:
   CEventArray(const CEventArray&);
   CEventArray& operator=(const CEventArray&);
};

//////////////////////////////////////////////////////////////////////
// CEventArray inline functions
//////////////////////////////////////////////////////////////////////
inline PACKET CEventArray::GetEvent(ULONG liEvent)
{
   _ASSERTE(liEvent < m_vecEvents.size());
   return m_vecEvents[liEvent].GetPacket();
}

inline bool CEventArray::IsEvent(const GUID& guid) const
{
   return m_mapGuidToEvent.count(guid) > 0;
}


#endif // !defined(AFX_EVENTARRAY_H__EEDD3DBC_36B6_4D73_9F57_C7B7BCE4A65C__INCLUDED_)
