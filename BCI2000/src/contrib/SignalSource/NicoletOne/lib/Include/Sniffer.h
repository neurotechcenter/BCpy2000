#ifndef __SNIFFER_H_
#define __SNIFFER_H_

#include <PacketDef.h>

class CNPacket;

class CNSniffer
{
public:
   CNSniffer();
   virtual ~CNSniffer();

public:
   void Reset();
   
   bool CheckPacket(PACKET pPacket) const;
   bool CheckPacketEx(PACKET pPacket, bool& bChanged) const;
   long WhichCatcher(PACKET pPacket) const;

   bool SetPacketPermanent(PACKET pPacket);  // returns true if at least one catcher accepts packet
   bool SetPacketPermanentEx(PACKET pPacket, bool& bChanged);
   bool SetPacketAndReady(PACKET pPacket);

   virtual bool IsReady() const;             // returns true if all catchers have got packets

   void AddCatcher(CNPacket* pCatcher);
   CNPacket* GetCatcher(unsigned long liCatcher) const;
   unsigned long GetCatcherCount() const;
   long FindCatcher(const GUID& guid) const;

private:
   std::vector<CNPacket*> m_vecCatchers;
};

#endif
