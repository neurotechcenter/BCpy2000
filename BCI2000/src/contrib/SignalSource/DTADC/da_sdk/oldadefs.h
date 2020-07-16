#if !defined(_OLDADEFS_)
#define _OLDADEFS_

#include <oltypes.h>

/*-----------------------------------------------------------------------

 Copyright  (C)  1992.   Data  Translation,  Inc.,  100  Locke   Drive,
 Marlboro Massachusetts 01752-1192.

 All rights reserved.  This software is furnished to purchaser under  a
 license  for  use  on a single computer system and can be copied (with
 the inclusion of DTI's copyright notice) only for use in such  system,
 except  as  may  be otherwise provided in writing by Data Translation,
 Inc., 100 Locke Drive, Marlboro, Massachusetts 01752-1192.

 The information in this document is subject to change  without  notice
 and  should not be construed as a commitment by Data Translation, Inc.
 Data Translation, Inc.  assumes no responsibility for any errors  that
 may appear in this document.

 Data Translation cannot assume any responsibility for the use  of  any
 portion  of  this  software  on  any  equipment  not  supplied by Data
 Translation, Inc.

FILE NAME: oldadefs.h

ABSTRACT:

   This unit contains common declarations for the DT Open Layers 
   Data Acquisition Library.


10/13/92
12/1/96     SEB     Modified for v2.5 for new DT3010 features.

04/16/2001	JSJ		Added OLDA_WM_DEVICE_REMOVAL for DT USB Modules...
---------------------------------------------------------------------------*/


/* prototype for OLNOTIFYOPROC needed by driver as well as API */

typedef void (FAR PASCAL *OLNOTIFYPROC) (
   UINT uiMsg,
   WPARAM wParam,
   LPARAM lParam);


typedef enum olss_tag
   {
   OLSS_AD,
   OLSS_DA,
   OLSS_DIN,
   OLSS_DOUT,
   OLSS_SRL,
   OLSS_CT
   }
OLSS;


typedef enum olssc_tag
   {
   OLSSC_MAXSECHANS,
   OLSSC_MAXDICHANS,
   OLSSC_CGLDEPTH,
   OLSSC_NUMFILTERS,
   OLSSC_NUMGAINS,
   OLSSC_NUMRANGES,
   OLSSC_NUMDMACHANS,
   OLSSC_NUMCHANNELS,
   OLSSC_NUMEXTRACLOCKS,
   OLSSC_NUMEXTRATRIGGERS,
   OLSSC_NUMRESOLUTIONS,
   
   OLSSC_SUP_INTERRUPT,
   OLSSC_SUP_SINGLEENDED,
   OLSSC_SUP_DIFFERENTIAL,
   OLSSC_SUP_BINARY,
   OLSSC_SUP_2SCOMP,
   OLSSC_SUP_SOFTTRIG,
   OLSSC_SUP_EXTERNTRIG,
   OLSSC_SUP_THRESHTRIGPOS,
   OLSSC_SUP_THRESHTRIGNEG,
   OLSSC_SUP_ANALOGEVENTTRIG,
   OLSSC_SUP_DIGITALEVENTTRIG,
   OLSSC_SUP_TIMEREVENTTRIG,
   OLSSC_SUP_TRIGSCAN,
   OLSSC_SUP_INTCLOCK,
   OLSSC_SUP_EXTCLOCK,
   OLSSC_SUP_SWCAL,
   OLSSC_SUP_EXP2896,
   OLSSC_SUP_EXP727,
   OLSSC_SUP_FILTERPERCHAN,
   OLSSC_SUP_DTCONNECT,
   OLSSC_SUP_FIFO,
   OLSSC_SUP_PROGRAMGAIN,
   OLSSC_SUP_PROCESSOR,
   OLSSC_SUP_SWRESOLUTION,
   OLSSC_SUP_CONTINUOUS,
   OLSSC_SUP_SINGLEVALUE,
   OLSSC_SUP_PAUSE,
   OLSSC_SUP_WRPMULTIPLE,
   OLSSC_SUP_WRPSINGLE,

   OLSSC_SUP_POSTMESSAGE,
   OLSSC_SUP_CASCADING,
   OLSSC_SUP_CTMODE_COUNT,
   OLSSC_SUP_CTMODE_RATE,
   OLSSC_SUP_CTMODE_ONESHOT,
   OLSSC_SUP_CTMODE_ONESHOT_RPT,

   OLSSC_MAX_DIGITALIOLIST_VALUE,
   OLSSC_SUP_DTCONNECT_CONTINUOUS,
   OLSSC_SUP_DTCONNECT_BURST,
   OLSSC_SUP_CHANNELLIST_INHIBIT,
   OLSSC_SUP_SYNCHRONOUS_DIGITALIO,
   OLSSC_SUP_SIMULTANEOUS_START,
   OLSSC_SUP_INPROCESSFLUSH,

   OLSSC_SUP_RANGEPERCHANNEL,
   OLSSC_SUP_SIMULTANEOUS_SH,
   OLSSC_SUP_RANDOM_CGL,
   OLSSC_SUP_SEQUENTIAL_CGL,
   OLSSC_SUP_ZEROSEQUENTIAL_CGL,

   OLSSC_SUP_GAPFREE_NODMA,
   OLSSC_SUP_GAPFREE_SINGLEDMA,
   OLSSC_SUP_GAPFREE_DUALDMA,

   OLSSCE_MAXTHROUGHPUT,
   OLSSCE_MINTHROUGHPUT,
   OLSSCE_MAXRETRIGGER,
   OLSSCE_MINRETRIGGER,
   OLSSCE_MAXCLOCKDIVIDER,
   OLSSCE_MINCLOCKDIVIDER,
   OLSSCE_BASECLOCK,

   OLSSCE_RANGELOW,
   OLSSCE_RANGEHIGH,
   OLSSCE_FILTER,
   OLSSCE_GAIN,
   OLSSCE_RESOLUTION, 

   OLSSC_SUP_PLS_HIGH2LOW,      
   OLSSC_SUP_PLS_LOW2HIGH,      

   OLSSC_SUP_GATE_NONE,         
   OLSSC_SUP_GATE_HIGH_LEVEL,
   OLSSC_SUP_GATE_LOW_LEVEL,    
   OLSSC_SUP_GATE_HIGH_EDGE,    
   OLSSC_SUP_GATE_LOW_EDGE,     
   OLSSC_SUP_GATE_LEVEL,     
   OLSSC_SUP_GATE_HIGH_LEVEL_DEBOUNCE,
   OLSSC_SUP_GATE_LOW_LEVEL_DEBOUNCE,    
   OLSSC_SUP_GATE_HIGH_EDGE_DEBOUNCE,    
   OLSSC_SUP_GATE_LOW_EDGE_DEBOUNCE,     
   OLSSC_SUP_GATE_LEVEL_DEBOUNCE,

   OLSS_SUP_RETRIGGER_INTERNAL,
   OLSS_SUP_RETRIGGER_SCAN_PER_TRIGGER,

   OLSSC_MAXMULTISCAN,
   OLSSC_SUP_CONTINUOUS_PRETRIG,
   OLSSC_SUP_CONTINUOUS_ABOUTTRIG, 
   OLSSC_SUP_BUFFERING,
   OLSSC_SUP_RETRIGGER_EXTRA,

   OLSSC_NONCONTIGUOUS_CHANNELNUM,
   OLSSC_SUP_SINGLEVALUE_AUTORANGE

   }
OLSSC;

#define OLSSCMIN      OLSSC_MAXSECHANS
#define OLSSCMAX      OLSSC_SUP_GAPFREE_DUALDMA
#define OLSSCCOUNT    (OLSSCMAX - OLSSCMIN + 1)

#define OLSSCEMIN     OLSSCE_MAXTHROUGHPUT
#define OLSSCEMAX     OLSSCE_BASECLOCK 
#define OLSSCECOUNT   (OLSSCEMAX - OLSSCEMIN + 1)
#define OLSSCEOFFSET  (OLSSCE_RESOLUTION - OLSSCEMIN +1)

#define OLSSCNMIN     OLSSC_SUP_PLS_HIGH2LOW
#define OLSSCNMAX     OLSSC_SUP_SINGLEVALUE_AUTORANGE 
#define OLSSCNCOUNT   (OLSSCNMAX - OLSSCNMIN + 1)


typedef enum oldc_tag
   {
   OLDC_ADELEMENTS,
   OLDC_DAELEMENTS,
   OLDC_DINELEMENTS,
   OLDC_DOUTELEMENTS,
   OLDC_SRLELEMENTS,
   OLDC_CTELEMENTS,
   OLDC_TOTALELEMENTS=~0
   }
OLDC;

#define OLDCMIN      OLDC_ADELEMENTS
#define OLDCMAX      OLDC_CTELEMENTS
#define OLDCOUNT     (OLDCMAX - OLDCMIN) + 1

/* for OLDRV_SETCHANNELTYPE         */
#define OL_CHNT_SINGLEENDED         100
#define OL_CHNT_DIFFERENTIAL        101

/* for OLDRV_SETENCODING            */
#define OL_ENC_BINARY               200
#define OL_ENC_2SCOMP               201

/* for OLDRV_SETTRIGGER             */
#define OL_TRG_SOFT                 300
#define OL_TRG_EXTERN               301
#define OL_TRG_THRESH               302
#define OL_TRG_ANALOGEVENT          303
#define OL_TRG_DIGITALEVENT         304
#define OL_TRG_TIMEREVENT           305
#define OL_TRG_EXTRA                306

/* for OLDRV_SETCLOCKSOURCE         */
#define OL_CLK_INTERNAL             400
#define OL_CLK_EXTERNAL             401
#define OL_CLK_EXTRA                402

/* for OLDRV_SETGATETYPE            */
#define OL_GATE_NONE                500
#define OL_GATE_HIGH_LEVEL          501
#define OL_GATE_LOW_LEVEL           502
#define OL_GATE_HIGH_EDGE           503
#define OL_GATE_LOW_EDGE            504
#define OL_GATE_LEVEL               505
#define OL_GATE_HIGH_LEVEL_DEBOUNCE 506
#define OL_GATE_LOW_LEVEL_DEBOUNCE  507
#define OL_GATE_HIGH_EDGE_DEBOUNCE  508
#define OL_GATE_LOW_EDGE_DEBOUNCE   509
#define OL_GATE_LEVEL_DEBOUNCE      510


/* for OLDRV_SETPULSETYPE           */
#define OL_PLS_HIGH2LOW             600
#define OL_PLS_LOW2HIGH             601

/* for OLDRV_SETCOUNTERMODE         */
#define OL_CTMODE_COUNT             700
#define OL_CTMODE_RATE              701
#define OL_CTMODE_ONESHOT           702
#define OL_CTMODE_ONESHOT_RPT       703

/* for OLDRV_SETDATAFLOW */
#define OL_DF_CONTINUOUS            800   
#define OL_DF_SINGLEVALUE           801
#define OL_DF_DTCONNECT_CONTINUOUS  802   
#define OL_DF_DTCONNECT_BURST       803  
#define OL_DF_CONTINUOUS_PRETRIG    804   
#define OL_DF_CONTINUOUS_ABOUTTRIG  805   
 

/* for OLDRV_SETCASCADEMODE*/
#define OL_CT_CASCADE               900
#define OL_CT_SINGLE                901

/* for OLDRV_SETWRAPMODE*/
#define OL_WRP_NONE                 1000
#define OL_WRP_MULTIPLE             1001
#define OL_WRP_SINGLE               1002

/* for OLDRV_GETQUEUESIZES*/
#define OL_QUE_READY                1100
#define OL_QUE_DONE                 1101
#define OL_QUE_INPROCESS            1102

/* NEW to OLDRV_SETTRIGGER             */
#define OL_TRG_THRESHPOS            1200
#define OL_TRG_THRESHNEG            1201

/* for OLDRV_GETRETTRIGGERMODE*/
#define OL_RETRIGGER_INTERNAL          1300
#define OL_RETRIGGER_SCAN_PER_TRIGGER  1301
#define OL_RETRIGGER_EXTRA             1302

/* Offset value for olConfigWriteReg */
#define OL_POWER_STATE				100

#define OL_POWER_OFF				0
#define OL_POWER_ON					1


#define OLDA_WM_TRIGGER_ERROR     (WM_USER+100)
#define OLDA_WM_UNDERRUN_ERROR    (WM_USER+101)
#define OLDA_WM_OVERRUN_ERROR     (WM_USER+102)
#define OLDA_WM_BUFFER_DONE       (WM_USER+103)
#define OLDA_WM_QUEUE_DONE        (WM_USER+104)
#define OLDA_WM_BUFFER_REUSED     (WM_USER+105)
#define OLDA_WM_QUEUE_STOPPED     (WM_USER+106)
#define OLDA_WM_EVENT_ERROR       (WM_USER+107)
#define OLDA_WM_MEASURE_DONE      (WM_USER+108)
#define OLDA_WM_DTCONNECT_DONE    (WM_USER+109)
#define OLDA_WM_DTCONNECT_STOPPED (WM_USER+110)
#define OLDA_WM_EVENT_DONE        (WM_USER+111)
#define OLDA_WM_PRETRIGGER_BUFFER_DONE  (WM_USER+112)
#define OLDA_WM_DEVICE_REMOVAL    (WM_USER+113)
#define OLDA_WM_IO_COMPLETE		  (WM_USER+114)

// (WM_USER+200) is reserved in olspi.h for internal driver messages

#endif



