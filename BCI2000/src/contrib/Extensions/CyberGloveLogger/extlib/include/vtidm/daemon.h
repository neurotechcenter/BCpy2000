/*============================================================================

  $Id: daemon.h,v 1.2 2002/10/11 22:33:17 dgomez Exp $

  HISTORY:

 ============================================================================*/

#ifndef _VT_DAEMON_H
#define _VT_DAEMON_H

#ifdef __cplusplus
extern "C" {
#endif

/* VTDP_NEW, VTDP_DESTROY not implemented, devinst is largely not used
   as there should have only one master daemon (at least before
   we really want to try "server pool"*/
VT_DLL_PROC vt_bool VT_DaemonGetInfo(vt_uchar *devtype, char ** desc);
VT_DLL_PROC vt_bool VT_DaemonInit(void);
VT_DLL_PROC vt_bool VT_DaemonControl(void *msgbuf, int bufsz);
VT_DLL_PROC vt_bool VT_DaemoShutdown(void);

#ifdef __cplusplus
}
#endif

#endif
