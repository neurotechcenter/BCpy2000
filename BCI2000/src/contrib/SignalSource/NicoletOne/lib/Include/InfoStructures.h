#ifndef __INFOSTRUCTURES_H__
#define __INFOSTRUCTURES_H__

#ifndef __midl

#include <nrvdate.h>

#endif

#include <InfoStructureSizes.h>

typedef struct tagSleepScoreHeader
{
  LONG lScoreType;                  //Type of sleep score
  DWORD dwReserved;
  BYTE RESERVEDBYTES[512];
} SLEEPSCOREHEADER;

typedef struct tagTRENDHEADER
{
   long lCommonDuration;
   BYTE RESERVEDBYTES[508];
} TRENDHEADER;

typedef struct tagNOINFO
{
   BYTE RESERVEDBYTES[512];
} NOINFO;

/*      TIME SERIES INFO                                         */

typedef struct tagTSHEADER
{
   double         dNotch;              // Notch value (50 or 60 Hz)
   BYTE RESERVEDBYTES[512];
} TSHEADER;

typedef struct tagFILEINFOHEADER
{
   BOOL bReadOnly; //
   DWORD dwReserved1;
   BYTE RESERVEDBYTES[512];
} FILEINFOHEADER;


typedef struct tagNRVERRORINFOHEADER
{
   WCHAR    wcDescription[ERRORTEXTSIZE];
   WCHAR    wcSource[ERRORTEXTSIZE];
   HRESULT  hrError;
   DWORD dwReserved1;
   BYTE RESERVEDBYTES[512];
} NRVERRORINFOHEADER;

typedef struct tagCHANNELHEADER
{
   BYTE reserved1[16];  // guidSensorGroupID was here
   GUID guidDeviceID;
   BYTE RESERVEDBYTES[488];
} CHANNELHEADER;

typedef struct tagVIDEOHEADER
{
   long lVideoType;
   long lFrameRate;
   long lMaxFileDuration;
   long lSyncInterval;
   BYTE RESERVEDBYTES[512];
} VIDEOHEADER;

typedef struct tagPAGEHEADER
{
   double dTimeBase;
   long lTimeBaseType;
   long lBackColor;
   long lMajorGridlinesFreq;
   long lMinorGridlinesFreq;
   long lMajorGridlinesStyle;
   long lMinorGridlinesStyle;
   long lMajorGridlinesColor;
   long lMinorGridlinesColor;
   BOOL bLabelsOn;
   BOOL bMontageWatermark;
   BOOL bPalettesOn;
   BOOL bEventsOnTop;
   BYTE RESERVEDBYTES[504];
} PAGEHEADER;

typedef struct tagACCINFOHEADER
{
   double dLowCut;
   double dHighCut;
   double dStopLowCut;
   double dStopHighCut;
   double dDeflection;
   double dSensitivity;
   BOOL   bNotch;
   DWORD  dwReserved1;
   BYTE   RESERVEDBYTES[512];
} ACCINFOHEADER;

typedef struct tagRECCTRL
{
   DWORD dwReserved1;
   LONG lMaxFileLength;
   BOOL bContImpTest;
   LONG lImpThreshold;
   BYTE RESERVEDBYTES[512];
} RECCTRL;

typedef struct tagFOURIERSETTINGS
{
   double   dDuration;
   double   dFreqResolution;
   long     lWindow;
   BOOL     bOverlap;
   long     lDeTrend;
   DWORD dwReserved1;
   double   dSpectralEdge;
   BYTE RESERVEDBYTES[504];
} FOURIERSETTINGS;

typedef struct tagHWHEADER
{
   // Amp info
   BOOL bHWOn;
   BOOL bGenerationMode;
   DWORD dwAmpAddr;
   DWORD dwSerialNo;
   DWORD dwReserved1;
   DWORD dwHardwareVer;
   DWORD dwSoftwareVer;
   DWORD dwMasterChannels;
   DWORD dwSlaveChannels;
   DWORD dwMasterAmp;
   DWORD dwSlaveAmp;
   DWORD dwAmpConfig;
   WCHAR wcAmpConfigName[HWLABELSIZE];
   WCHAR wcMasterAmpName[HWLABELSIZE];
   WCHAR wcSlaveAmpName[HWLABELSIZE];
   // Photic info
   DWORD dwPhoticAddress;
   DWORD dwPhoticSerialNo;
   DWORD dwPhoticWordcount;
   DWORD dwPhoticMode;
   DWORD dwPhoticProgMode;
   DWORD dwPhoticRunAutoTrain;
   DWORD dwPhoticMainsOn;
   DWORD dwPhoticStim;
   DWORD dwPhoticOverTemp;
   DWORD dwPhoticIntensity;
   DWORD dwPhoticStatus;
   DWORD dwPhoticHardwareVer;
   DWORD dwPhoticSoftwareVer;
   //USB info
   WORD  wUSBContrSerialNo;
   WORD  wUSBDigitalIO;
   WORD  wUSBTxOverPipe;
   WORD  wUSBModeStatus;
   WORD  wUSBTxErrorsOverPipe;
   WORD  wUSBMemory;
   WORD  wUSBVersion;
   WORD  wUSBBulkOverflow;
   WORD  wUSBNoOfHostIN;
   WORD  wUSBNoOfHostOUT;
   //DSP info
   WORD  wDSPSerialNo;
   WORD  wDSPChannels;
   WORD  wDSPModeStatus;
   WORD  wDSPMemory;
   WORD  wDSPHwVersion;
   WORD  wDSPSwVersion;

   LONGLONG llSamplesDropped;
   LONGLONG llSamplesAdded;
   BYTE RESERVEDBYTES[916];
} HWHEADER;

typedef struct tagFOURIERSETTINGSHEADER
{
   double      dDuration;        // Duration in FFT transform
   double      dOverlap;         // Overlap in FFT transform
   LONG        lDetrend;         // None, Constant or Linear
   LONG        lWindow;          // Window function used in FFT transform
   double      dTargetRes;       // User defined resolution
   double      dSpectralEdge;    // User definded spectral edge threshold
   BYTE        RESERVEDBYTES[512];
} FOURIERSETTINGSHEADER;

///////////////////////////////////////////////////////////////////////////////////////////

typedef struct tagTSINFO
{
   WCHAR          szLabel[TSLABELSIZE];
   WCHAR          szActiveSensor[LABELSIZE];
   WCHAR          szRefSensor[LABELSIZE];
   double         dLowCut;
   double         dHighCut;
   double         dSamplingRate; // #Samples pr. second
   double         dResolution;   // Units (µV, %, ...) per sample bit
   BOOL           bSpecialMark; 
   BOOL           bNotch;        // TRUE if notch filter is selected
   double         dOffset;     // Y offset, is normally 0
   BYTE           RESERVEDBYTES[248];
} TSINFO;


typedef struct tagSIGNALINFO
{
   WCHAR    szSignalName[LABELSIZE]; 
   WCHAR    szUnit[UNITSIZE];
   GUID     guidSignalType;
   BOOL     bBipolar;
   BOOL     bAC;
   BOOL     bHighFilter;
   LONG     lDefaultColor;
   BYTE     Reserved[256];
} SIGNALINFO;

typedef struct tagSENSORINFO
{
   WCHAR    szSensorName[LABELSIZE]; 
   WCHAR    szTransducerType[TRANSDUCERSIZE];
   double   dUnitsPrMicroVolt; // unit pr µV
   double   dOffsetUnits; // Offset in units
	double	dMaxIn;
	double	dMinIn;
   double   dX1;
   double   dX2;
   double   dX3;
   BOOL     bDigital;
   BYTE     Reserved[252];
} SENSORINFO;

typedef struct tagDERIVEDSIGNALINFO
{
   WCHAR    szDerivedSignalName[LABELSIZE]; 
   WCHAR    szSourceSignalName[LABELSIZE];
   double   dWeight;
   BYTE     Reserved[256];
} DERIVEDSIGNALINFO;


typedef struct tagDERIVATIONINFO
{
   WCHAR          szLabel[TSLABELSIZE];
   WCHAR          wcActiveSensor[LABELSIZE];
   WCHAR          wcReferenceSensor[LABELSIZE];
   BOOL           bDerived;
   BOOL           bSpecial;
   BYTE           RESERVEDBYTES[256];
} DERIVATIONINFO;


typedef struct tagFILTERINFO
{
   // BandPass filter
   double         dLowCut;             //Hz value of a low cut filter
   double         dHighCut;            //Hz value of a high cut filter
   // BandStop filter
   double         dStopLowCut;         //Hz value of a low cut filter
   double         dStopHighCut;        //Hz value of a high cut filter
   // Notch filter
   BOOL           bNotch;              //TRUE if notch filter is selected
   BOOL           bSpecial;
   BYTE           RESERVEDBYTES[32];
} FILTERINFO;

typedef struct tagDISPLAYINFO
{
   double         dSensitivity;        //as in microvolts per cm
   double         dDeflection;         //mm of trace before cutoff
   double         dBaseLevel;          //Level in measurement units of the display baseline
   LONG           lDisplayType;        //0 = Off, 1 = Trace, 2 = numerical
   LONG           lPolarity;           //positive trace direction up (1) or down (-1)
   LONG           rgbColor;            //rgb color value of trace
   BOOL           bSpecial;
   BYTE           RESERVEDBYTES[128];
} DISPLAYINFO;

typedef struct tagFILEINFO
{
   NRVDATE        ndtStart;
   double         dDuration;
   BYTE           RESERVEDBYTES[128];
} FILEINFO;

typedef struct tagSRINFO
{
   double         dNewSamplingRate;
   BYTE           RESERVEDBYTES[128];
} SRINFO;

// This Packet is written to file 
// it gives vital information about what trend calc and 
// overview nodes have to be created when the file is opened
// This packet corresponds almost to the elements in TrendTemplateCache
typedef struct tagTRENDSETTINGSINFO  
{
   GUID guidOverview;                  // The application can use this guid to create the appropriate
                                       // overview node.
   GUID guidUnique;                    // A unique guid created at runtime. A dynamic packet stream
                                       // with the same guid is created in the file that contains the 
                                       // data for the trend 
   WCHAR wcLabel[LABELSIZE];           // The display 
   WCHAR wcName[LABELSIZE];            // The name of the trend
   WCHAR wcActiveSensor[LABELSIZE];    // Active
   WCHAR wcReferenceSensor[LABELSIZE]; // Reference
   WCHAR wcUnit[LABELSIZE];            // Unit (e.g. "bpm", "µV" etc.)
   double dLowCut;
   double dHighCut;
   double dMinY;
   double dMaxY;
   BOOL bSpecialScaling;
   BOOL bLogarithmic;
   BOOL bReserved1;                    // Previously bRectified (never uesed)
   BOOL bNumericalValue;
   GUID guidTrend;
   long lValueColor;
   DWORD wdReserved1;
   double dBlockDuration;
   long lcAddChannels;
   BYTE RESERVEDBYTES[484];
} TRENDSETTINGSINFO;

typedef struct tagEVENTTYPEINFO
{
   GUID  guidEventType;
   GUID  guidEventClassID;
   WCHAR wcDescription[DESCRIPTIONSIZE];
   WCHAR wcAbbrevation[ABBREVATIONSIZE];
   BYTE Reserved1[8];
   LONG  lColor;
   LONG  lCategoryID;
   LONG  lPriorityID;
   BOOL  bDuration;
   long  lSecurity;
   long  lEditType;
   BYTE  RESERVEDBYTES[512];
} EVENTTYPEINFO;


typedef struct tagAUDIOINFO
{
   BOOL bOn;
   LONG lLeftVolume;
   LONG lRightVolume;
   DWORD dwReserved1;
   BYTE RESERVEDBYTES[128];
} AUDIOINFO;

typedef struct tagCHANNELINFO
{
   WCHAR wcSensor[LABELSIZE];
	double dSamplingRate;
	BOOL bOn;
	LONG lInputID;
   LONG lInputSettingsID;
   DWORD dwReserved1;
   BYTE RESERVEDBYTES[128];
}CHANNELINFO;

typedef struct tagINPUTINFO
{
   LONGLONG lInputID;
	LONG lInputIndex;
	LONG lXPos;
	LONG lYPos;
	DWORD dwDigitalInputMask;
   BYTE RESERVEDBYTES[128];
}INPUTINFO;

typedef struct tagINPUTSETTINGSINFO
{
	double dResolution;
	double dLowCut;
	double dHighCut;
   BOOL bTakesSlave;
   BOOL bDigital;
   BOOL bAC;
	LONG lBits;
	BOOL bImpedance;
   LONG lInputSettingsID;
   BOOL bBipolar;
   BYTE RESERVEDBYTES[124];
}INPUTSETTINGSINFO;

typedef struct tagPHOTICINFO
{
   LONG lFrequency;
   LONG lDuration;
   LONG lLuminance;
   DWORD dwReserved1;
   BYTE RESERVEDBYTES[32];
}PHOTICINFO;

typedef struct tagTRIGGEREVENTINFO
{
   GUID guidEventType;
   LONG lSecsBefore;
   LONG lSecsAfter;   
   BYTE RESERVEDBYTES[512];
} TRIGGEREVENTINFO;

typedef struct tagALARMEVENTINFO
{
   GUID  guidEventType;
   WCHAR wcAction[ALARMACTIONSIZE];
   LONG  lActionType;
   BOOL  bAddToEventLog;
   BYTE RESERVEDBYTES[512];
} ALARMEVENTINFO;

typedef struct tagSYNCINFO
{
	LONGLONG  llPos;					// Frame number
	NRVDATE nrv;
	WCHAR  wcFile[DESCRIPTIONSIZE];	//The name of the avi without path
	BYTE BYTESRESERVED[64];
} SYNCINFO;

typedef struct tagSLEEPSCOREINFO
{
   WCHAR wcID[LABELSIZE];				//Unic name of the sleep score
	WCHAR wcLabel[LABELSIZE];        //Label of the SleepScore
   LONG lEpochLength;
   DWORD dwReserved;
   BYTE RESERVEDBYTES[512];
} SLEEPSCOREINFO;

//////////////////////////// Fourier Parameters ///////////////////////////

typedef struct tagPOWERBAND
{
   WCHAR szBand[LABELSIZE];            // Name of band (e.g. 'alpha')
   double dLower;                      // Lower limits [Hz]
   double dUpper;                      // Upper limits [Hz]
   COLORREF lColor;                    // Color (in RGB) used to display band
   BYTE RESERVEDBYTES[516];
} POWERBAND;

///////////////////////// General spectrum packet /////////////////////////

typedef struct tagSPECTRUMINFO
{
   WCHAR  szLabel[LABELSIZE];    // Label, e.g. "Left"
   WCHAR  szChannel1[LABELSIZE]; // Name of first EEG channel, e.g. C3-A1
   WCHAR  szChannel2[LABELSIZE]; // Name of second EEG channel, e.g. C4-A2 (Cross spectrum & coherence only)
   double dSamplingRate;         // Set by the Fourier node
   double dResolution;           //           - " -
   double dWss;                  //           - " -
   long   lType;                 // PowerSpectrum, CrossSpectrum or Coherence
   long  lcWelch;                // Number of blocks averaged by the Welch node
   double dX1;
   double dX2;
   double dX3;
   BYTE RESERVEDBYTES[512];
} SPECTRUMINFO;

typedef struct tagARTIFACTINFO
{
   TIMESTAMP timestamp;
   BYTE RESERVEDBYTES[512];
} ARTIFACTINFO;

#endif // __INFOSTRUCTURES_H__
