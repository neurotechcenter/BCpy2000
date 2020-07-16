// DataPacket.h: interface for the CDataPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAPACKET_H__54B73A4A_77D6_11D2_9931_00104B6F4F70__INCLUDED_)
#define AFX_DATAPACKET_H__54B73A4A_77D6_11D2_9931_00104B6F4F70__INCLUDED_

#include <NPacket.h>
#include <datastructures.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CGeneralDataPacket : public CNPacket
{
public:
   CGeneralDataPacket();
   virtual ~CGeneralDataPacket();

public:
   virtual bool SetPacket(PACKET pPacket);
   virtual void PreparePacket(LONG lcNrOfSeries, const LONG* plcSerieSamples);

   LONG GetSerieCount() const;
   LONG GetSampleCount(LONG liSerie) const;
   LONG GetMaxSampleCount() const;
   void SetTimeStamp(TIMESTAMP nrvTimeStamp);
   TIMESTAMP GetTimeStamp() const;
   SHORT* operator[] (LONG liSerie) const;
   GUID GetSubGuid() const;
   void SetSubGuid(const GUID& guid);

protected:
   bool _CheckPacket2(PACKET pPacket, const GUID& guid) const;
   static GUID _GetSubGuid(PACKET pPacket);

   void _FillSerieIndexArray();
   void _SetSubGuid(const GUID& guid);

private:
   virtual bool SetPacketPermanent(PACKET);

private:
   SHORT** m_ppSerieItemPtrs;
   ULONG m_lcAllocatedSerieItemPtrs;
};



class CDataPacket : public CGeneralDataPacket
{
public:
   CDataPacket();
   virtual ~CDataPacket();

public:
   virtual bool CheckPacket(PACKET pPacket) const;
   virtual void PreparePacket(LONG lcNrOfSeries, const LONG* plcSerieSamples);

};


class CDelayLinePacket : public CGeneralDataPacket
{
public:
   CDelayLinePacket();
   virtual ~CDelayLinePacket();

public:
   virtual bool CheckPacket(PACKET pPacket) const;
   virtual void PreparePacket(LONG lcNrOfSeries, const LONG* plcSerieSamples);

};

class CFilterPacket : public CGeneralDataPacket
{
public:
   CFilterPacket();
   virtual ~CFilterPacket();

public:
   virtual bool CheckPacket(PACKET pPacket) const;
   virtual void PreparePacket(LONG lcNrOfSeries, const LONG* plcSerieSamples);
};

inline bool CGeneralDataPacket::_CheckPacket2(PACKET pPacket, const GUID& guid) const
{
   if (!CNPacket::CheckPacket(pPacket))
      return false;

   if (_GetSubGuid(pPacket) != guid)
      return false;

   return true;
}

/////////////////////////////////////////////////////////////////////////////
//
// operator[]
//
// Pre : liSerie points to a serie in the packet and the packet exists
// Post: A ItemType pointer to the beginning of this serie is returned
//
/////////////////////////////////////////////////////////////////////////////
inline SHORT* CGeneralDataPacket::operator[] (LONG liSerie) const
{
   _ASSERTE(IsValid());
   _ASSERTE(liSerie < GetSerieCount());
   _ASSERTE(liSerie >= 0);
   return m_ppSerieItemPtrs[liSerie];
}

/////////////////////////////////////////////////////////////////////////////
//
// GetSampleCount
//
// Pre : 0 <= liSerie < GetSerieCount()
// Post: The returned value is the number of samples for serie liSerie
//      in the packet.
//
/////////////////////////////////////////////////////////////////////////////
inline LONG CGeneralDataPacket::GetSampleCount(LONG liSerie) const
{
   _ASSERTE(liSerie < GetSerieCount());
   _ASSERTE(liSerie >= 0);
   return *((LONG*)(m_pPacket + OFFSET_DP_SERIESSAMPLES + liSerie * SIZE_DP_SERIESSAMPLESUNIT));
}

inline GUID CGeneralDataPacket::GetSubGuid() const
{
   return _GetSubGuid(m_pPacket);
}

inline GUID CGeneralDataPacket::_GetSubGuid(PACKET pPacket)
{
   return *((GUID*)(pPacket + OFFSET_DP_SUBGUID));
}

inline void CGeneralDataPacket::SetSubGuid(const GUID& guid) 
{
   _SetSubGuid(guid);
}

inline void CGeneralDataPacket::_SetSubGuid(const GUID& guid)
{
   GUID *pSubGuid = (GUID*) (m_pPacket + OFFSET_DP_SUBGUID);
   *pSubGuid = guid;
}

inline bool CDataPacket::CheckPacket(PACKET pPacket) const
{
   return _CheckPacket2(pPacket, SHORTSAMPLESGUID);
}

inline bool CDelayLinePacket::CheckPacket(PACKET pPacket) const
{
   return _CheckPacket2(pPacket, DELAYLINESAMPLESGUID);
}

inline bool CFilterPacket::CheckPacket(PACKET pPacket) const
{
   return _CheckPacket2(pPacket, FILTERSAMPLESGUID);
}

#endif // !defined(AFX_DATAPACKET_H__54B73A4A_77D6_11D2_9931_00104B6F4F70__INCLUDED_)
