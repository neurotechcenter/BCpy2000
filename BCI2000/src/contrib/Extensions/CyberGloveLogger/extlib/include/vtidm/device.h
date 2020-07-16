/*============================================================================

  $Id: device.h,v 1.2 2002/10/11 22:33:17 dgomez Exp $

 ============================================================================*/

#ifndef _VT_DEVICE_H_
#define _VT_DEVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "list.h"

#define MAX_NUM_DEVICES   256
#define MAX_URL_SZ        128
#define DEFAULT_COMMAND_REPLY_CHECK_INTERVAL 10	/* in msec */
#define DEFAULT_COMMAND_REPLY_TIMEOUT_COUNT  20
#define DEFAULT_FPS_PERIOD                   100 /* counts to calc fps */

/* device type IDs */
/* general daemon control and interal comm messages */
#define VT_MASTER      0	/* master */
#define VT_CLIENT      1
#define VT_SOCKET      2	/* master socket server  */
#define VT_GLOVE       3	/* with touch options */
#define VT_TRACKER     4	/* magnetic trackers */
#define VT_GRASP       5	/* cybergrasp */
#define VT_GESTUREPLUS 6
#define VT_SERIAL      7	/* serial manager */
#define VT_STYLUS3     8	/* phantom, forcetrak, microscribe etc */


#define VT_CURRENT_DEVICES 10

#define VT_USER1       128
#define VT_USER2       129
#define VT_USER3       130
#define VT_USER4       131
#define VT_USER5       132
#define VT_USER6       133
#define VT_USER7       134
#define VT_USER8       135
#define VT_USER9       136
#define VT_USER10      137
#define VT_USER11      138
#define VT_USER12      139
#define VT_USER13      140
#define VT_USER14      141
#define VT_USER15      142
#define VT_USER16      143
#define VT_USER17      144

/*----------------------------------------*/
/* device data packet                     */
/*----------------------------------------*/

/* device mode */
/* ideally (i.e. everything works), there are only two stable states 
   DS_STREAM and DS_POLL. All transent state should be constraint by
   time-out to avoid device hang,and provide active error notification */
#define   DS_ERROR         0x1	/* device has error other than comm error */
#define   DS_STOP          0x2	/* device stopped, normally not used */
#define   DS_STREAM        0x4	/* data streaming in */
#define   DS_POLL	   0x8	/* COMMAND_READY state */
#define   DS_COMMAND_SENT  0x10 /* transent state, back to POLL or STREAM*/
#define   DS_INIT          0x20 /* transent state, back to POLL or STREAM*/

typedef unsigned char DEVSTATE;

#define VT_IsDeviceStream(dstat)    (dstat & DS_STREAM)
#define VT_IsDevicePoll(dstat)    (dstat & DS_POLL)
#define VT_SetDevicePollFlag(dstat) (dstat = (dstat | DS_POLL) & ~DS_STREAM)
#define VT_SetDeviceStreamFlag(dstat) (dstat = (dstat | DS_STREAM) & ~DS_POLL)
#define VT_IsDeviceReady(dstat)    (VT_IsDeviceStream(dstat) || \
                                    VT_IsDevicePoll(dstat))

#define VT_IsDeviceReplied(dstat) (VT_IsDeviceReady(dstat) && \
                                   (dstat & DS_COMMAND_SENT == 0)) 

#ifdef _DEBUG
VT_DLL_PROC void VT_PrintDeviceState(DEVSTATE stat);
#else
#define VT_PrintDeviceState(stat)
#endif

typedef struct {
    vt_ulong   period;        
    vt_ulong   count;
    double     lasttime;	/* last measured time */
    double     fps;
} VTFPS_t, *   VTFPS;

VT_DLL_PROC void VT_FpsInit(VTFPS fps, vt_ulong update_period);
VT_DLL_PROC vt_bool VT_FpsUpdate(VTFPS fps); /* return true if fps is updated */
#define VT_FpsGet(fps)  ((fps)->fps)

/*----------------------------------------*/
/* device plug-in (DP) instance           */
/*----------------------------------------*/
/* FIXME: this should be called VTDPI_LIST, which is a LIST with pdata
   setting to VTDPI, but we have to dereference twice to reach the
   members. */
typedef struct _VTDPI_t {
    /* list items */
    LIST_HEAD(_VTDPI_t);

    vt_uchar             type;	/* device type */
    char                 name[MAX_URL_SZ]; /* local link name, e.g. */
    int                  debugLevel; /* 0: no debug message */
    vt_error             lasterror;
    int                  numClients; /* number of clients sharing this inst */
    VTFPS_t              fps_t;
#ifdef DPI_LOCK
    vt_mutex             lock;
#endif

    /* master private data */
    void *               mdata;	
    /* plug-in (driver) private data */
    void *               pdata;
} VTDPI_t, * VTDPI;

#define VT_InstanceGetLastUpdateTime(inst) ((inst)->fps_t.lasttime)
#define VT_InstanceSetLastUpdateTime(inst, t) ((inst)->fps_t.lasttime=t)
#define VT_InstanceSetLastError(inst, errcode) ((inst)->lasterror = (errcode))
#define VT_InstanceGetLastError(inst)      ((inst)->lasterror)
#define VT_InstanceSetDebugLevel(l) ((inst)->debugLevel = l)

#define VT_PrintInstance(inst) \
{ \
    VT_Verbose("inst:       %x\n", inst); \
    VT_Verbose("  type:     %d\n", inst->type);\
    VT_Verbose("  name:     %s\n", inst->name);\
    VT_Verbose("  lasterror:%d\n", inst->lasterror);\
    VT_Verbose("  dbglevel: %d\n", inst->debugLevel); \
    VT_Verbose("  nclients: %d\n", inst->numClients);\
    VT_Verbose("  lasttime: %f\n", inst->lastUpdateTime);\
    VT_Verbose("  fps:      %f\n", inst->fps_t.fps);\
    VT_Verbose("  mdata:    %x\n", inst->mdata);\
    VT_Verbose("  pdata:    %x\n", inst->pdata);\
}

/* following functions required all VTDPI has link[LINK_NAME_SZ] declared
   right after standard list_base items (magic, prev, next) */
#define VT_CompareInstance(inst1, inst2) ((inst1) == (inst2))
VT_DLL_PROC vt_bool VT_CompareInstancesByName(VTDPI inst1, VTDPI inst2);
VT_DLL_PROC VTDPI   VT_GetInstanceByName(vt_list lst, char * name);

/*VT_DLL_PROC vt_bool VT_DPIIsValid(vt_list lst, VTDPI inst);*/
/* disable isitemoflist test, as the the item is cut loose and no longer
   belongs to the list by the time the fpdel is called. */
#define VT_DPIIsValid(lst, inst)  (VT_ListItemIsValid(inst))

#ifdef DPI_LOCK
VT_DLL_PROC vt_error VT_DPILockRead(VTDPI inst);
VT_DLL_PROC vt_error VT_DPIUnlockRead(VTDPI inst);
#define VT_DPILockWrite VT_DPILockRead
#define VT_DPIUnlockWrite VT_DPIUnlockRead
#endif

#ifdef __cplusplus
}
#endif

#endif







