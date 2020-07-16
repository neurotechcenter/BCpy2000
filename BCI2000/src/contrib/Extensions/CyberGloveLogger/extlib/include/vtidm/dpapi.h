/*============================================================================

  $Id: dpapi.h,v 1.2 2002/10/11 22:33:17 dgomez Exp $
  Device Plug-in Interface defination

  HISTORY:

 ============================================================================*/

#ifndef _VT_DPAPI_H_
#define _VT_DPAPI_H_

#ifdef __cplusplus
extern "C" {
#endif

/* common includes, so include don't have to include them again*/
#include <string.h>

#include "types.h"
#include "debug.h"
#include "error.h"
#include "dstream.h"
#include "device.h"
#include "message.h" 
#include "ipc.h"
#include "multithread.h"
#include "socket.h"
#include "util.h"

#define VTDP_VERSION_MAJOR   1
#define VTDP_VERSION_MINOR   0

#ifndef _WIN32
typedef void * HMODULE;
#endif

/*--------------------------------------------------*/
/* Device Plug-in (VTDP_) function definations      */
/*--------------------------------------------------*/
/* init the so module, and set the plug-in jump table to access master */
typedef vt_error (*VTDP_INITIALIZE)();	
extern vt_error VTDP_Initialize();	

/* create a new instance of the device and open the device */
/* the options can be set later through Control()*/
typedef VTDPI (*VTDP_NEW)(char *link, char *options); 
extern VTDPI VTDP_New(char *link, char *options); 

/* destroy the instance and close the device */
typedef vt_error (*VTDP_DESTROY)(VTDPI devInstance);	
extern vt_error VTDP_Destroy(VTDPI devInstance);	

/* unload the so module */
typedef vt_error (*VTDP_SHUTDOWN)(void); 
extern vt_error VTDP_Shutdown(void); 

/* control the device behavior (state) */
typedef vt_error (*VTDP_CONTROL)(char *phead, char *pbody, int bodysz);	
extern vt_error VTDP_Control(char *phead, char *pbody, int bodysz);	

/* notify the plugin instance of a new stream */
typedef vt_error (*VTDP_NEWDATASTREAM) (VTDPI instance, VT_DATASTREAM strm);
extern  vt_error VTDP_AttachDataStream (VTDPI instance, VT_DATASTREAM strm);

/* notify the plugin of deleting of a data stream */
typedef vt_error (* VTDP_DESTROYDATASTREAM) (VTDPI instance, 
					     VT_DATASTREAM strm);
extern  vt_error VTDP_DetachDataStream (VTDPI instance, VT_DATASTREAM strm);

/* determine whether a device is covered by the plugin */
/* return device minor number if match find, otherwise -1 */
typedef int ( *VTDP_GETMINOR)(char * name);
extern  int VTDP_GetMinor(char * name);

/* DDCB: device driver control block */
/* hold information to call standard device driver callback functions */ 
#define VT_MAGIC_DDCB     0x44444342
typedef struct _VTDDCB_t {
    long            magic;
    vt_ushort       size;
    vt_ushort       version;
    vt_uchar        type;	/* device type */
    char        *   desc;	/* string description, LINK_NAME_SZ */
    vt_ushort       numInstances;
    HMODULE         handle;	/* dll module handle */
    int             debugLevel; 

    VTDP_NEW        New;
    VTDP_DESTROY    Destroy;
    VTDP_GETMINOR   GetMinor;
    VTDP_NEWDATASTREAM AttachDataStream;
    VTDP_DESTROYDATASTREAM DetachDataStream;
    VTDP_CONTROL    Control;
} VTDDCB_t, *VTDDCB;

/* switch board for all device messages (max 255) */
typedef vt_error (*VTDP_MESSAGEHANDLER[256])(char *head, char* body, 
					     int bodysz);

/*----------------------------------------------------------------
 Device manager (master) functions which can be  called by Plug-in  
 -----------------------------------------------------------------*/
/* memory methods*/
typedef void * (*VTDM_MEMALLOC)(vt_uint size);
typedef vt_error (*VTDM_MEMFREE)(void *);

/* Open, close a device */
typedef VTDPI  (*VTDM_OPENURL) (char * url);
typedef vt_error (*VTDM_CLOSEURL) (VTDPI);
typedef vt_error (*VTDM_SENDMESSAGE)(char * phead, char *pbody, int sz);

/* configuration */
#define VT_CONFIG_TYPE_DEVICE  0
#define VT_CONFIG_TYPE_CLIENT  1
#define VT_CONFIG_TYPE_SESSION 2

typedef int (*VTDM_GETCONFIG)(char * key, char *buf, int bufsz);
typedef vt_error (*VTDM_PUTCONFIG)(char * key, char *buf, int bufsz);

/* create, destroy a data stream */
typedef vt_error (*VTDM_NEWDATASTREAM) (VTDPI, VT_DATASTREAM);
typedef vt_error (*VTDM_DESTROYDATASTREAM) (VTDPI, VT_DATASTREAM);

/* allocate, destory a device instance */
extern  void *   VTDM_MemAlloc(vt_uint size);
extern  vt_error VTDM_MemFree(void *);
extern  vt_error VTDM_SendMessage(char *phead, char * pbody, int sz);
extern  VTDPI    VTDM_OpenURL (char * url);
extern  vt_error VTDM_CloseURL (VTDPI);
extern  vt_error VTDM_AttachDataStream (VTDPI, VT_DATASTREAM);
extern  vt_error VTDM_DetachDataStream (VTDPI, VT_DATASTREAM);
extern  int VTDM_GetConfig(char * key, char *buf, int bufsz);
extern  vt_error VTDM_PutConfig(char * key, char *buf, int bufsz);

/* Create and destroy a VTDPI (devcie plug-in instance ) */
extern VTDPI    VTDM_NewDPI(vt_uint data_sz);
extern vt_error VTDM_DestroyDPI(VTDPI);

/* device manager control block */
typedef struct _VTDMCB_t {
    vt_ushort                 size;
    vt_ushort                 version;
    VTDM_MEMALLOC             MemAlloc;
    VTDM_MEMFREE              MemFree;
    VTDM_OPENURL              OpenURL;
    VTDM_CLOSEURL             CloseURL;
    VTDM_NEWDATASTREAM        AttachDataStream;
    VTDM_DESTROYDATASTREAM    DetachDataStream;
    VTDM_SENDMESSAGE          SendMessage; /* sync/blocking */
    VTDM_GETCONFIG            GetConfig;
    VTDM_PUTCONFIG            PutConfig;
} VTDMCB_t, * VTDMCB;

/* the only function resolved and called by master to fill up VTDDCB 
   structure */
typedef vt_error (* VT_PLUGIN_GETENTRYPOINTS)( VTDDCB ddcb);
typedef vt_error (* VT_PLUGIN_INITIALIZE)(VTDMCB dmcb, VTDDCB ddcb);
typedef vt_error (* VT_PLUGIN_SHUTDOWN)();


/* some required global variables for the plugin */
extern VTDDCB     g_ddcb;

extern char    *  g_description;
extern vt_uchar   g_devtype;
extern vt_list    g_dpilist;
extern vt_indexed_string_t g_devnames[];
extern VTDP_MESSAGEHANDLER g_messageHandler;


#ifndef _TEST_MODULE_
#endif

#ifdef __cplusplus
}
#endif

#endif



