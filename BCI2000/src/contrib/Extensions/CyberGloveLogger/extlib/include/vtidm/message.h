/*===========================================================================
 $Id: message.h,v 1.2 2002/10/11 22:33:17 dgomez Exp $ 

  define all messages and functions used to access messages
  
  changes: 
    JT, 1/13/98: ackcode in msghdr realigned for RISC machines

 ============================================================================*/

#ifndef _VT_MESSAGE_H_
#define _VT_MESSAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _WIN32
#include <sys/time.h>
#include <sys/unistd.h>
#else
#include <sys/timeb.h>
#endif

#include "types.h"
#include "error.h"
#include "device.h"
#include "util.h"

#ifndef _WIN32
static struct timeval g_time_now; /* used to set the time filed of the hdr */
#else
static struct timeb   g_time_now; /* used to set the time filed of the hdr */
#endif
static vt_ushort    g_msg_nseq; /* message sequence number */

#ifndef _IRIX
#pragma pack(1) 
#endif
/* message header structure */
/* members arragned to meet 4-byte boundary */
typedef struct 
{
    char        tag1, tag2, tag3;/* should always be VTI, 
				   for verification or probing */
    vt_uchar    hdrsz;		/* hdr size, for variable length hdr */

    vt_uchar    msgid;		/* Message ID */
    vt_uchar    sndrtype;	/* sender type */
    vt_uchar    rcvrtype;	/* receiver type */
    vt_uchar    argcode;	/* argument code */

    vt_ulong    sndrinst;	/* sndr inst */
    vt_ulong    rcvrinst;	/* receiver VTDPI */

    vt_ushort   seq;		/* sequential number, start from 1 */
    vt_ushort   ackseq;		/* sequence number of acknowledging message */

    vt_ulong    usec;		/* microseconds */
    vt_ulong    sec;		/* time since 1/1/1970 in seconds*/

    vt_uchar    needrply;	/* boolean, does this message need to 
				   be replied */
    char        ackcode;	/* acknowledgement byte*/
    vt_ushort   bodysz;		/* number of bytes of the optional 
				   data segament */
    /* add new fields here */

} vtMsgHdr_t, *vtMsgHdr;

#define HDRADDR(hdr, var)    (&(((vtMsgHdr)(hdr))->var))

/* return to normal packing */

#if defined (_LINUX)  
#pragma pack() 
#elif defined (_QNX) 
#pragma pack (0)  
#elif defined (_sgi) 
#elif defined (_WIN32)  
#pragma pack  () 
#endif  

/* MsgHdrSize should be defined as sizeof(vtMsgHdr_t) */
#define VT_MSG_HDR_SZ  sizeof(vtMsgHdr_t)

/* general reply codes */
#define RPLY_OK                    0
#define RPLY_ERR                   -1

#define MSGERR(err)         ((err < -127)?-1:(char) err)

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*             macros                                                        */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#define VT_IsVTIMsg(msgbuf) (((msgbuf)[0] == 'V') && ((msgbuf)[1] == 'T') \
			  && ((msgbuf)[2] == 'I'))
#define VT_SetVTITag(msgbuf)  ((msgbuf)[0] = 'V', (msgbuf)[1] = 'T', (msgbuf)[2] = 'I')

#define VT_GetMsgHdrSz(msgbuf)   (((vtMsgHdr)(msgbuf))->hdrsz)
#define VT_SetMsgHdrSz(msgbuf, sz) (((vtMsgHdr)(msgbuf))->hdrsz = sz)

#define VT_GetMsgID(msgbuf)   (((vtMsgHdr)(msgbuf))->msgid)
#define VT_SetMsgID(msgbuf, ID) (((vtMsgHdr)(msgbuf))->msgid = ID)

#define VT_GetMsgReceiverType(msgbuf)   (((vtMsgHdr)(msgbuf))->rcvrtype)
#define VT_SetMsgReceiverType(msgbuf, type) (((vtMsgHdr)(msgbuf))->rcvrtype = type)

#define VT_GetMsgReceiverInst(msgbuf)   (VTDPI)VT_GetLong(HDRADDR(msgbuf, rcvrinst))
#define VT_SetMsgReceiverInst(msgbuf, inst) VT_PutLong(HDRADDR(msgbuf, rcvrinst), inst)

#define VT_GetMsgSenderType(msgbuf)   (((vtMsgHdr)(msgbuf))->sndrtype)
#define VT_SetMsgSenderType(msgbuf, type) (((vtMsgHdr)(msgbuf))->sndrtype = type)

#define VT_GetMsgSenderInst(msgbuf)   (VTDPI)VT_GetLong(HDRADDR(msgbuf, sndrinst))
#define VT_SetMsgSenderInst(msgbuf, inst) VT_PutLong(HDRADDR(msgbuf, sndrinst), inst)

#define VT_GetMsgSeq(msgbuf)  VT_GetShort(HDRADDR(msgbuf, seq))
#define VT_SetMsgSeq(msgbuf, data)  VT_PutShort(HDRADDR(msgbuf, seq), (data))

#define VT_GetMsgAckCode(msgbuf) (((vtMsgHdr)(msgbuf))->ackcode)
#define VT_SetMsgAckCode(msgbuf, code) ((((vtMsgHdr)(msgbuf))->ackcode) = code)

#define VT_GetMsgArgCode(msgbuf) (((vtMsgHdr)(msgbuf))->argcode)
#define VT_SetMsgArgCode(msgbuf, code) ((((vtMsgHdr)(msgbuf))->argcode) = code)

#define VT_GetMsgAckSeq(msgbuf)  VT_GetShort(HDRADDR(msgbuf, ackseq))
#define VT_SetMsgAckSeq(msgbuf, data)  VT_PutShort(HDRADDR(msgbuf, ackseq), (data))

#define VT_GetMsgSec(msgbuf)  VT_GetLong(HDRADDR(msgbuf, sec))
#define VT_SetMsgSec(msgbuf, time) VT_PutLong(HDRADDR(msgbuf, sec), time)

#define VT_GetMsgUSec(msgbuf)  VT_GetLong(HDRADDR(msgbuf, usec))
#define VT_SetMsgUSec(msgbuf, time)  VT_PutLong(HDRADDR(msgbuf, usec), time)

#define VT_GetMsgNeedReply(msgbuf) (((vtMsgHdr)(msgbuf))->needrply)
#define VT_SetMsgNeedReply(msgbuf, nr) ((((vtMsgHdr)(msgbuf))->needrply) = nr)

#define VT_GetMsgBodySize(msgbuf) VT_GetShort(HDRADDR(msgbuf, bodysz))
#define VT_SetMsgBodySize(msgbuf, sz) VT_PutShort(HDRADDR(msgbuf, bodysz), (sz))

#define VT_GetMsgSize(msgbuf) (VT_GetMsgBodySize(msgbuf) + VT_MSG_HDR_SZ)
#define VT_GetMsgBody(msgbuf) ((char *)(msgbuf) + VT_MSG_HDR_SZ)


#define VT_ZeroMsgHdr(msgbuf) memset(msgbuf, 0, VT_MSG_HDR_SZ)

#ifdef _WIN32
#define VT_TimestampMsg(msgbuf) ftime(&g_time_now); \
    	                     VT_SetMsgSec(msgbuf, g_time_now.time); \
			     VT_SetMsgUSec(msgbuf, (g_time_now.millitm * 1000))
#else
#define VT_TimestampMsg(msgbuf) gettimeofday(&g_time_now, NULL); \
    	                     VT_SetMsgSec(msgbuf, g_time_now.tv_sec); \
			     VT_SetMsgUSec(msgbuf, g_time_now.tv_usec)
#endif
#define VT_GetMsgTime(msgbuf) (VT_GetMsgSec(msgbuf) + \
			       ((VT_GetMsgUSec(msgbuf))/1.0e6))

/* does message need reply */
#define VT_IsQueryMsg(msgbuf)  (VT_GetMsgNeedReply(msgbuf))

/* is message a reply for query # seq */
#define VT_IsReplyMsg(msgbuf, seq)  (VT_IsVTIMsg(msgbuf) && \
                             (VT_GetMsgAckSeq(msgbuf) == seq))
#define VT_IsReplyMsgOk(msgbuf, seq) (VT_IsReplyMsg(msgbuf, seq) && \
                                (VT_GetMsgAckCode(msgbuf) >=  0)) 

#define VT_SetMsgStream(msgbuf, rcvrtype, rcvrinst, sndrtype, sndrinst, code, arg, sz) \
                              { \
                                  ++g_msg_nseq; \
				  VT_SetVTITag(msgbuf); \
                                  VT_SetMsgHdrSz(msgbuf, VT_MSG_HDR_SZ); \
                                  VT_SetMsgSenderType(msgbuf, sndrtype); \
                                  VT_SetMsgSenderInst(msgbuf, sndrinst); \
				  VT_SetMsgSeq(msgbuf, g_msg_nseq); \
                                  VT_SetMsgReceiverType(msgbuf, rcvrtype); \
                                  VT_SetMsgReceiverInst(msgbuf, rcvrinst); \
                                  VT_SetMsgID(msgbuf, code); \
                                  VT_SetMsgArgCode(msgbuf, arg); \
                                  VT_SetMsgAckCode(msgbuf, 0); \
                                  VT_SetMsgAckSeq(msgbuf, 0); \
				  VT_SetMsgBodySize(msgbuf, sz);\
				  VT_SetMsgNeedReply(msgbuf, FALSE); \
				  VT_TimestampMsg(msgbuf); }
			      

#define VT_SetMsgQuery(msgbuf, rcvrtype, rcvrinst, sndrtype, sndrinst, code, arg, sz) \
                                  { ++g_msg_nseq; \
                                  VT_SetVTITag(msgbuf); \
                                  VT_SetMsgHdrSz(msgbuf, VT_MSG_HDR_SZ); \
                                  VT_SetMsgSenderType(msgbuf, sndrtype); \
                                  VT_SetMsgSenderInst(msgbuf, sndrinst); \
				  VT_SetMsgSeq(msgbuf, g_msg_nseq); \
                                  VT_SetMsgReceiverType(msgbuf, rcvrtype); \
                                  VT_SetMsgReceiverInst(msgbuf, rcvrinst); \
                                  VT_SetMsgAckSeq(msgbuf, 0); \
                                  VT_SetMsgID(msgbuf, code); \
                                  VT_SetMsgArgCode(msgbuf, arg); \
                                  VT_SetMsgAckCode(msgbuf, 0); \
				  VT_SetMsgBodySize(msgbuf, sz);\
				  VT_SetMsgNeedReply(msgbuf, TRUE); \
				  VT_TimestampMsg(msgbuf); }

#define VT_SetMsgReply(msgbuf, ackcode, sz) \
{\
 register unsigned char devtype = VT_GetMsgReceiverType(msgbuf); \
 register VTDPI inst = VT_GetMsgReceiverInst(msgbuf); \
 ++g_msg_nseq; \
 VT_SetMsgReceiverType(msgbuf, VT_GetMsgSenderType(msgbuf)); \
 VT_SetMsgReceiverInst(msgbuf, VT_GetMsgSenderInst(msgbuf)); \
 VT_SetMsgSenderType(msgbuf, devtype); \
 VT_SetMsgSenderInst(msgbuf, inst); \
 VT_SetMsgAckCode(msgbuf, ackcode); \
 VT_SetMsgAckSeq(msgbuf, VT_GetMsgSeq(msgbuf));\
 VT_SetMsgSeq(msgbuf, g_msg_nseq); \
 VT_SetMsgNeedReply(msgbuf, FALSE); \
 VT_SetMsgBodySize(msgbuf, sz);\
 VT_TimestampMsg(msgbuf);}

#define VT_CheckMessage(buf) ((buf) != NULL && VT_IsVTIMsg(buf))

#define VT_ResetMsgSeq()   (g_msg_nseq = 0)
#define VT_GetCurrentMsgSeq()  (g_msg_nseq)

VT_DLL_PROC void VT_PrintVTIMsg(char * msgbuf);
VT_DLL_PROC void VT_DumpVTIMsg(char * msgbuf);

/* constants */
#define VT_DATA_FORMAT_ASCII 0x80
#define VT_DATA_FORMAT_BINARY 0x0
/* for use with various *format command */
#define VT_RequestASCIIData(id)  (id =| VT_DATA_FORMAT_ASCII) 
#define VT_RequestBinaryData(id) (id) 
#define VT_NeedASCIIData(id)  (id & VT_DATA_FORMAT_ASCII) 
#define VT_NeedBinaryData(id) (!VT_NeedASCIIData(id)) 
#define VT_StripFormatBit(id) (id & ~VT_DATA_FORMAT_ASCII)

/* general device IO codes */
/* 0x00 to 0x7f general messages */
/* 0x80 to 0xfe device specific messages */
/* 0xff: extended message */

#define DM_INFO              0	/* return device driver info, can be used to
				   test the driver and the connection to it
				   also can be used to get description of a
				   msgid */

#define DM_OPEN              1	/* open a new device instance */
#define DM_CLOSE             2	/* close the instance */
#define DM_RESET	     3	/* reset the device instance*/

#define DM_GET_LAST_ERROR    4	/* get last error, instance, error string */

#define DM_GET_TIMEOUT       5	/* current timeout value, block stat */
#define DM_SET_TIMEOUT       6

#define DM_GET_LOCK_STATUS   7	/* is lock?, what locked, locker, timeout */
#define DM_SET_LOCK_STATUS   8	/* */

#define DM_GET_CONFIG        9	/* get device parameters, : delimited */
#define DM_SET_CONFIG        10	/* set device parameters (config, switch...) */

#define DM_GET_REPORT_FORMAT 11	/* current or all supported data */
#define DM_SET_REPORT_FORMAT 12	/* what to report for DM_GET_DATA
				   or in stream mode */

#define DM_GET_CONTROL_FORMAT 13 /* current control keywords (touch etc.) */
#define DM_SET_CONTROL_FORMAT 14

#define DM_GET_DATA          15 /* argcode = keyword_id, 0:using current 
				   report fmt 255: the first line of msgbody
				   is fmt str*/
#define DM_SET_DATA          16 /* argcode = keyword_id, 0:using current 
				   ctrl fmt */

#define DM_GET_CALIBRATION_FORMAT 17 /* keywords such as gain_offset, 
					hand_model... */
#define DM_SET_CALIBRATION_FORMAT 18

#define DM_GET_CALIBRATION   19 /* calibration data (points) */
#define DM_SET_CALIBRATION   20 

#define DM_GET_EVENT_FORMAT  21	/* get event names */
#define DM_SET_EVENT_FORMAT  22 /* set event mask */

#define DM_GET_STREAM_EVENTS 23	/* return current events triggering streaming
				   update, or return all supported events*/
#define DM_SET_STREAM_EVENTS 24

#define DM_ATTACH_STREAM     25	/* attach client to this device, data will be
				   streamed to the client according to supplied
				   report fmt str at stream_rate or triggered
				   by events */
#define DM_DETACH_STREAM     26	/* detach client(s) from this device */

#define DM_STREAM_UPDATE     27 /* stream messages */

#define DM_EVENT             28	/* notification of events */

#define VT_CURRENT_MESSAGES  29 

/* !!!            next available id:           !!!
   !!!                   28                    !!! 
   !!!always update this when add new messages !!!*/

#define DM_DEVICE_SPECIFIC  128

#define DM_EXTEND           255 /* extend messages */

/* default argCode */
#define DEFAULT_ARGCODE      0	/* default option for the message type */

/* master control message */

/* client send this message to inform the server of its udp port number
 * server responds with its udp port number, this way, only tcp port
 * need to be configured, the broadcast addr is also set here.
 * note in case user wants to start glove streaming but not udp/broadcast
 * streaming (daq app), user can explictly start/stop them*/


#ifdef __cplusplus
}
#endif

#endif









