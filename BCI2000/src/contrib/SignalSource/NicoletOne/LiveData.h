#pragma once

#include "CommonDecl.h"
#include "SocketImplDR.h"
#include "TSInfo.h"
#include "Sniffer.h"
#include "SignalInfo.h"
#include "ChannelInfo.h"
#include "RecordingHelper.h"
#include "SensorInfo.h"
#include "Lock.h"
#include "PrecisionTime.h"

#include <map>
#include <queue>


class CSocketImplDR;

class CLiveData : public IMyDataReceiver
{
public:
   CLiveData(void);
   ~CLiveData(void);

   bool ConnectToComputer(BSTR wcComputer, long lPort);
   void SetSocket(CSocketImplDR* pSocket){m_pcSocket = pSocket;};
   virtual void ReceivePacket(PACKET pPacket);

   void ClearBufferedData();
   bool GetData( int ChannelSize, int DataBlockSize, float** DataBlock, bool &newTsInfo );
   bool GetSampleRate( double *rate );
   bool GetNumChannels( int *channels );
   bool GetDCOffset( double*& offset );
   bool GetDCResolution( double*& resolution );

   void StartLogging();
   void StopLogging();

private:
   void _GrabData();

private:
   CLock *mDataLock;
   CSocketImplDR* m_pcSocket;
   CGeneralDataPacket m_cData;
   CRecordingHelper m_cRecordingHelper;
   CTSInfo m_cTSInfo;
   CSensorInfo m_cSensorInfo;
   CSignalInfo m_cSignalInfo;
   bool m_bNewTsInfo;
   bool m_bTsInfoReceived;
   bool m_bDataReceived;
   NRVDATE m_nrvdateFirst;
   bool m_bLogging;
   std::map< int, std::queue< float > > mCurData;
   PrecisionTime mStartTime;
   int mCount;
};

