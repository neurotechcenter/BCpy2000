// NotifyPacket.h: interface for the CNotifyPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOTIFYPACKET_H__11D6CDEC_4999_4502_800C_32469FC4E3C0__INCLUDED_)
#define AFX_NOTIFYPACKET_H__11D6CDEC_4999_4502_800C_32469FC4E3C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <NPacket.h>

// {1F47D937-2187-439a-87D1-634F887FCC04}
static const GUID NOTIFYGUID = 
{ 0x1f47d937, 0x2187, 0x439a, { 0x87, 0xd1, 0x63, 0x4f, 0x88, 0x7f, 0xcc, 0x4 } };

typedef struct tagNOTIFY
{
   GUID     guidCommon;
   LONGLONG llSize;
   GUID guid;
   VARIANT varArgument;
} NOTIFY;

class CNotifyPacket : public CNPacket
{
public:
	CNotifyPacket();
	virtual ~CNotifyPacket();

   virtual void PreparePacket();

   void SetNotification(const GUID& guidNotify);

   const GUID& GetNotification() const;

   // Pre : PreparePacker has been called.
   // Post: The Argument is set to varArgument.
   HRESULT SetArgument(const VARIANT& varArgument);

   // Pre : PreparePacker has been called.
   // Post: The hint is returned.
   const VARIANT& GetArgument() const;
};

class CNotify : public CNotifyPacket
{
public:
   explicit CNotify(const GUID& guid);
   virtual ~CNotify();

   virtual void PreparePacket();
   virtual bool CheckPacket(PACKET pPacket) const;

private:
   GUID m_guidNotify;
};



#endif // !defined(AFX_NOTIFYPACKET_H__11D6CDEC_4999_4502_800C_32469FC4E3C0__INCLUDED_)
