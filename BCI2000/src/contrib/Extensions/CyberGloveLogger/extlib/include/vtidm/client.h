/*============================================================================

  $Id: client.h,v 1.2 2002/10/11 22:33:17 dgomez Exp $

  HISTORY:

============================================================================*/

#ifndef _VT_CLIENT_H_
#define _VT_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "types.h"
#include "device.h"
#include "message.h"
#include "socket.h"
#include "dstream.h"
#include "client-types.h"

/* DCB: device control block */
/*  every instance of the device has a DCB associated with it, 
    the master keep DCBs for all instances*/
typedef struct _VTDID_t {
    LIST_HEAD(_VTDID_t);

    client_socket       ctrlsock; /* tcp sock for remote, unixsock for local*/
    client_socket       strmsock; /* tcp socket for incoming streamint data */
    void *              inst;	/* device instance */
    vt_uchar            type;	/* device type */
    char                url[MAX_URL_SZ]; /* URL for the device */
    vt_ulong            remote_stream; /* remote stream to attach/watch */
    double              lasttime; /* in seconds since 1970.1.1 */
    double              adjtime; /* diff between client time and host time */

    DEVSTATE            state;	/* device mode, streaming... */
} VTDID_t, *VTDID;

enum {
    CLOSE_ONE = 0,		/* minus reference count by one */
    CLOSE_ALL,			/* rm all, guarantee to remove the instance */ 
    CLOSE_ALL_BUT_ONE		/* ref count to 1, the instance still exists */
};
/* functions to access the master */
VT_DLL_PROC VTDID VT_DeviceOpen(char * srvname, char *port, char *device);
VT_DLL_PROC vt_error VT_DeviceCloseEx(VTDID did, int opt);
VT_DLL_PROC vt_error VT_DeviceClose(VTDID did);

VT_DLL_PROC int VT_DeviceControl(VTDID did, vt_uchar msgid, vt_uchar argcode,
				 char *pbody, int * datasz,int bodysz);

VT_DLL_PROC int VT_DeviceManagerControl(VTDID did, vt_uchar msgid, 
					vt_uchar argcode, char *pbody, 
					int * datasz,int bodysz);

VT_DLL_PROC int VT_DeviceManagerLog(VTDID did, char * str);


/* functions derived from VT_DeviceControl */
/* Get prototype */
VT_DLL_PROC int VT_DeviceControlGet(VTDID did, vt_uchar msgid, 
				    vt_uchar argcode, char * buf, int bufsz);
/* set prototype */
VT_DLL_PROC int VT_DeviceControlSet(VTDID did, vt_uchar msgid, 
				    vt_uchar argcode, char * buf, int datasz);

/* a few wrapper functions to hide the msgid */
#define VT_DeviceGetInfo(did, argcode, buf, bufsz) \
VT_DeviceControlGet(did, DM_INFO, argcode, buf, bufsz)

#define VT_DeviceSetData(did, argcode, buf, datasz) \
VT_DeviceControlSet(did, DM_SET_DATA, argcode, buf, datasz)

#define VT_DeviceGetData(did, argcode, buf, bufsz) \
VT_DeviceControlGet(did, DM_GET_DATA, argcode, buf, bufsz)

#define VT_DeviceSetCalibration(did, argcode, buf, datasz) \
VT_DeviceControlSet(did, DM_SET_CALIBRATION, argcode, buf, datasz)

#define VT_DeviceGetCalibration(did, argcode, buf, bufsz) \
VT_DeviceControlGet(did, DM_GET_CALIBRATION, argcode, buf, bufsz)

#define VT_DeviceSetConfig(did, argcode, buf, datasz) \
VT_DeviceControlSet(did, DM_SET_CONFIG, argcode, buf, datasz)

#define VT_DeviceGetConfig(did, argcode, buf, bufsz) \
VT_DeviceControlGet(did, DM_GET_CONFIG, argcode, buf, bufsz)

#define VT_DeviceGetLastUpdateTime(did)  (did->lasttime)

VT_DLL_PROC vt_error VT_DeviceWatch(VTDID did, VT_DATASTREAM strm);
VT_DLL_PROC vt_error VT_DeviceUnwatch(VTDID did);

VT_DLL_PROC vt_bool VT_IsRemoteServer(char * srvopt);
VT_DLL_PROC void VT_ClientExit(); /* cleanup, mostly call from signal handler*/

#ifdef __cplusplus
}
#endif

#endif
