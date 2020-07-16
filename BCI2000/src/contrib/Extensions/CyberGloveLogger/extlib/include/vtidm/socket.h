/*============================================================================

  $Id: socket.h,v 1.3 2002/10/11 22:33:16 dgomez Exp $

  HISTORY:

 ============================================================================*/

#ifndef _VT_SOCKET_H_
#define _VT_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif
#ifdef _WIN32
#include <windows.h>		/* for sock_addr */
#else
#include <sys/socket.h>		/* for sock_addr */
#include <netinet/in.h>
#endif

#ifdef _IRIX65
#pragma pack(0)
#endif

#include "types.h"
#include "error.h"
#include "util.h"
#include "list.h"
#include "debug.h"
#include "multithread.h"

#define VT_TCP_SOCKET   0
#define VT_UDP_SOCKET   1
#ifndef _WIN32
#define VT_UNIX_SOCKET  2
#else  /* Win32 doesn't support unix socket */
#define VT_UNIX_SOCKET  -1
#endif 

#define VT_MAX_SOCKET_CLIENTS   "10"

#define VT_SOCKET_TIMEOUT 1000          /* time out in msec */
#define VT_SOCKET_CLIENT_IDLE_DIE_COUNT 300 /* 5 minutes */

#ifdef _WIN32
#define VTSOCKET SOCKET
#define VT_SocketIsValid(s)  (s != INVALID_SOCKET)
#undef EWOULDBLOCK
#define EWOULDBLOCK   WSAEWOULDBLOCK
#else
#define VTSOCKET int
#define INVALID_SOCKET -1		/* win32 defined it as ~0 */
#define SOCKET_ERROR   -1
#define VT_SocketIsValid(s) ( s > 0)
#endif /* _WIN32 */

/* proto type for service function, read message from socket stream,
   and generate response, it has to maintain its own buffer */
typedef vt_error (* SOCKET_SERVICE_PROC)(void *);

/* set type, selected, datasz all as int, to avoid alignment problem */
typedef struct _client_socket_t {
    LIST_HEAD (_client_socket_t);

    int	                type;	/* type of socket */
    VTSOCKET            sock;

    struct sockaddr_in  addr;	/* addr to send, for UDP */

    int                 idlecnt; /* client socket idle count, for STERAM.
				   if it is over VT_SOCKET_CLIENT_DIE_COUNT,
				   server will close it*/
    int                 selected; /* flag: FD_ISSET, for STREAM */
    int                 read_timeout;
    int                 write_timeout;
    vt_bool             keep_going; /* flag: should worker keep watching */
    
    int                 datasz;
    vt_thread           worker_thread;
    SOCKET_SERVICE_PROC service_proc;
    void *              service_proc_data;
    char                buf[BUFSZ];
} client_socket_t, * client_socket;

typedef struct _server_socket_t {
    int			type;
    VTSOCKET	        sock;
    fd_set		afds;	  /* current active fds incl. srvr and clnts */
    VTSOCKET		maxfd;	  /* maximum fd incl. srvr and clnts  */
    vt_list             client_list;  /* points to the first client */
    vt_ushort           maxclients;
    vt_thread           worker_thread;

    SOCKET_SERVICE_PROC service_proc;
    void *              service_proc_data;

    vt_bool             keep_going; /* flag: should worker keep watching */
} server_socket_t, * server_socket;


/* high level functions */
/* valid types are "tcp", "udp" and "unix" corresponding to protocol in cfg*/
VT_DLL_PROC server_socket VT_ServerSocketOpen(char *url);
VT_DLL_PROC client_socket  VT_ServerSocketAccept(server_socket s);
VT_DLL_PROC void VT_ServerSocketClose(server_socket ss);	

VT_DLL_PROC client_socket VT_ClientSocketOpen(char *url);
VT_DLL_PROC int VT_ClientSocketClose(client_socket cs);
VT_DLL_PROC int VT_ClientSocketRead(client_socket cs, char *buf, int sz);
VT_DLL_PROC int VT_ClientSocketWrite(client_socket cs, char * buf, int sz);

#define VT_ClientSocketSetReadTimeout(sock, to)  (sock->read_timeout = to)
#define VT_ClientSocketGetReadTimeout(sock)  (sock->read_timeout)
#define VT_ClientSocketSetWriteTimeout(sock, to)  (sock->write_timeout = to)
#define VT_ClientSocketGetWriteTimeout(sock)  (sock->write_timeout)


/* start a seperate thread to manager the client socket */
VT_DLL_PROC vt_error VT_ClientSocketWatch(client_socket cs,
					  SOCKET_SERVICE_PROC service_proc);
VT_DLL_PROC vt_error VT_ClientSocketUnwatch(client_socket cs);

VT_DLL_PROC vt_error VT_ServerSocketWatch(server_socket pss, 
					  SOCKET_SERVICE_PROC service_proc);


VT_DLL_PROC int VT_IsRemoteAddress(char * srvopt);

#define VT_INVALID_ADDR -1
#define VT_LOCAL_ADDR  0
#define VT_REMOTE_ADDR 1

VT_DLL_PROC void VT_SocketClose(VTSOCKET s);

/* messages */
VT_DLL_PROC int VT_ClientSocketSendMessage(client_socket pcs, char * phead,
					  char  * pbody);
VT_DLL_PROC int VT_ClientSocketRecvMessage(client_socket pcs, char * phead, 
					  char * pbody, int bodysz);

#ifdef __cplusplus
}
#endif

#endif











