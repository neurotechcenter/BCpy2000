#pragma once

#include <PacketDef.h>
#include "Datapacket.h"

interface IMyDataReceiver
{
public:
   virtual void ReceivePacket(PACKET pPacket) = 0;
};
