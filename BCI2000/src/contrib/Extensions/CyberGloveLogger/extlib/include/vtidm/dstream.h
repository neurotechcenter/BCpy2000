/*============================================================================

  $Id: dstream.h,v 1.2 2002/10/11 22:33:17 dgomez Exp $

  HISTORY:

 ============================================================================*/

#ifndef _VT_DSTREAM_H_
#define _VT_DSTREAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "device.h"
#include "list.h"

#define VT_DATASTREAM_ALL     1000000 /* a large number */

/*
 * reasons to call Data stream callback
 * negative number is the same as vt_error, positive number for 
 * all other message passing purpose
 */
#define VT_DATASTREAM_REASON_UPDATE    0 /* normal */
/* announce that new data is available (posted somewhere else, instead of push
   onto the stream directly)*/
#define VT_DATASTREAM_REASON_NEW_DATA_POSTED  1
#define VT_DATASTREAM_REASON_PASS_THROUGH     2

#define VT_DATASTREAM_REASON_QUIT      3 /* stream head ready to quit */
#define VT_DATASTREAM_REASON_BUFFER_FULL      4

typedef int (*VT_DATASTREAM_CALLBACK) (void * , int reason);

typedef struct _VT_DATASTREAM_t {
    /* list items */
    LIST_HEAD(_VT_DATASTREAM_t);
 
    int     size;		/* size of the stream buf */
    int     head;		/* head of the data in buf */
    int     tail;		/* tail of the data in buf */
    double  lastmodified;
    void *  sndrdata;
    void *  rcvrdata;
    VT_DATASTREAM_CALLBACK callback;

    char *  buf;
    vt_bool buf_internal;	/* is buffer buffer internal or external */

} VT_DATASTREAM_t, * VT_DATASTREAM;

#define VT_DataStreamIsValid VT_ListItemIsValid

#define VT_DataStreamIsEmpty(strm)  ((strm)->head == -1 && (strm)->tail == -1)
VT_DLL_PROC int   VT_DataStreamGetBytesReady(VT_DATASTREAM strm);
#define VT_DataStreamGetHeadPos(strm) (strm->head)

#define VT_DataStreamGetContinousEmptyBytes(strm) ((strm->head < strm->tail)?\
                                                   (strm->size - strm->tail):\
                                                   (strm->head - 1));
#define VT_DataStreamSetSenderData(strm, sndrd) (strm->sndrdata = (sndrd))
#define VT_DataStreamGetSenderData(strm) (strm->sndrdata)
#define VT_DataStreamSetReceiverData(strm, rcvrd) (strm->rcvrdata = (rcvrd))
#define VT_DataStreamGetReceiverData(strm) (strm->rcvrdata)

/* consumer receives data from sender upstream, and should be the one
   creates data stream and attaches it to the sender 
 */
VT_DLL_PROC VT_DATASTREAM  VT_DataStreamNew(vt_uint size, 
					    VT_DATASTREAM_CALLBACK cb,
					    void * rcvrdata);

VT_DLL_PROC vt_error VT_DataStreamDestroy(VT_DATASTREAM strm);

VT_DLL_PROC vt_error VT_DataStreamSetBuffer(VT_DATASTREAM strm, char * buf,
					    int sz);

#define VT_DataStreamPurge(strm) (strm->head = strm->tail = -1)

VT_DLL_PROC char VT_DataStreamGetNextChar(VT_DATASTREAM strm, int * pos);
VT_DLL_PROC int  VT_DataStreamRead(VT_DATASTREAM strm, char * buf, int bufsz);
VT_DLL_PROC int  VT_DataStreamWrite(VT_DATASTREAM strm, char * buf, int sz);
VT_DLL_PROC int  VT_DataStreamScan(VT_DATASTREAM strm, vt_uchar ch, int nmax);
/* scan for 8bit binary byte (bit 8 = 1 instead of 0) from stream */
VT_DLL_PROC int  VT_DataStreamScan8Bit(VT_DATASTREAM str, int nmax);
VT_DLL_PROC int  VT_DataStreamScanStr(VT_DATASTREAM strm, char * str, int nmax);

/* read in data and put it in stream */
typedef int (* VT_DATASTREAM_IOPROC) (void *, char * buf, int sz);

VT_DLL_PROC int  VT_DataStreamIn(VT_DATASTREAM strm, 
				     VT_DATASTREAM_IOPROC inproc,
				     void * fd);
VT_DLL_PROC int  VT_DataStreamOut(VT_DATASTREAM strm,
				      VT_DATASTREAM_IOPROC outproc,
				      void *fd);
/* report error */
VT_DLL_PROC int VT_DataStreamAnnounce(VT_DATASTREAM strm, int reason);
				   
#ifdef _DEBUG
VT_DLL_PROC void VT_DataStreamPrint(VT_DATASTREAM strm); 
#else
#define VT_DataStreamPrint(strm)  
#endif
#ifdef __cplusplus
}
#endif

#endif




