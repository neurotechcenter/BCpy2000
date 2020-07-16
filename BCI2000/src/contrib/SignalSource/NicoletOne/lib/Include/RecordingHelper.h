#if !defined(_RECORDINGHELPER_H__INCLUDED_)
#define _RECORDINGHELPER_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <NotifyPacketDef.h>
#include <NPacket.h>

class CRecordingHelper
{
public:
   enum Mode { Off, Segment, Monitoring, ImpTest, HWInf };

public:
   CRecordingHelper();
   virtual ~CRecordingHelper();

public:
   void Reset();
   bool CheckPacket(PACKET pPacket) const;
   bool SetPacket(PACKET pPacket);
   bool NotifySubscriber(LPOLESTR pszGuid);

   bool IsSegment() const;
   bool IsMonitoring() const;
   bool IsRunning() const; // Either monitoring or recording
   bool IsImpTest() const;
   bool IsOff() const;  // not recording, monitoring or imptest
   bool HasBeenRecording() const;
   Mode GetMode() const;

private:
   STOP m_cStop;
   STARTSEGMENT m_cStartSegment;
   STARTMONITOR m_cStartMonitor;
   STARTIMPTEST m_cStartImpTest;
   bool m_bHasBeenRecording;
   Mode m_mode;
};

#endif 
