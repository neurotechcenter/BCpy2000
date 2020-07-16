// TextInfoBase.h: interface for the CTextInfoBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTINFOBASE_H__A9721428_C0ED_40D4_9303_C6FB38DE3308__INCLUDED_)
#define AFX_TEXTINFOBASE_H__A9721428_C0ED_40D4_9303_C6FB38DE3308__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <NPacket.h>
#include <NrvDate.h>

class CTextInfoBase : public CNPacket  
{
public:
   struct TEXTINFOHEADER
   {
      PACKETHEADER ph;
      ULONGLONG llcDoubles;
      ULONGLONG llcStrings;
   };

   struct DOUBLEINFO
   {
      ULONGLONG llID;
      double dVal;
   };

   struct STRINGINFO
   {
      ULONGLONG llID;
      ULONGLONG llSize;
   };

public:
	CTextInfoBase();
	virtual ~CTextInfoBase();

public:
   virtual bool CheckPacket(PACKET pPacket) const;
   virtual bool SetPacket(PACKET pPacket);
   virtual bool SetPacketPermanent(PACKET pPacket);
   virtual void Clear();

   void PreparePacket();

protected:
   void _AddItem(ULONGLONG llID, double* pdtItem);
   void _AddItem(ULONGLONG llID, _bstr_t* pbstrItem);
   void _RemoveAllItems();

   virtual void _OnUnknownID(ULONGLONG llID);

private:
   typedef std::map<ULONGLONG, double*> dblMap;
   typedef std::map<ULONGLONG, _bstr_t*> stringMap;

private:
   ULONGLONG _CalcSize(ULONGLONG* pllcDoubles, ULONGLONG* pllcStrings) const;
   void _WriteToPacket();
   void _ReadPacket();
   void _ClearAll();

   DOUBLEINFO* _GetDoubleArray();
   STRINGINFO* _GetStringSizeArray();
   WCHAR* _GetStringStart();

private:
   dblMap m_mapDoubles;
   stringMap m_mapStrings;
};

template <typename Element>
class CArrayTextInfoBase : public CTextInfoBase
{
public:
   CArrayTextInfoBase();

public:
   ULONG GetElementCount() const;

   Element& operator[](ULONG liElement);
   const Element& operator[](ULONG liElement) const;

   ULONG AddElement(const Element& e = Element());

   virtual void Clear();

protected:
   virtual void _AddItems(ULONG liElement) = 0;
   virtual void _OnUnknownID(ULONGLONG llID);

private:
   std::deque<Element> m_elements;
};

template <typename Element>
CArrayTextInfoBase<Element>::CArrayTextInfoBase()
{
}

template <typename Element>
ULONG CArrayTextInfoBase<Element>::GetElementCount() const
{
   return m_elements.size();
}

template <typename Element>
Element& CArrayTextInfoBase<Element>::operator[](ULONG liElement)
{
   _ASSERTE(liElement < m_elements.size());
   return m_elements[liElement];
}

template <typename Element>
const Element& CArrayTextInfoBase<Element>::operator[](ULONG liElement) const
{
   _ASSERTE(liElement < m_elements.size());
   return m_elements[liElement];
}

template <typename Element>
ULONG CArrayTextInfoBase<Element>::AddElement(const Element& e)
{
   ULONG liElement = m_elements.size();
   m_elements.push_back(e);
   _AddItems(liElement);
   return liElement;
}

template <typename Element>
void CArrayTextInfoBase<Element>::_OnUnknownID(ULONGLONG llID)
{
   ULONG liElement = HIWORD(static_cast<LONG>(llID));

   while (liElement >= GetElementCount())
      AddElement();
}

template <typename Element>
void CArrayTextInfoBase<Element>::Clear()
{
   _RemoveAllItems();
   m_elements.clear();
   CTextInfoBase::Clear();
}

#endif // !defined(AFX_TEXTINFOBASE_H__A9721428_C0ED_40D4_9303_C6FB38DE3308__INCLUDED_)
