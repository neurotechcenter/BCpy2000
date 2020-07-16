// EventTypeInfo.h: interface for the CEventTypeInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTTYPEINFO_H__28C8BDAB_1854_4DC8_A0E6_7C4C68E25712__INCLUDED_)
#define AFX_EVENTTYPEINFO_H__28C8BDAB_1854_4DC8_A0E6_7C4C68E25712__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <InfoPacket.h>
#include <InfoStructures.h>

class CEventTypeInfo  : public CInfoPacket<NOINFO, EVENTTYPEINFO>
{
public:
   long FindEventType(const EVENTTYPEINFO *pEventType) const;
   long FindEventType(GUID guid) const;
   bool UnionB(const PACKET pPacket);
   bool UnionA(const PACKET pPacket);
};

#endif // !defined(AFX_EVENTTYPEINFO_H__28C8BDAB_1854_4DC8_A0E6_7C4C68E25712__INCLUDED_)
