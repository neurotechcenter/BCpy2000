#pragma once
#include "afxsock.h"


#include "CommonDecl.h"
#include "LiveData.h"
#define DUMP_FOLDER_PATH_SIZE 256



class CSocketImplDR :
   public CAsyncSocket
{
public:
   CSocketImplDR(void);
   ~CSocketImplDR(void);

public:

   HRESULT GetSocketPort(UINT *puiSocketPort);
	HRESULT CreateSocket();
	void CloseSocket();
   HRESULT ConnectToSocket(BSTR bstrConnection, UINT uiSocketPort);
   bool IsConnected();
   void SetDumpToFile(bool bDump, WCHAR* strFolderPath);

//   void SetCallBackFunction(pDataStream pf){m_cf = pf;};
   void SetReceiver(IMyDataReceiver* cr){m_cr = cr;};

protected:

   virtual void OnAccept( int nErrorCode );
   virtual void OnClose( int nErrorCode );
   virtual void OnConnect( int nErrorCode );
   virtual void OnReceive( int nErrorCode );
   virtual void OnSend( int nErrorCode );

   virtual void DoTimer(UINT nID, DWORD dwTime);

private:
   bool _Connect();
   bool _ReceiveData();
   void _ReceiveAndSendDataGuarded();
   int _SendData();
   void _SendPacket(PACKET pPacket);
private:
   UINT m_uiConnectedTimer;
   UINT m_uiDisconnectedTimer;

   CString m_strConnect;
   UINT m_uiSocketPort;

   bool m_bEstablished;
   bool m_bDiscardBufferedData;
   bool m_bInReceiveAndSendData;

   std::vector<BYTE> m_vecBufferedData;

   bool m_bDumpToFile;
   CFile m_cDumpFile;
   WCHAR m_strDumpFolderPath[DUMP_FOLDER_PATH_SIZE];
   IMyDataReceiver* m_cr;
   //pDataStream m_cf;
   
};
