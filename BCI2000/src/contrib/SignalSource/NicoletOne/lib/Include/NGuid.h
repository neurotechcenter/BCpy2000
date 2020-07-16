//////////////////////////////////////////////////////////////////////
// NGuid.h: interface for the CNGuid class.
// Purpose: Wraps GUID with common operations.
//
// Copyright 2003 by Taugagreining hf. - All rights reserved.
// THIS CODE IS CONFIDENTIAL
// Created: 19.06.2003 PER
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NGUID_H__3DE8B4E8_B0AA_479D_8EA6_35A4519CABE3__INCLUDED_)
#define AFX_NGUID_H__3DE8B4E8_B0AA_479D_8EA6_35A4519CABE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _bstr_t;

struct CNGuid : GUID
{
	CNGuid();
   CNGuid(const GUID& guid);

   explicit CNGuid(LPOLESTR pszGuid);
   _bstr_t ToString() const;

   HRESULT CoCreateGuid();
};

bool operator<(const CNGuid& guid1, const CNGuid& guid2);

//////////////////////////////////////////////////////////////////////
// Constructors
//////////////////////////////////////////////////////////////////////
inline CNGuid::CNGuid()
   : GUID(GUID_NULL)
{

}

inline CNGuid::CNGuid(const GUID& guid)
   : GUID(guid)
{
}

//////////////////////////////////////////////////////////////////////
// operator<
//
// Allows CNGuid to be used as a key in std::set or std::map.
//////////////////////////////////////////////////////////////////////
inline bool operator<(const CNGuid& guid1, const CNGuid& guid2)
{
   return memcmp(&guid1, &guid2, sizeof(GUID)) < 0;
}

#endif // !defined(AFX_NGUID_H__3DE8B4E8_B0AA_479D_8EA6_35A4519CABE3__INCLUDED_)
