// InfoPacket.h: interface for the CInfoPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOPACKET_H__36DB0C63_7946_11D2_9931_00104B6F4F70__INCLUDED_)
#define AFX_INFOPACKET_H__36DB0C63_7946_11D2_9931_00104B6F4F70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include <NPacket.h>
#include <InfoStructures.h>
#include <TgCopy.h>

#include <memory>    // for uninitialized_fill
#include <string.h>  // for memmove

enum 
{
	DEFAULT_ITEM_SUB_TYPE = 0L,
	DATEX_ITEM_SUB_TYPE = 1L,
};

struct INFOPACKETHEADER
{
   GUID guidItem; // Previously llItemID
   WCHAR szItemName[ITEMNAMESIZE];
	ULONG lItemSubType;
	DWORD dwReserved;
   BYTE RESERVEDBYTES[56];
};


template <class HeaderType_, class ElementType_>
struct InfoPacketHelper
{
   PACKETHEADER ph;
   INFOPACKETHEADER iph;
   HeaderType_ header;
   ULONG lcElems;
   ULONG lcAlloc;
   ElementType_ elems[1];
};


template <class HeaderType_, class ElementType_>
class CInfoPacket : public CNPacket
{
public:
   typedef HeaderType_ HeaderType;
   typedef ElementType_ ElementType;

   static const GUID& GetGuid();

public:
   CInfoPacket();
	CInfoPacket(long lItemSubType);
   virtual ~CInfoPacket() = 0;

public:
   virtual bool CheckPacket(PACKET pPacket) const;
   virtual bool SetPacketPermanent(PACKET pPacket);
   virtual bool IsPacketEqual(PACKET pPacket) const;

public:
	bool CheckPacketSubType(PACKET pPacket, ULONG lType) const;
   void PreparePacket(ULONG lcElements, const HeaderType& h = sm_defHeader, const ElementType& e = sm_defElement);

   ElementType& AddElement(const ElementType& e = sm_defElement);
   ElementType& InsertElement(ULONG liElement, const ElementType& e = sm_defElement);
   void RemoveElement(ULONG liElement);
   void Shrink();
	void SwapElements(ULONG liElementA, ULONG liElementB);

   const GUID& GetItemID() const
   {
      _ASSERTE(IsValid());
      return _GetHelper()->iph.guidItem;
   }
   
   void SetItemID(const GUID& guid)
   {
      _ASSERTE(IsAllocated());
      _GetHelper()->iph.guidItem = guid;
   }

   void SetItemID(BSTR bstrItemID)
   {
      GUID guid;
      if (!bstrItemID || wcslen(bstrItemID) == 0)
      {
         // Map NULL/empty string to GUID_NULL (CLSIDFromString fails on these)
         guid = GUID_NULL;
      }
      else
      {
         HRESULT hr = CLSIDFromString(bstrItemID, &guid);
         _ASSERT(SUCCEEDED(hr));
      }

      SetItemID(guid);
   }

	void SetItemSubType(const long& lType)
	{
		_ASSERTE(IsAllocated());
		_GetHelper()->iph.lItemSubType = lType;
	}
	const ULONG& GetItemSubType() const
	{
		_ASSERTE(IsValid());
		return _GetHelper()->iph.lItemSubType;
	}

   const WCHAR* GetItemName() const;
   void SetItemName(const WCHAR* pszName);

   HeaderType& Header()
   {
      _ASSERTE(IsAllocated());
      return _GetHelper()->header;
   }

   const HeaderType& GetHeader() const
   {
      _ASSERTE(IsValid());
      return _GetHelper()->header;
   }

   void SetHeader(const HeaderType& h)
   {
      _ASSERTE(IsAllocated());
      _GetHelper()->header = h;
   }

   ULONG GetElementCount() const;

   ElementType& Element(ULONG liElement);
   void SetElement(ULONG liElement, const ElementType& e);
   const ElementType& GetElement(ULONG liElement) const;
   const ElementType& operator[](ULONG liElement) const   { return GetElement(liElement); }

   static ULONGLONG CalcPacketSize(ULONG lcElements);

// The values used as default parameters
// These have to be defined for each class that inherits from
// CInfoPacket
// See DEFINE_IP_DEFAULTS below
public:
   static const HeaderType sm_defHeader;
   static const ElementType sm_defElement;
   static const GUID sm_guid;
	static const long sm_lsubtype;

private:
   typedef InfoPacketHelper<HeaderType, ElementType> helper;

   helper* _GetHelper()
   {
      _ASSERTE(IsAllocated());
      return (helper*)m_pPacket;
   }

   const helper* _GetHelper() const
   {
      _ASSERTE(IsValid());
      return (const helper*)m_pPacket;
   }

   void _Grow();
};


/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
template <class HeaderType_, class ElementType_>
CInfoPacket<HeaderType_, ElementType_>::CInfoPacket()
{
   _ASSERTE(sizeof(HeaderType) % 8 == 0);
   _ASSERTE(sizeof(ElementType) % 8 == 0);
   _ASSERTE(sizeof(helper) % 8 == 0);
   _ASSERTE(sizeof(INFOPACKETHEADER) == 208);

   _SetGuid(sm_guid);
}
template <class HeaderType_, class ElementType_>
CInfoPacket<HeaderType_, ElementType_>::CInfoPacket(long lItemSubType)
{
   _ASSERTE(sizeof(HeaderType) % 8 == 0);
   _ASSERTE(sizeof(ElementType) % 8 == 0);
   _ASSERTE(sizeof(helper) % 8 == 0);
   _ASSERTE(sizeof(INFOPACKETHEADER) == 208);

   _SetGuid(sm_guid);
	_SetItemSubType(lItemSubType);
}

template <class HeaderType_, class ElementType_>
const GUID& CInfoPacket<HeaderType_, ElementType_>::GetGuid()
{
   return sm_guid;
}

template <class HeaderType_, class ElementType_>
CInfoPacket<HeaderType_, ElementType_>::~CInfoPacket()
{
}

/////////////////////////////////////////////////////////////////////////////
//
// PreparePacket
//
// Pre : parameters are valid.
// Post: A packet has been created that can hold lcSeries number of 
//       series info.
//
/////////////////////////////////////////////////////////////////////////////
template <class HeaderType_, class ElementType_>
void CInfoPacket<HeaderType_, ElementType_>::PreparePacket(ULONG lcElements,
   const HeaderType& h, const ElementType& e)
{
   _ASSERTE(lcElements >= 0);

   LONGLONG llSize = CalcPacketSize(lcElements);
   _Allocate(llSize);

   memset(&_GetHelper()->iph, 0, sizeof(INFOPACKETHEADER));
   _GetHelper()->header = h;

   _GetHelper()->lcElems = lcElements;
   _GetHelper()->lcAlloc = lcElements;

   ElementType* pElem = _GetHelper()->elems;
   std::uninitialized_fill(pElem, pElem + lcElements, e);
}


template <class HeaderType_, class ElementType_>
inline ULONG CInfoPacket<HeaderType_, ElementType_>::GetElementCount() const
{
   if (IsValid())
   {
      _ASSERTE(_GetHelper()->lcAlloc >= _GetHelper()->lcElems);
      _ASSERTE(CalcPacketSize(_GetHelper()->lcAlloc) <= GetSize());
      return _GetHelper()->lcElems;
   }

   return 0;
}


template <class HeaderType_, class ElementType_>
ElementType_& CInfoPacket<HeaderType_, ElementType_>::AddElement(const ElementType& e)
{
   return InsertElement(GetElementCount(), e);
}


template <class HeaderType_, class ElementType_>
ElementType_& CInfoPacket<HeaderType_, ElementType_>::InsertElement(ULONG liElement, const ElementType& e)
{
   _ASSERTE(IsAllocated());
   _ASSERTE(GetElementCount() >= liElement);

   _ASSERTE(_GetHelper()->lcElems <= _GetHelper()->lcAlloc);
   if (_GetHelper()->lcElems == _GetHelper()->lcAlloc)
      _Grow();

   if (liElement < _GetHelper()->lcElems)
      memmove(&Element(liElement + 1), &Element(liElement), sizeof(ElementType) * (GetElementCount() - liElement));

   ++_GetHelper()->lcElems;

   _ASSERTE(_GetHelper()->lcAlloc >= _GetHelper()->lcElems);
   _ASSERTE(CalcPacketSize(_GetHelper()->lcAlloc) == GetSize());
   return Element(liElement) = e;
}

template <class HeaderType_, class ElementType_>
void CInfoPacket<HeaderType_, ElementType_>::RemoveElement(ULONG liElement)
{
   _ASSERTE(IsAllocated());
   _ASSERTE(GetElementCount() > liElement);

   if (liElement < GetElementCount() - 1)
      memmove(&Element(liElement), &Element(liElement + 1), sizeof(ElementType) * (GetElementCount() - liElement - 1));

   --_GetHelper()->lcElems;
}


template <class HeaderType_, class ElementType_>
void CInfoPacket<HeaderType_, ElementType_>::SwapElements(ULONG liElementA, ULONG liElementB)
{
   _ASSERTE(IsAllocated());

	ULONG liBigger = max(liElementA, liElementB);
	ULONG liSmaller = min(liElementA, liElementB);

   _ASSERTE(GetElementCount() > liBigger);

   ElementType_ ebig = GetElement(liBigger);
	ElementType_ esmall = GetElement(liSmaller);
	RemoveElement(liBigger);
	RemoveElement(liSmaller);
	InsertElement(liSmaller, ebig);
	InsertElement(liBigger, esmall);
}


template <class HeaderType_, class ElementType_>
inline ElementType_& CInfoPacket<HeaderType_, ElementType_>::Element(ULONG liElement)
{
   _ASSERTE(IsAllocated());
   _ASSERTE(liElement < GetElementCount());
   _ASSERTE(_GetHelper()->lcAlloc >= _GetHelper()->lcElems);
   _ASSERTE(CalcPacketSize(_GetHelper()->lcAlloc) <= GetSize());
   return _GetHelper()->elems[liElement];
}

template <class HeaderType_, class ElementType_>
inline void CInfoPacket<HeaderType_, ElementType_>::SetElement(ULONG liElement, const ElementType& e)
{
   _ASSERTE(IsAllocated());
   _ASSERTE(liElement < GetElementCount());
   _ASSERTE(_GetHelper()->lcAlloc >= _GetHelper()->lcElems);
   _ASSERTE(CalcPacketSize(_GetHelper()->lcAlloc) <= GetSize());
   _GetHelper()->elems[liElement] = e;
}


template <class HeaderType_, class ElementType_>
inline const ElementType_& CInfoPacket<HeaderType_, ElementType_>::GetElement(ULONG liElement) const
{
   _ASSERTE(IsValid());
   _ASSERTE(liElement < GetElementCount());
   _ASSERTE(_GetHelper()->lcAlloc >= _GetHelper()->lcElems);
   _ASSERTE(CalcPacketSize(_GetHelper()->lcAlloc) <= GetSize());

   return _GetHelper()->elems[liElement];
}
   
/////////////////////////////////////////////////////////////////////////////
// IsPacketEqual
//
// Pre : NONE
// Post: Returnes TRUE if the memory of pPacket is identical to the memory of
//       m_pPacket. 
//       Overridden because InfoPackets may contain extra allocated space
//       that should not be compared.
/////////////////////////////////////////////////////////////////////////////
template <class HeaderType_, class ElementType_>
bool CInfoPacket<HeaderType_, ElementType_>::IsPacketEqual(PACKET pPacket) const
{
   if (m_pPacket == NULL && pPacket == NULL)
      return true;
   else if (m_pPacket == NULL || pPacket == NULL)
      return false;

   // Compare fields 1 at a time (can't do a simple memcmp for
   // the whole packet because PacketSize and lcAlloc may be
   // different
   if (_GetGuid(pPacket) != GetGuid())
      return false;

   if (GETPACKETSIZE(pPacket) < CalcPacketSize(0)) // pPacket is dubious
   {
      _ASSERT(false);
      return false;
   }

   const helper* pHelper = (const helper*)pPacket;

   if (memcmp(&pHelper->iph, &_GetHelper()->iph, sizeof(INFOPACKETHEADER)) != 0)
      return false;

   if (memcmp(&pHelper->header, &GetHeader(), sizeof(HeaderType)) != 0)
      return false;

   if (pHelper->lcElems != GetElementCount())
      return FALSE;

   if (GetElementCount() > 0)
   {
      return memcmp(&pHelper->elems[0], &GetElement(0), sizeof(ElementType) * GetElementCount()) == 0;
   }
   else
   {
      return true;
   }
}

/////////////////////////////////////////////////////////////////////////////
// CheckPacket
/////////////////////////////////////////////////////////////////////////////
template <class HeaderType_, class ElementType_>
bool CInfoPacket<HeaderType_, ElementType_>::CheckPacket(PACKET pPacket) const
{
   if (pPacket == NULL)
      return false;

   if (::IsBadReadPtr(pPacket, sizeof(PACKETHEADER)))
   {
      _ASSERT(false);
      return false;
   }

   if (_GetGuid(pPacket) != GetGuid())
      return false;

   // Check for bogus packets
   ULONGLONG llSize = GETPACKETSIZE(pPacket);
   if (llSize < CalcPacketSize(0) ||
       ::IsBadReadPtr(pPacket, UINT_PTR(llSize)))
   {
      _ASSERT(false);
      return false;
   }

   const helper* pHelper = (const helper*)pPacket;
   if (CalcPacketSize(pHelper->lcElems) > llSize)
   {
      _ASSERT(false);
      return false;
   }

   return true;
}

/////////////////////////////////////////////////////////////////////////////
// SetPacketPermanent
//
// Pre : NONE
// Post: If the object can read this packet pPacket is copyed to a member
//       of the class and TRUE is returned, otherwise FALSE is returned.
//       Overridden because InfoPackets may contain extra allocated space
//       that should not be copied.
/////////////////////////////////////////////////////////////////////////////
template <class HeaderType_, class ElementType_>
bool CInfoPacket<HeaderType_, ElementType_>::SetPacketPermanent(PACKET pPacket)
{
   if (!CheckPacket(pPacket))
      return false;

   // Neccessary sanity check
   if (pPacket == m_pPacket && IsAllocated())
      return true;

   const helper* pHelper = (const helper*)pPacket;

   // Only allocate as much space as is needed to hold the actual contents
   // of the packet
   ULONGLONG llSize = CalcPacketSize(pHelper->lcElems);
   _Allocate(llSize);

   // Take care not to overwrite the size, it may be different
   memcpy(m_pPacket + sizeof(PACKETHEADER), pPacket + sizeof(PACKETHEADER), (size_t)llSize - sizeof(PACKETHEADER));
   // Adjust lcAlloc in case we are allocating less than before
   _GetHelper()->lcAlloc = _GetHelper()->lcElems;

   _ASSERTE(_GetHelper()->lcElems == _GetHelper()->lcAlloc);
   _ASSERTE(CalcPacketSize(_GetHelper()->lcAlloc) == GetSize());
   _ASSERTE(IsAllocated());

   return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// _Grow
//
// Allocates extra space
/////////////////////////////////////////////////////////////////////////////
template <class HeaderType_, class ElementType_>
void CInfoPacket<HeaderType_, ElementType_>::_Grow()
{
   _ASSERTE(CalcPacketSize(_GetHelper()->lcAlloc) == GetSize());
   const ULONG lNewSize = _GetHelper()->lcAlloc * 2 + 1;
   const ULONGLONG llSize = CalcPacketSize(lNewSize);

   _Resize(llSize);
   _ASSERTE(llSize == GetSize());
   _GetHelper()->lcAlloc = lNewSize;
   _ASSERTE(CalcPacketSize(_GetHelper()->lcAlloc) == GetSize());
}


/////////////////////////////////////////////////////////////////////////////
// Shrink
//
// Disposes extra allocated space
/////////////////////////////////////////////////////////////////////////////
template <class HeaderType_, class ElementType_>
void CInfoPacket<HeaderType_, ElementType_>::Shrink()
{
   if (!IsAllocated())
      return;

   _ASSERTE(CalcPacketSize(_GetHelper()->lcAlloc) == GetSize());
   const ULONG lNewSize = _GetHelper()->lcElems;
   const ULONGLONG llSize = CalcPacketSize(lNewSize);

   _Resize(llSize);
   _ASSERTE(llSize == GetSize());
   _GetHelper()->lcAlloc = lNewSize;

   _ASSERTE(CalcPacketSize(_GetHelper()->lcAlloc) == GetSize());
   _ASSERTE(_GetHelper()->lcAlloc == _GetHelper()->lcElems);
}

template <class HeaderType_, class ElementType_>
const WCHAR* CInfoPacket<HeaderType_, ElementType_>::GetItemName() const
{
   _ASSERTE(IsValid());
   _ASSERTE(wcslen(_GetHelper()->iph.szItemName) < ITEMNAMESIZE);
   return _GetHelper()->iph.szItemName;
}

template <class HeaderType_, class ElementType_>
void CInfoPacket<HeaderType_, ElementType_>::SetItemName(const WCHAR* pszName)
{
   _ASSERTE(IsAllocated());
   _ASSERTE(pszName != NULL);
   _ASSERTE(wcslen(pszName) < ITEMNAMESIZE);

   wszcpy(_GetHelper()->iph.szItemName, pszName, ITEMNAMESIZE);
}

template <class HeaderType_, class ElementType_>
inline ULONGLONG CInfoPacket<HeaderType_, ElementType_>::CalcPacketSize(ULONG lcElements)
{
   // lcElements - 1 because helper includes 1 ElementType
   return sizeof(helper) + (lcElements - 1) * sizeof(ElementType);
}

template <class HeaderType_, class ElementType_>
bool CInfoPacket<HeaderType_, ElementType_>::CheckPacketSubType(PACKET pPacket, ULONG lType) const
{
	INFOPACKETHEADER* pcInfo = (INFOPACKETHEADER*)(pPacket + sizeof(PACKETHEADER)); //(InfoPacketHelper*) does not work because it is of unknown size
	return (pcInfo->lItemSubType == lType);
}

/////////////////////////////////////////////////////////////////////////////
// CSimpleInfoPacket: Info packet without elements

template <class HeaderType_>
class CSimpleInfoPacket : public CInfoPacket<HeaderType_, NOINFO>
{
public:
   virtual ~CSimpleInfoPacket() = 0;

public:
   void PreparePacket(const HeaderType& h = sm_defHeader);
   const HeaderType* operator->() const;
   static ULONGLONG CalcPacketSize();

private:
   // Hide members that deal with elements
   const ElementType& operator[](ULONG liElement) const;              // not implemented
   ULONG GetElementCount() const;                                     // not implemented
   ElementType& Element(ULONG liElement);                             // not implemented
   const ElementType& GetElement(ULONG liElement) const;              // not implemented
   void SetElement(ULONG liElement, const ElementType& e);            // not implemented
   ElementType& AddElement(const ElementType& e);                    // not implemented
   ElementType& InsertElement(ULONG liElement, const ElementType& e); // not implemented
   void RemoveElement(ULONG liElement);                               // not implemented
};

template <class HeaderType_>
CSimpleInfoPacket<HeaderType_>::~CSimpleInfoPacket()
{
}

template <class HeaderType_>
inline void CSimpleInfoPacket<HeaderType_>::PreparePacket(const HeaderType& h)
{
   CInfoPacket<HeaderType_, NOINFO>::PreparePacket(0, h);
}

template <class HeaderType_>
inline const HeaderType_* CSimpleInfoPacket<HeaderType_>::operator->() const
{
   return &CInfoPacket<HeaderType_, NOINFO>::GetHeader();
}

template <class HeaderType_>
inline ULONGLONG CSimpleInfoPacket<HeaderType_>::CalcPacketSize()
{
   return CInfoPacket<HeaderType_, NOINFO>::CalcPacketSize(0);
}

#define DEFINE_IP_DEFAULTS(type, guid)                                                 \
const GUID CInfoPacket<type::HeaderType, type::ElementType>::sm_guid = guid;           \
const type::HeaderType CInfoPacket<type::HeaderType, type::ElementType>::sm_defHeader; \
const type::ElementType CInfoPacket<type::HeaderType, type::ElementType>::sm_defElement; \
const long CInfoPacket<type::HeaderType, type::ElementType>::sm_lsubtype = DEFAULT_ITEM_SUB_TYPE;


#endif // !defined(AFX_INFOPACKET_H__36DB0C63_7946_11D2_9931_00104B6F4F70__INCLUDED_)

