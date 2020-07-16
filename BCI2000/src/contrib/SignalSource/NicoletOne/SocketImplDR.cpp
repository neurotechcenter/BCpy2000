
#include "stdafx.h"
#include "socketimpldr.h"
//#include <tgtimer.h>
//#include <logaction.h>
#include <nrverrormacros.h>
#include "LiveData.h"
#include "BCIStream.h"

CSocketImplDR::CSocketImplDR(void)
 : m_uiConnectedTimer(0),
   m_bInReceiveAndSendData(false),
   m_bDiscardBufferedData(false),
   //m_bEnableSendBack(true),
   m_bEstablished(false),
   m_bDumpToFile(false)
{
}

CSocketImplDR::~CSocketImplDR(void)
{
}


void CSocketImplDR::OnClose( int nErrorCode )
{
   m_bDiscardBufferedData = true;
   TRACE(_T("SocketDR::OnClose(%d)\n"), nErrorCode);

   if (nErrorCode != 0)
   {
      TRACE(_T("Connection closed because of some network error\n"));
      // The connection has been closed,
      // try to reconnect
      _Connect();
   }
   else
   {
      TRACE(_T("Connection closed from server\n"));
   }
   CAsyncSocket::OnClose(nErrorCode);
}

void CSocketImplDR::OnConnect( int nErrorCode )
{
   TRACE(_T("SocketDR::OnConnect(%d)\n"), nErrorCode);
   
   if (0 != nErrorCode)
   {
	  //bcierr << "Connection to NicoletOne failed." << std::endl;
	  //if( nErrorCode == WSAEADDRINUSE )
		//  bcierr << "Connection currently in use.  Try again in a bit." << std::endl;
   }
   else
   {   
      // m_vecBufferedData can't be cleared here, so signal that it should be
      // cleared at the next opportunity
      // Signal that OnReceive may call Receive
      m_bDiscardBufferedData = true;
      m_bEstablished = true;
      TRACE(_T("SocketDR: Has connection\n"));
   }
   
   CAsyncSocket::OnConnect(nErrorCode);
}

void CSocketImplDR::OnReceive( int nErrorCode )
{

    TRACE(_T("OnReceived Called, Error %d\n"), nErrorCode);
   if (nErrorCode == WSAENETDOWN)
   {
      
      ASSERT(FALSE);
      _Connect();
   }
   else
   {
      TRACE(_T("Can begin to receive\n"));
      
      _ReceiveAndSendDataGuarded();
   }
   
   CAsyncSocket::OnReceive(nErrorCode);
}



void CSocketImplDR::OnSend( int nErrorCode )
{
   TRACE(_T("Can begin to send\n"));
   //_SendData();
   CAsyncSocket::OnSend(nErrorCode);
}


void CSocketImplDR::CloseSocket()
{
   CAsyncSocket::Close();
   
	return;
}

void CSocketImplDR::OnAccept( int nErrorCode )
{
    CAsyncSocket::OnAccept(nErrorCode);
   
	return;
}
HRESULT CSocketImplDR::CreateSocket()
{
 
   long lFirst = 6050;
   long lLast = 6080; 

   bool bSucceeded = false;
   long lSocket = lFirst;
   while (lSocket <= lLast)
   {
      if (CAsyncSocket::Create(lSocket))
      {     
         bSucceeded = true;
         break;
      }

      if (GetLastError() != WSAEADDRINUSE)
         break;

      lSocket++;
   }
   if (!bSucceeded)
   {
      HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
      return hr;

      if (!CAsyncSocket::Create())
      {
         HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
         return hr;
      }
   }

   CAsyncSocket::GetSockName(m_strConnect, m_uiSocketPort);
   TRACE(_T("SocketDT: listening on address %s port %d\n"),
      m_strConnect, m_uiSocketPort);
   CString cstr = _T("listening on address: ") + m_strConnect;

   // Only listen to 5 connection
   CAsyncSocket::Listen(5); 


	return S_OK;
}

HRESULT CSocketImplDR::ConnectToSocket(BSTR bstrConnection, UINT uiSocketPort)
{
   m_strConnect = bstrConnection;
   m_uiSocketPort = uiSocketPort;
   if (_Connect())
      return S_OK;
   else
   {
      int err = GetLastError();
	  if( err == WSAEWOULDBLOCK )
		  return S_OK;
      return E_FAIL;
   }
}

bool CSocketImplDR::_Connect()
{

   CAsyncSocket::Close();
   long lFirst = 6050;
   long lLast = 6080; 

   bool bSucceeded = false;
   long lSocket = lFirst;
   while (lSocket <= lLast)
   {
      if (CAsyncSocket::Create(lSocket))
      {     
         bSucceeded = true;
         break;
      }
      int ii = GetLastError();
      if (GetLastError() != WSAEADDRINUSE)
         break;

      lSocket++;
   }

   if (!bSucceeded)
   {
      /*HRESULT hr = HRESULT_FROM_WIN32(GetLastError());*/
      return false;
   }
    if (m_cDumpFile.m_hFile != INVALID_HANDLE_VALUE)
      m_cDumpFile.Close();

   CString strDumpFilePath;
   strDumpFilePath.Format(_T("%s\\%ld.dump"), m_strDumpFolderPath, ::GetTickCount());

   if (m_bDumpToFile)
   {
      if (!m_cDumpFile.Open(strDumpFilePath,
         CFile::shareDenyNone | CFile::modeCreate | CFile::modeWrite))
         m_bDumpToFile = false;
   }

   return CAsyncSocket::Connect(m_strConnect, m_uiSocketPort) ? true : false;
}

void CSocketImplDR::SetDumpToFile(bool bDump, WCHAR* pstrFolderPath)
{
   m_bDumpToFile = bDump;
   _ASSERTE(wcslen(pstrFolderPath) < DUMP_FOLDER_PATH_SIZE);
   _ASSERTE(pstrFolderPath != NULL);
   wcsncpy(m_strDumpFolderPath, pstrFolderPath, DUMP_FOLDER_PATH_SIZE);
}

bool CSocketImplDR::IsConnected()
{
   //return m_hSocket != INVALID_SOCKET;
   return m_bEstablished;
}
void CSocketImplDR::DoTimer(UINT nID, DWORD /*dwTime*/)
{
   if (nID == m_uiConnectedTimer)
   {
      // this hack is neccessary to get the all the nework errors, for example if the
      // recording machine has been disconnected from the network.
      BYTE buff[100];
      memset(buff, 0, 100);
      int nSent = CAsyncSocket::Send(buff, 100);
      if (nSent == SOCKET_ERROR)
      {
         if (GetLastError() != WSAEWOULDBLOCK)
         {
            TRACE(_T("CSocketDR: Client send back failed\n"));
            //stopTimer(m_uiConnectedTimer);
            // Notify Listeners that the connection is not working
            /*for (UINT iNotify = 0; iNotify < m_vecListeners.size(); iNotify++)
            {
               HRESULT hr = m_vecListeners[iNotify]->NotifyConnectionClosed(L"");
               _ASSERTE(hr == S_OK);
            }*/
            CAsyncSocket::Close();
            m_bEstablished = false;
            //HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
         }
      }

      // It seems that sometimes we stop getting OnReceive even though the
      // sender is busy sending data. Calling Receive seems to fix this.
      _ReceiveAndSendDataGuarded();
   }
}

bool CSocketImplDR::_ReceiveData()
{
   _ASSERTE(m_bEstablished);
   
   BYTE buff[65536];
   int nRead = Receive(buff, sizeof(buff));
   switch (nRead)
   {
   case 0:
      TRACE(_T("SocketDR: Receive return 0, closing socket\n"));
      CAsyncSocket::Close();
      m_bEstablished = false;
      return false;
   case SOCKET_ERROR:
      {
         DWORD dwError = GetLastError();
         if (dwError != WSAEWOULDBLOCK)
         {
            _Connect();
         }
         CString cstr;
         cstr.Format(_T("_ReceiveData():: Receiving 0 bytes Socket_Error %d"), dwError);

      }
      return false;
   default:
      if (m_bDumpToFile)
      {
         m_cDumpFile.Write(buff, nRead);
      }
      m_vecBufferedData.insert(m_vecBufferedData.end(),
         buff, buff + nRead);
      return true;
   }
}

int CSocketImplDR::_SendData()
{
   _ASSERTE(m_bEstablished);
   ULONG lcBytesSent = 0;

   // If connection is closed while inside this function (from a ProcessData
   // handler), don't send any data remaining in the buffer.
   while (m_bEstablished && !m_bDiscardBufferedData)
   {
      _ASSERTE(lcBytesSent <= m_vecBufferedData.size());
      const ULONG lcRemaining = m_vecBufferedData.size() - lcBytesSent;

      // Check if buffer contains a whole packet.
      if (lcRemaining < sizeof(PACKETHEADER))
         break;
      PACKET const pPacket = &m_vecBufferedData[lcBytesSent];
      const ULONG lPacketSize = static_cast<ULONG>(GETPACKETSIZE(pPacket));
      if (lPacketSize > lcRemaining)
         break;

      _SendPacket(pPacket);

      // Check that buffer hasn't changed during _PassData
      _ASSERTE(lcRemaining == m_vecBufferedData.size() - lcBytesSent);
      _ASSERTE(pPacket == &m_vecBufferedData[lcBytesSent]);
      _ASSERTE(lPacketSize == static_cast<ULONG>(GETPACKETSIZE(pPacket)));

      lcBytesSent += lPacketSize;
   }

   // Bugfix
   m_vecBufferedData.erase(m_vecBufferedData.begin(), m_vecBufferedData.begin() + lcBytesSent);

   return lcBytesSent;
}

// Calls _ReceiveAndSendData, but prevents re-entry. This is necessary to
// ensure that packets are forwarded in the same order as they were received
// from the socket and to prevent modification to the buffer passed to
// _PassData.
void CSocketImplDR::_ReceiveAndSendDataGuarded()
{
   if (m_bInReceiveAndSendData)
      return;

   m_bInReceiveAndSendData = true;
   __try
   {
      if (m_bDiscardBufferedData)
      {
         TRACE(_T("SocketDR: Discarding %ld bytes in m_vecBufferedData\n"),
            m_vecBufferedData.size());
         m_vecBufferedData.clear();
         m_bDiscardBufferedData = false;
      }
      while (m_bEstablished)
      {
         if (!_ReceiveData())
            break;
         _SendData();
      }
   }
   __finally
   {
      m_bInReceiveAndSendData = false;
   }
}

void CSocketImplDR::_SendPacket(PACKET pPacket)
{
   CDataPacket cdata;
   if (cdata.SetPacket(pPacket))
   {
	   // Genius.
       //TRACE(_T("SocketDR::_SendPacket Sending dataPacket size (%d)\n"), GETPACKETSIZE(pPacket));
   }
   else
   {
	   // Only uncomment this if you're desperate
       //TRACE(_T("SocketDR::_SendPacket Sending other dataPacket size (%d)\n"), GETPACKETSIZE(pPacket));
   }

   m_cr->ReceivePacket(pPacket);
}

