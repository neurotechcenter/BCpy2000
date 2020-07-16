#ifndef __ATLANTISEVENTSTRUCTS_H_INCLUDED__
#define __ATLANTISEVENTSTRUCTS_H_INCLUDED__

#ifndef __midl

#include <PacketDef.h>
#include <nrvdate.h>

#endif

#include <InfoStructureSizes.h>  // For LABELSIZE

#define USERLEN 12
#define TEXTLAYOUTLEN 4

typedef struct tagEVENTMARKER
{
   LONG           lEventID;      // Should rename it, it is not used as an ID...
   BOOL           bSystemDetected; //The event was detected by an detection of some kind
                                   // provided by the software.
   NRVDATE        ndtTimeStamp;
   double         dDuration;
   BYTE           Reserved2[16]; // guidSubObjID was here
   GUID           guidUnique;
   BYTE           Reserved3[8];  // llSubObjSize was here
   double         dCreationDate;
   WCHAR          wcCreatedByUser[USERLEN];
   LONGLONG       llTextLen;
   GUID           guidEventType;
   BYTE           Reserved4[16]; // UserID was here
   WCHAR          wcLabel[LABELSIZE];
   BYTE           Reserved5[32];
} EVENTMARKER;

typedef struct tagEVENTPACKET
{
   struct PACKETHEADER   header;
   EVENTMARKER       eventMarker;
   WCHAR             wcText[TEXTLAYOUTLEN];  // Actually unlimited
} EVENTPACKET;

typedef struct tagEVENTSPIKE
{
   WCHAR  wcActive[LABELSIZE];
   WCHAR  wcReference[LABELSIZE];   
   double dLikelihoodDifference;
   double dMaxAmplDifference;
   SHORT  sSign;
   SHORT sGroup;
   TIMESTAMP tstmpTimeStamp;
   BYTE BYTESRESERVED[32];

} EVENTSpike;

typedef struct tagIMPTESTHEADER
{
   GUID guidDeviceID;
   BYTE reserved1[16];  // guidSensorGroupID was here
   ULONG lThreshold;
   ULONG lcElems;
   BYTE BYTESRESERVED[128];
} IMPTESTHEADER;

typedef struct tagIMPTESTEVENTINFO
{
	WCHAR szSensor[LABELSIZE];
   LONG lInputID;
	BOOL bCheckActive;
	double dImpedanceValue;
	LONG lXPos;
	LONG lYPos;
   BYTE BYTESRESERVED[32];
} IMPTESTEVENTINFO;

const unsigned long DET_NORMAL      = 0x0;
const unsigned long DET_DRUG        = 0x1;
const unsigned long DET_FLUID       = 0x2;

const unsigned long DEA_NORMAL       = 0x0;
const unsigned long DEA_INITIATION   = 0x1;
const unsigned long DEA_TERMINATION  = 0x2;
const unsigned long DEA_RATECHANGE   = 0x3;
const unsigned long DEA_BOLUS        = 0x4;

typedef struct tagDATEXEVENT
{
   WCHAR wcDrug[2 * LABELSIZE];
   WCHAR wcUnit[LABELSIZE];
   double dMagnitude;
   unsigned long ulType;
   unsigned long ulAction;
   BYTE BYTESRESERVED[256];
} DATEXEVENT;
#endif
