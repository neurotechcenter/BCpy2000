#ifndef VHT_MATH_H
#define VHT_MATH_H
/********************************************************************
 FILE: $Id: vhtMath.h,v 1.5 1999/09/08 19:46:13 hugo Exp $
 AUTHOR: Chris Ullrich.
 DATE: July 1999.

   DESCRIPTION:
    -
 HISTORY:
  -
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998, 1999 --
********************************************************************/
/*
#if defined(_WIN32)
#include <cmath>
#else
#include <math.h>
#endif
*/


typedef enum { VHT_POSTMULT, VHT_PREMULT } vhtRotationOrder;
typedef enum { VHT_GLOBALFRAME=0, VHT_LOCALFRAME=1 } vhtRotationFrame;

typedef enum { VHT_X = 0, VHT_Y = 1, VHT_Z = 2 } vhtAxes;
typedef enum { VHT_U = 0, VHT_V = 1, VHT_W = 2, VHT_P = 3 } vhtLocalAxes;



//#define MAX(x,y) ((x)>(y))?(x):(y)
#if !defined(_WIN32)
#include <sys/param.h>
#endif


#endif		/* VHT_MATH_H */
