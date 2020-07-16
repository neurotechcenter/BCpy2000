// Packet.h: interface for the CNPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKET_H__935AEC66_7A07_11D2_9931_00104B6F4F70__INCLUDED_)
#define AFX_PACKET_H__935AEC66_7A07_11D2_9931_00104B6F4F70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <PacketDef.h>

class CNPacket  
{
protected:
   explicit CNPacket(const GUID& guid = GUID_NULL);

public:
   virtual ~CNPacket() = 0;

public:
   const GUID& GetGuid() const;
   ULONGLONG GetSize() const;
   PACKET GetPacket() const;

   bool IsValid() const;
   bool IsAllocated() const;
   bool operator!() const;

   virtual void Clear();

   virtual bool CheckPacket(PACKET pPacket) const;
   virtual bool SetPacket(PACKET pPacket);
   virtual bool SetPacketPermanent(PACKET pPacket);

   virtual bool IsPacketEqual(PACKET pPacket) const;

   bool CheckPacketEx(PACKET pPacket, bool& bChanged) const;
   bool SetPacketPermanentEx(PACKET pPacket, bool& bChanged);

protected:
   void _Allocate(ULONGLONG llSize);
   void _Resize(ULONGLONG llSize);
   void _SetGuid(const GUID& guid);

   PACKETHEADER& _GetHeader() const;

   static const GUID& _GetGuid(PACKET pPacket);
   static ULONGLONG _GetSize(PACKET pPacket);
   
protected:
   PACKET m_pPacket;

private:
   ULONGLONG m_llAllocated;   // Actual allocated size in m_pPacket, 0 if packet is not owned
   GUID m_guid;

private:
   CNPacket(const CNPacket&);             // Not implemented
   CNPacket& operator=(const CNPacket&);  // Not implemented
};

/////////////////////////////////////////////////////////////////////////////
// Operators
/////////////////////////////////////////////////////////////////////////////
inline bool operator==(const CNPacket& cPacket1, const CNPacket& cPacket2)
{
   return cPacket1.IsPacketEqual(cPacket2.GetPacket());
}

inline bool operator==(const CNPacket& cPacket, PACKET pPacket)
{
   return cPacket.IsPacketEqual(pPacket);
}

inline bool operator==(PACKET pPacket, const CNPacket& cPacket)
{
   return cPacket.IsPacketEqual(pPacket);
}

inline bool operator!=(const CNPacket& cPacket1, const CNPacket& cPacket2)
{
   return !cPacket1.IsPacketEqual(cPacket2.GetPacket());
}

inline bool operator!=(const CNPacket& cPacket, PACKET pPacket)
{
   return !cPacket.IsPacketEqual(pPacket);
}

inline bool operator!=(PACKET pPacket, const CNPacket& cPacket)
{
   return !cPacket.IsPacketEqual(pPacket);
}

/////////////////////////////////////////////////////////////////////////////
// Constructor
/////////////////////////////////////////////////////////////////////////////
inline CNPacket::CNPacket(const GUID& guid)
   : m_pPacket(NULL), m_llAllocated(0), m_guid(guid)
{
}

/////////////////////////////////////////////////////////////////////////////
// Destructor
/////////////////////////////////////////////////////////////////////////////
inline CNPacket::~CNPacket()
{
   if (m_llAllocated > 0)
      delete[] m_pPacket;
}

/////////////////////////////////////////////////////////////////////////////
// IsValid
//
// Pre : None
// Post: Returns TRUE if the Packet contains data, otherwise FALSE.
/////////////////////////////////////////////////////////////////////////////
inline bool CNPacket::IsValid() const
{
   return m_pPacket != NULL;
}

/////////////////////////////////////////////////////////////////////////////
// _GetGuid
//
// Pre : pPacket is a valid PACKET object.
// Post: returns the GUID of pPacket.
/////////////////////////////////////////////////////////////////////////////
inline const GUID& CNPacket::_GetGuid(PACKET pPacket)
{
   _ASSERTE(pPacket != NULL);
   return *((GUID*)pPacket);
}

/////////////////////////////////////////////////////////////////////////////
// CheckPacket
// 
// Checks if packet matches without changing m_pPacket
/////////////////////////////////////////////////////////////////////////////
inline bool CNPacket::CheckPacket(PACKET pPacket) const
{
   if (pPacket == NULL)
      return false;

   const GUID& guid = _GetGuid(pPacket);
   if (0 != memcmp(&guid, &m_guid, sizeof(GUID)))
      return false;

   return true;
}

/////////////////////////////////////////////////////////////////////////////
// SetPacket
//
// Pre : NONE
// Post: If the object can read this packet the packet member is set 
//       to reference the packet and TRUE is returned, otherwise 
//       FALSE is returned.
/////////////////////////////////////////////////////////////////////////////
inline bool CNPacket::SetPacket(PACKET pPacket)
{
   if (!CheckPacket(pPacket))
      return false;

   if (m_llAllocated > 0)
   {
      delete[] m_pPacket;
      m_llAllocated = 0;
   }

   m_pPacket = pPacket;

   return true;
}

/////////////////////////////////////////////////////////////////////////////
// GetPacket
//
// Pre : None
// Post: If a packet has been assigned to the object a reference to 
//       the packet is returned, otherwise NULL is returned.
/////////////////////////////////////////////////////////////////////////////
inline PACKET CNPacket::GetPacket() const
{
   return m_pPacket;
}

/////////////////////////////////////////////////////////////////////////////
// GetGuid
//
// Pre : NONE
// Post: Returns the GUID the packet identifies  
/////////////////////////////////////////////////////////////////////////////
inline const GUID& CNPacket::GetGuid() const 
{
   return m_guid;
}

/////////////////////////////////////////////////////////////////////////////
// GetSize
//
// Pre : SetPacket or SetPacketPermanet has been called sucessfully
// Post: Returns the size of the packet
/////////////////////////////////////////////////////////////////////////////
inline ULONGLONG CNPacket::GetSize() const
{
   return m_pPacket ? _GetSize(m_pPacket) : 0;
}

/////////////////////////////////////////////////////////////////////////////
// _GetHeader
//
// Return:     the front of m_pPacket as a reference to a PACKETHEADER
//             structure
/////////////////////////////////////////////////////////////////////////////
inline PACKETHEADER& CNPacket::_GetHeader() const
{
   _ASSERTE(m_pPacket != NULL);
   return *(PACKETHEADER*)(m_pPacket);
}

/////////////////////////////////////////////////////////////////////////////
// _GetSize
//
// Pre : pPacket is a valid PACKET object.
// Post: returns the SIZE of pPacket.
/////////////////////////////////////////////////////////////////////////////
inline ULONGLONG CNPacket::_GetSize(PACKET pPacket)
{
   _ASSERTE(pPacket != NULL);
   return GETPACKETSIZE(pPacket);
}

#endif // !defined(AFX_PACKET_H__935AEC66_7A07_11D2_9931_00104B6F4F70__INCLUDED_)
