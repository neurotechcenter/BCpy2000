// VSyncInfo.h: interface for the CVSyncInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VSYNCINFO_H__98A38910_4C64_409C_88E8_CD2738A399C0__INCLUDED_)
#define AFX_VSYNCINFO_H__98A38910_4C64_409C_88E8_CD2738A399C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <InfoPacketDef.h>
#include <addinif.h>

class CVSyncInfo
{
public:
	CVSyncInfo();
	~CVSyncInfo();

public:
   void Clear();
   bool SetPacketPermanent(PACKET pPacket);

   ULONG GetElementCount() const { return m_cSyncEvents.GetElementCount(); }
   const SYNCINFO& GetElement(ULONG liElement) const { return m_cSyncEvents.GetElement(liElement); }

public:
	long Seek(const NRVDATE nrv) const;
	long NrvDate2SyncEvent(NRVDATE nrvDate);
	HRESULT NrvDate2CurPos(NRVDATE nrvDate, __int64* pllPos, long *plIndex, bool* pbNext, bool* pbInFile, IUnknown* punk, bool bFrame);
	HRESULT CurPos2NrvDate(__int64 llPos, NRVDATE *pnrvDate, long lIndex, IUnknown* punk, bool bFrame);
	HRESULT Date2SegmentDate(NRVDATE nrvDate, bool* pbNext, NRVDATE* pNrvDate, bool* pbInSegment, IUnknown* punk);
	HRESULT BuildEvents(IUnknown* punk);
   HRESULT BuildEvents2(INrvData3* pNrvData);
   long GetNextVideoFileDate(long lIndex, NRVDATE* pnrv, IUnknown* p);

	ULONG GetVideoFileCount() const;
   const WCHAR* GetVideoFileName(ULONG liFile) const;
	LONG GetNextVideoFile(NRVDATE nrvDate) const;
   bool VideoFileExists(ULONG liFile, const WCHAR* pwcPath) const;
   void GetVideoFileTime(ULONG liFile, NRVDATE* pndtStart, NRVDATE* pndtEnd) const;
   ULONG GetElementIndexFromFile(ULONG liFile) const;

private:
   struct VIDEOFILEINFO
   {
      WCHAR wcFile[DESCRIPTIONSIZE];
      ULONG liFirstEvent;
   };

private:
	void _BuildVideoFileList();
   void _CorrectEvents();
	LONG _IndexFromFileName(const WCHAR* pwcFile) const;

private:
   IPVIDEOSYNC m_cSyncEvents;
   std::vector<VIDEOFILEINFO> m_vecVideoFiles;

	bool m_bTrace;
   bool m_bCorrectionNeeded;
};

#endif // !defined(AFX_VSYNCINFO_H__98A38910_4C64_409C_88E8_CD2738A399C0__INCLUDED_)
