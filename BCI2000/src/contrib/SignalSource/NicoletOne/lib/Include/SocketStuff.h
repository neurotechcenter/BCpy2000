// SocketStuff.h: interface for the CSocketStuff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETSTUFF_H__EE2AF97E_826B_487A_834E_188C34D43ECC__INCLUDED_)
#define AFX_SOCKETSTUFF_H__EE2AF97E_826B_487A_834E_188C34D43ECC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <streams.h>
#include <crtdbg.h>
#include <windows.h>
#pragma warning (disable: 4786)

#include "sharedthreadstuff.h"

#include <winsock.h>
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////

class CASocket
{
private:
	CASocket(const CASocket& rSrc);    // no implementation
	void operator=(const CASocket& rSrc);  // no implementation

// Construction
public:
	CASocket();
	BOOL Create(UINT nSocketPort = 0, int nSocketType=SOCK_STREAM,
		long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE,
		wchar_t* lpszSocketAddress = NULL);

// Attributes
public:
	SOCKET m_hSocket;

	operator SOCKET() const;
	BOOL Attach(SOCKET hSocket, long lEvent =
		FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE);
	SOCKET Detach();

	//BOOL GetPeerName(CString& rPeerAddress, UINT& rPeerPort);
   void GetSocketInfo(char* pc, long* plPort);
	BOOL GetPeerName(SOCKADDR* lpSockAddr, int* lpSockAddrLen);

	//BOOL GetSockName(CString& rSocketAddress, UINT& rSocketPort);
	BOOL GetSockName(SOCKADDR* lpSockAddr, int* lpSockAddrLen);

	BOOL SetSockOpt(int nOptionName, const void* lpOptionValue,
		int nOptionLen, int nLevel = SOL_SOCKET);
	BOOL GetSockOpt(int nOptionName, void* lpOptionValue,
		int* lpOptionLen, int nLevel = SOL_SOCKET);

	static CASocket* PASCAL FromHandle(SOCKET hSocket);
	static int PASCAL GetLastError();

// Operations
public:

	virtual BOOL Accept(CASocket& rConnectedSocket,
		SOCKADDR* lpSockAddr = NULL, int* lpSockAddrLen = NULL);

	BOOL Bind(UINT nSocketPort, wchar_t* lpszSocketAddress = NULL);
	BOOL Bind (const SOCKADDR* lpSockAddr, int nSockAddrLen);

	virtual bool Close();

	BOOL Connect(wchar_t* lpszHostAddress, UINT nHostPort);
	BOOL Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen);

	BOOL IOCtl(long lCommand, DWORD* lpArgument);

	BOOL Listen(int nConnectionBacklog=5);

	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);

	//int ReceiveFrom(void* lpBuf, int nBufLen,
	//	CString& rSocketAddress, UINT& rSocketPort, int nFlags = 0);
	int ReceiveFrom(void* lpBuf, int nBufLen,
		SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags = 0);

	enum { receives = 0, sends = 1, both = 2 };
	BOOL ShutDown(int nHow = sends);

	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);

	int SendTo(const void* lpBuf, int nBufLen,
		UINT nHostPort, wchar_t* lpszHostAddress = NULL, int nFlags = 0);
	int SendTo(const void* lpBuf, int nBufLen,
		const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags = 0);

	BOOL AsyncSelect(long lEvent =
		FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE);

// Overridable callbacks
protected:
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);

// Implementation
public:
	virtual ~CASocket();

	static CASocket* PASCAL LookupHandle(SOCKET hSocket, BOOL bDead = FALSE);
	static void PASCAL AttachHandle(SOCKET hSocket, CASocket* pSocket, BOOL bDead = FALSE);
	static void PASCAL DetachHandle(SOCKET hSocket, BOOL bDead = FALSE);
	static void PASCAL KillSocket(SOCKET hSocket, CASocket* pSocket);
	static void PASCAL DoCallBack(WPARAM wParam, LPARAM lParam);
   static bool Init();
   static void FreeMaps();

	BOOL Socket(int nSocketType=SOCK_STREAM, long lEvent =
		FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE,
		int nProtocolType = 0, int nAddressFormat = PF_INET);

private:
   shared_thread_stuff* m_shared;

protected:
	friend class CSocketwnd;

	virtual BOOL ConnectHelper(const SOCKADDR* lpSockAddr, int nSockAddrLen);
	virtual int ReceiveFromHelper(void* lpBuf, int nBufLen,
		SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags);
	virtual int SendToHelper(const void* lpBuf, int nBufLen,
		const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags);
};

inline BOOL CASocket::Bind(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{ 
   return (SOCKET_ERROR != bind(m_hSocket, lpSockAddr, nSockAddrLen)); 
}

inline BOOL CASocket::Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{ 
   return ConnectHelper(lpSockAddr, nSockAddrLen); 
}

inline int CASocket::SendTo(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags)
{ 
   return SendToHelper(lpBuf, nBufLen, lpSockAddr, nSockAddrLen, nFlags); 
}

inline BOOL CASocket::IOCtl(long lCommand, DWORD* lpArgument)
{ 
   return (SOCKET_ERROR != ioctlsocket(m_hSocket, lCommand, lpArgument)); 
}

inline BOOL CASocket::Listen(int nConnectionBacklog)
{ 
   return (SOCKET_ERROR != listen(m_hSocket, nConnectionBacklog)); 
}

inline CASocket::operator SOCKET() const	
{ 
   return m_hSocket; 
}
inline BOOL CASocket::GetPeerName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
{ 
   return (SOCKET_ERROR != getpeername(m_hSocket, lpSockAddr, lpSockAddrLen)); 
}
inline BOOL CASocket::GetSockName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
{ 
   return (SOCKET_ERROR != getsockname(m_hSocket, lpSockAddr, lpSockAddrLen)); 
}
inline BOOL CASocket::SetSockOpt(int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel)
{ 
   return (SOCKET_ERROR != setsockopt(m_hSocket, nLevel, nOptionName, (LPCSTR)lpOptionValue, nOptionLen)); 
}
inline BOOL CASocket::GetSockOpt(int nOptionName, void* lpOptionValue, int* lpOptionLen, int nLevel)
{ 
   return (SOCKET_ERROR != getsockopt(m_hSocket, nLevel, nOptionName, (LPSTR)lpOptionValue, lpOptionLen)); 
}
inline CASocket* PASCAL CASocket::FromHandle(SOCKET hSocket)
{ 
   return CASocket::LookupHandle(hSocket, FALSE); 
}
inline int PASCAL CASocket::GetLastError()
{ 
   return WSAGetLastError(); 
}
inline int CASocket::ReceiveFrom(void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags)
{ 
   return ReceiveFromHelper(lpBuf, nBufLen, lpSockAddr, lpSockAddrLen, nFlags); 
}
inline BOOL CASocket::ShutDown(int nHow)
{ 
   return (SOCKET_ERROR != shutdown(m_hSocket,nHow)); 
}

#endif // !defined(AFX_SOCKETSTUFF_H__EE2AF97E_826B_487A_834E_188C34D43ECC__INCLUDED_)
