/*--------------------------------------------------------------------------*/
// fglove.h
//
// 5DT Data Glove driver SDK
//
// Copyright (C) 2000-2011, 5DT <Fifth Dimension Technologies>
// http://www.5dt.com/
/*--------------------------------------------------------------------------*/

#pragma once

#ifdef FGLOVE_EXPORTS
#define FGLOVEAPI extern "C" __declspec( dllexport )
#else
#define FGLOVEAPI extern "C" __declspec( dllimport )
#endif

#define DG14U_R			0x00
#define DG14U_L			0x01
#define DG5U_R			0x10
#define DG5U_L			0x11

enum EfdGloveHand
{
	FD_HAND_LEFT,   // left-handed glove
	FD_HAND_RIGHT   // right-handed glove
};

enum EfdGloveTypes
{
	FD_GLOVENONE,   // no glove
	FD_GLOVE5U,		// DG5 Ultra serial
	FD_GLOVE5UW,	// DG5 Ultra serial, wireless
	FD_GLOVE5U_USB,	// DG5 Ultra USB
	FD_GLOVE7,      // 7-sensor
	FD_GLOVE7W,     // 7-sensor, wireless
	FD_GLOVE16,     // 16-sensor
	FD_GLOVE16W,    // 16-sensor, wireless
	FD_GLOVE14U,	// DG14 Ultra serial
	FD_GLOVE14UW,	// DG14 Ultra serial, wireless
	FD_GLOVE14U_USB	// DG14 Ultra USB
};

enum EfdSensors
{
	FD_THUMBNEAR=0,
	FD_THUMBFAR,
	FD_THUMBINDEX,
	FD_INDEXNEAR,
	FD_INDEXFAR,
	FD_INDEXMIDDLE,
	FD_MIDDLENEAR,
	FD_MIDDLEFAR,
	FD_MIDDLERING,
	FD_RINGNEAR,
	FD_RINGFAR,
	FD_RINGLITTLE,
	FD_LITTLENEAR,
	FD_LITTLEFAR,
	FD_THUMBPALM,
	FD_WRISTBEND,
	FD_PITCH,
	FD_ROLL
};

#ifndef FDT_BUILD
typedef struct
{
	
} fdGlove;
#endif

//FGLOVEAPI fdGlove *fdOpen(char *pPort, bool bOnlyAllowSingleConnection = false);	
FGLOVEAPI fdGlove *fdOpen(char *pPort);	
FGLOVEAPI int   fdClose(fdGlove *pFG);
FGLOVEAPI int   fdGetGloveHand(fdGlove *pFG);
FGLOVEAPI int   fdGetGloveType(fdGlove *pFG);
FGLOVEAPI int   fdGetNumSensors(fdGlove *pFG);
FGLOVEAPI void  fdGetSensorRawAll(fdGlove *pFG, unsigned short *pData);
FGLOVEAPI unsigned short fdGetSensorRaw(fdGlove *pFG, int nSensor);
FGLOVEAPI void  fdSetSensorRawAll(fdGlove *pFG, unsigned short *pData);
FGLOVEAPI void  fdSetSensorRaw(fdGlove *pFG, int nSensor, unsigned short nRaw);
FGLOVEAPI void  fdGetSensorScaledAll(fdGlove *pFG, float *pData);
FGLOVEAPI float fdGetSensorScaled(fdGlove *pFG, int nSensor);
FGLOVEAPI int   fdGetNumGestures(fdGlove *pFG);
FGLOVEAPI int   fdGetGesture(fdGlove *pFG);
FGLOVEAPI int	fdGetGestureA(fdGlove *pFG);
FGLOVEAPI void  fdGetCalibrationAll(fdGlove *pFG, unsigned short *pUpper, unsigned short *pLower);
FGLOVEAPI void  fdGetCalibration(fdGlove *pFG, int nSensor, unsigned short *pUpper, unsigned short *pLower);
FGLOVEAPI void  fdSetCalibrationAll(fdGlove *pFG, unsigned short *pUpper, unsigned short *pLower);
FGLOVEAPI void  fdSetCalibration(fdGlove *pFG, int nSensor, unsigned short nUpper, unsigned short nLower);
FGLOVEAPI void  fdResetCalibration(fdGlove *pFG, int nSensor);
FGLOVEAPI void  fdResetCalibrationAll(fdGlove *pFG);
FGLOVEAPI void  fdGetSensorMaxAll(fdGlove *pFG, float *pMax);
FGLOVEAPI float fdGetSensorMax(fdGlove *pFG, int nSensor);
FGLOVEAPI void  fdSetSensorMaxAll(fdGlove *pFG, float *pMax);
FGLOVEAPI void  fdSetSensorMax(fdGlove *pFG, int nSensor, float fMax);
FGLOVEAPI void  fdGetThresholdAll(fdGlove *pFG, float *pUpper, float *pLower);
FGLOVEAPI void  fdGetThreshold(fdGlove *pFG, int nSensor, float *pUpper, float *pLower);
FGLOVEAPI void  fdSetThresholdAll(fdGlove *pFG, float *pUpper, float *pLower);
FGLOVEAPI void  fdSetThreshold(fdGlove *pFG, int nSensor, float fUpper, float fLower);
FGLOVEAPI void  fdGetGloveInfo(fdGlove *pFG, unsigned char *pData);
FGLOVEAPI void  fdGetDriverInfo(fdGlove *pFG, unsigned char *pData);
FGLOVEAPI void	fdSetCallback(fdGlove *pFG,void *pFunc, void *param);
FGLOVEAPI void	fdRemoveCallback(fdGlove *pFG);
FGLOVEAPI int	fdGetPacketRate(fdGlove *pFG);
FGLOVEAPI bool	fdNewData(fdGlove *pFG);
FGLOVEAPI int	fdGetFWVersionMajor(fdGlove *pFG);
FGLOVEAPI int	fdGetFWVersionMinor(fdGlove *pFG);
FGLOVEAPI bool	fdGetAutoCalibrate(fdGlove *pFG);
FGLOVEAPI bool	fdSetAutoCalibrate(fdGlove *pFG, bool bAutoCalibrate);
FGLOVEAPI bool	fdSaveCalibration(fdGlove *pFG, const char *pFileName);
FGLOVEAPI bool	fdLoadCalibration(fdGlove *pFG, const char *pFileName);
FGLOVEAPI int	fdScanUSB(unsigned short *aPID, int &nNumMax);
//FGLOVEAPI int	fdScanUSB();
FGLOVEAPI bool	fdGetSerialNumber(fdGlove *pFG, char *pData);