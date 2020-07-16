#ifndef __VHTSOCKET_H
#define __VHTSOCKET_H
/********************************************************************
 FILE: $Id: vhtSocket.h,v 1.5 2002/08/28 23:16:24 jsilver Exp $
 AUTHOR: Ron Carmel
 DATE: Aug 1998
<header>

 DESCRIPTION:
  -
 HISTORY:
  -
 NOTES:
  - currently only supports non-blocking streaming INET sockets
</header>
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

/* MOD-990212 [HD]: The fstream is already imported in the next segment.
* TODO: Find if conditional import of fstream is enough.
#ifdef _DEBUG
#   include <fstream.h>
#endif
*/


#include <assert.h>
#if defined(_WIN32)
#include <porting/msvIOstream.h>
#else
#include <iostream.h>
#include <fstream.h>
#endif

#ifdef _WIN32
#	define _WIN32_WINNT 0x0400
#   include <winsock2.h>
#	include <mswsock.h>
typedef int socklen_t;
#elif defined (_IRIX) || defined(linux)
#   include <stdio.h>
#   include <sys/time.h>
#   include <sys/utsname.h>
#   include <sys/param.h>
#   include <sys/types.h>
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <netdb.h>
#   include <unistd.h>
#   include <stdlib.h>
#   include <errno.h>
#else 
#   error If you are compiling on a Windows platform (95, 98 or NT4.X or newer) #define _WIN32.  If you are compiling on an Irix platform #define _IRIX (versions 6.3 through 6.5 are currently supported), If you are compiling on a QNX platform #define _QNX.  Other operating systems are currently not supported.  For more information, contact Virtual Technologies.
#endif

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#ifndef MAXHOSTNAMELEN 
#define MAXHOSTNAMELEN 256
#endif

#define VHTSOCKETPORT_NULL -1

class vhtSocket {

	// TODO: REMOVE VHTLOG!!!!
	public:
#if !defined(_WIN32)
		static fstream      vhtlog;
#endif

	public:
		VH_CORE_DLL_EXPORT vhtSocket(int af=AF_INET, int type=SOCK_STREAM, int protocol=0);
		VH_CORE_DLL_EXPORT vhtSocket(vhtSocket &s);
		VH_CORE_DLL_EXPORT ~vhtSocket();

		VH_CORE_DLL_EXPORT inline bool bind(int port = VHTSOCKETPORT_NULL) {
			
			bool success = false;
#ifdef _WIN32
            HOSTENT *host = NULL;
#else
			hostent *host = NULL;
#endif
            char hostName[MAXHOSTNAMELEN];
            if (gethostname(hostName, MAXHOSTNAMELEN)==0){

#if defined(_DEBUG) && !defined(_WIN32)
			vhtlog<<"\nDEBUG: gethostname returned \""<<hostName<<"\"\n";
            vhtlog.flush();
#endif

			host = gethostbyname(hostName);

			if (host != NULL){
				// set address:
				m_sockAddr.sin_addr.s_addr = INADDR_ANY;

				// set port:
				if (port == VHTSOCKETPORT_NULL){
					m_sockAddr.sin_port = 0;
				}
				else {
					m_sockAddr.sin_port = htons(port);
				}

				// set family:
				m_sockAddr.sin_family = m_family;

				// bind:
				success = (::bind(m_socket, (sockaddr*)&m_sockAddr, sizeof(sockaddr_in))==0);

#if defined( _DEBUG) && !defined(_WIN32)
				vhtlog<<"\nDEBUG: gethostbyname returned IP = ";
#               ifdef _WIN32
                vhtlog << (int)m_sockAddr.sin_addr.S_un.S_un_b.s_b1 << ".";
                vhtlog << (int)m_sockAddr.sin_addr.S_un.S_un_b.s_b2 << ".";
                vhtlog << (int)m_sockAddr.sin_addr.S_un.S_un_b.s_b3 << ".";
                vhtlog << (int)m_sockAddr.sin_addr.S_un.S_un_b.s_b4 << "\n";
                vhtlog.flush();
#               elif defined (_QNX)
                vhtlog << (int)((m_sockAddr.sin_addr.s_addr    ) & 0x000000ff) << ".";
                vhtlog << (int)((m_sockAddr.sin_addr.s_addr>>8 ) & 0x000000ff) << ".";
                vhtlog << (int)((m_sockAddr.sin_addr.s_addr>>16) & 0x000000ff) << ".";
                vhtlog << (int)((m_sockAddr.sin_addr.s_addr>>24) & 0x000000ff) << "\n";
#                                   else
                vhtlog << (int)((m_sockAddr.sin_addr.s_addr>>24) & 0x000000ff) << ".";
                vhtlog << (int)((m_sockAddr.sin_addr.s_addr>>16) & 0x000000ff) << ".";
                vhtlog << (int)((m_sockAddr.sin_addr.s_addr>>8 ) & 0x000000ff) << ".";
                vhtlog << (int)((m_sockAddr.sin_addr.s_addr    ) & 0x000000ff) << "\n";
#                                   endif
                vhtlog.flush();
#                               endif

                if (success==false){
                    perror();
                }
                assert(success);
            }
            else {
                perror();
            }
        }
        else {
            perror();
        }

        assert(success);
        return success;
    }

    VH_CORE_DLL_EXPORT inline bool         connect(const char *name, int port) const {
                            bool success = false;
                        #ifdef _WIN32
                            HOSTENT *host = NULL;
                        #else
                            hostent *host = NULL;
                        #endif
                            sockaddr_in peer;
                            host = gethostbyname(name);
                            if (host != NULL){
                                // set address:
                                memcpy(&peer.sin_addr, host->h_addr, host->h_length);
#if defined(_DEBUG) && !defined(_WIN32)
                                vhtlog<<"\nDEBUG: gethostbyname(\""<<name<<"\") returned IP = ";
#   ifdef _WIN32
                                vhtlog << (int)peer.sin_addr.S_un.S_un_b.s_b1 << ".";
                                vhtlog << (int)peer.sin_addr.S_un.S_un_b.s_b2 << ".";
                                vhtlog << (int)peer.sin_addr.S_un.S_un_b.s_b3 << ".";
                                vhtlog << (int)peer.sin_addr.S_un.S_un_b.s_b4 << "\n";
                                vhtlog.flush();
#   elif defined (_QNX)
                                vhtlog << (int)((peer.sin_addr.s_addr    ) & 0x000000ff) << ".";
                                vhtlog << (int)((peer.sin_addr.s_addr>>8 ) & 0x000000ff) << ".";
                                vhtlog << (int)((peer.sin_addr.s_addr>>16) & 0x000000ff) << ".";
                                vhtlog << (int)((peer.sin_addr.s_addr>>24) & 0x000000ff) << "\n";
#   else
                                vhtlog << (int)((peer.sin_addr.s_addr>>24) & 0x000000ff) << ".";
                                vhtlog << (int)((peer.sin_addr.s_addr>>16) & 0x000000ff) << ".";
                                vhtlog << (int)((peer.sin_addr.s_addr>>8 ) & 0x000000ff) << ".";
                                vhtlog << (int)((peer.sin_addr.s_addr    ) & 0x000000ff) << "\n";
#   endif
#if !defined(_WIN32)
                                vhtlog.flush();
#endif

#endif
                                // set port:
                                peer.sin_port = htons(port);
                                // set family:
                                peer.sin_family = m_family;
                                success = (::connect(m_socket, (sockaddr*)(&peer), sizeof(sockaddr_in))==0);
                                if (success==false){
                                    perror();
                                }
                            }
                            else {
                                perror();
                            }
                            return success;
                        }
    VH_CORE_DLL_EXPORT inline bool         connect(unsigned char ip1, unsigned char ip2, unsigned char ip3, unsigned char ip4, int port) const {
                            bool success = false;

                            sockaddr_in peer;
                            peer.sin_port                   = htons(port);
                            peer.sin_family                 = m_family;
                        #ifdef _WIN32
                            peer.sin_addr.S_un.S_un_b.s_b1  = ip1;
                            peer.sin_addr.S_un.S_un_b.s_b2  = ip2;
                            peer.sin_addr.S_un.S_un_b.s_b3  = ip3;
                            peer.sin_addr.S_un.S_un_b.s_b4  = ip4;
                        #else 
                            peer.sin_addr.s_addr = (ip1<<24) | (ip2<<16) | (ip3<<8) | (ip4);
                        #endif

                            success = (::connect(m_socket, (sockaddr*)(&peer), sizeof(sockaddr_in))==0);
                            if (success==false){
                                perror();
                            }

                            return success;
                        }

    VH_CORE_DLL_EXPORT inline bool         listen(int backLog=1) {
                            if (::listen(m_socket, backLog)==0){
                                return true;
                            }
                            else {
                                perror();
                                return false;
                            }
                        }
    VH_CORE_DLL_EXPORT inline bool         accept(vhtSocket &s) const {
                            unsigned int size = sizeof(sockaddr);
                            s.m_socket = ::accept(m_socket, (sockaddr*)(&(s.m_sockAddr)), (socklen_t *)&size);
#ifdef _WIN32
                            if (s.m_socket != INVALID_SOCKET){
#else
                            if (s.m_socket != -1){
#endif
                                return true;
                            }
                            else {
                                perror();
                                return false;
                            }
                        }

    VH_CORE_DLL_EXPORT inline int          send(const char *buf, int numBytes) const {
                            int returnVal = ::send(m_socket, buf, numBytes, 0);
                        #ifdef _WIN32
                            if (returnVal != INVALID_SOCKET){
                                return returnVal;
                            }
                            else {
                                perror();
                                return false;
                            }
                        #else
                            if (returnVal != -1){
                                return returnVal;
                            }
                            else {
                                perror();
                                return false;
                            }
                        #endif
                        }
    VH_CORE_DLL_EXPORT inline int          recv(char *buf, int numBytes) const {
                            int returnVal = ::recv(m_socket, buf, numBytes, 0); 
#                       ifdef _WIN32
                            if (returnVal == INVALID_SOCKET){
                                returnVal = -1;
#                       else
                            if (returnVal == -1){
#                       endif
                                perror();
                            }
                            return returnVal;
                        }

    VH_CORE_DLL_EXPORT inline bool         getSockName(unsigned char &ip1, unsigned char &ip2, unsigned char &ip3, unsigned char &ip4, int &port) const {
                            bool success = false;

                            sockaddr_in sockAddr;
                            unsigned int sockAddrSize = sizeof(sockaddr_in);
#if defined(linux)
                            if (getsockname(m_socket, (sockaddr*)(&sockAddr), (unsigned int *)&sockAddrSize)==0){
#else
                            if (getsockname(m_socket, (sockaddr*)(&sockAddr), (int *)&sockAddrSize)==0){
#endif
#ifdef _WIN32
                                ip1 = sockAddr.sin_addr.S_un.S_un_b.s_b1;
                                ip2 = sockAddr.sin_addr.S_un.S_un_b.s_b2;
                                ip3 = sockAddr.sin_addr.S_un.S_un_b.s_b3;
                                ip4 = sockAddr.sin_addr.S_un.S_un_b.s_b4;
#elif defined (_QNX)
                                ip1 = (sockAddr.sin_addr.s_addr    ) & 0x000000ff;
                                ip2 = (sockAddr.sin_addr.s_addr>>8 ) & 0x000000ff;
                                ip3 = (sockAddr.sin_addr.s_addr>>16) & 0x000000ff;
                                ip4 = (sockAddr.sin_addr.s_addr>>24) & 0x000000ff;
#else 
                                ip1 = (sockAddr.sin_addr.s_addr>>24) & 0x000000ff;
                                ip2 = (sockAddr.sin_addr.s_addr>>16) & 0x000000ff;
                                ip3 = (sockAddr.sin_addr.s_addr>>8 ) & 0x000000ff;
                                ip4 = (sockAddr.sin_addr.s_addr    ) & 0x000000ff;
#endif
                                port = ntohs(sockAddr.sin_port);
                                success = true;
                            }
                            else {
                                perror();
                            }

                            assert(success);
                            return success;
                        }

    VH_CORE_DLL_EXPORT inline bool         getPeerName(unsigned char &ip1, unsigned char &ip2, unsigned char &ip3, unsigned char &ip4, int &port) const {
                            bool success = false;

                            sockaddr_in peerAddr;
                            unsigned int peerAddrSize = sizeof(sockaddr_in);

                            if (getpeername(m_socket, (sockaddr*)(&peerAddr), (socklen_t *)&peerAddrSize)==0){

                            #ifdef _WIN32
                                ip1 = peerAddr.sin_addr.S_un.S_un_b.s_b1;
                                ip2 = peerAddr.sin_addr.S_un.S_un_b.s_b2;
                                ip3 = peerAddr.sin_addr.S_un.S_un_b.s_b3;
                                ip4 = peerAddr.sin_addr.S_un.S_un_b.s_b4;
                            #elif defined (_QNX)
                                ip1 = (peerAddr.sin_addr.s_addr    ) & 0x000000ff;
                                ip2 = (peerAddr.sin_addr.s_addr>>8 ) & 0x000000ff;
                                ip3 = (peerAddr.sin_addr.s_addr>>16) & 0x000000ff;
                                ip4 = (peerAddr.sin_addr.s_addr>>24) & 0x000000ff;
                            #else 
                                ip1 = (peerAddr.sin_addr.s_addr>>24) & 0x000000ff;
                                ip2 = (peerAddr.sin_addr.s_addr>>16) & 0x000000ff;
                                ip3 = (peerAddr.sin_addr.s_addr>>8 ) & 0x000000ff;
                                ip4 = (peerAddr.sin_addr.s_addr    ) & 0x000000ff;
                            #endif
                                port = ntohs(peerAddr.sin_port);
                                success = true;
                            }
                            else {
                                perror();
                            }

                            return success;
                        }
    VH_CORE_DLL_EXPORT inline bool         isReadable(unsigned int timeout_sec, unsigned int timeout_usec){
                            fd_set readSet;
                            FD_ZERO(&readSet);
                            FD_SET(m_socket, &readSet);

                            timeval timeout;
                            timeout.tv_sec = timeout_sec;
                            timeout.tv_usec = timeout_usec;

                            int selected = select(1, &readSet, NULL, NULL, &timeout);
                            return (selected==1);
                        }
    
    VH_CORE_DLL_EXPORT static inline int   selectReadable(vhtSocket **sockArray, int sockArraySize, unsigned int timeout_sec=1, unsigned int timeout_usec=0){
                            int i;
                            int nextIndex;
                            fd_set readSet;
                            FD_ZERO(&readSet);
                            for (i=0 ; i<sockArraySize ; i++){
                                FD_SET(sockArray[i]->m_socket, &readSet);
                            }

                            timeval timeout;
                            timeout.tv_sec = timeout_sec;
                            timeout.tv_usec = timeout_usec;

                            int selected = select(sockArraySize, &readSet, NULL, NULL, &timeout);

                            // rearrange sockArray to match the readSet:
                            for (i=0,nextIndex=0 ; i<sockArraySize ; i++){
                                // if this vhtSocket's m_socket is a member of the returned set:
                                if (FD_ISSET(sockArray[i]->m_socket,&readSet)){
                                    // move this vhtSocket to the appropriate index in the sockArray:
                                    sockArray[nextIndex] = sockArray[i];
                                    // increment the index counter:
                                    nextIndex++;
                                }
                            }
                            assert(selected == nextIndex);

                            return selected;
                        }

    VH_CORE_DLL_EXPORT inline bool operator == (const vhtSocket &s) const {
                            return (m_socket==s.m_socket);
                        }

    VH_CORE_DLL_EXPORT inline bool         close(){
                            bool success = 
                            #ifdef _WIN32
                                (closesocket(m_socket)==0);
                            #else 
                                (::close(m_socket)==0);
                            #endif

                            if (success==false){
                                perror();
                            }

                            return success;
                        }
private:

    inline void         perror(const char *str=NULL) const {
                            if (str!=NULL){
#if defined( _DEBUG) && !defined(_WIN32)
                                vhtlog<<"SOCKET ERROR: "<<str<<"\n";
                                vhtlog.flush();
#endif
                            }
                            else {
#ifdef _WIN32
#	if defined( _DEBUG) && !defined(_WIN32)
                                vhtlog << "SOCKET ERROR: Use ::WSAGetLastError() to retrieve error code\n";
                                vhtlog << "DEBUG: WSAGetLastError() returned " << WSAGetLastError() << "\n";
                                vhtlog.flush();
#	endif
#                           else 
                                ::perror("SOCKET ERROR");
#                           endif
                            }

                            //assert(0);
                        }

    static int          sm_socketsRefCount;

    bool                socketsStartup();
    bool                socketsShutdown();

#ifdef _WIN32
    SOCKET              m_socket;
#else
    int                 m_socket;
#endif

    bool                m_isOpen;
    int                 m_family;
    sockaddr_in         m_sockAddr;
};

#endif


