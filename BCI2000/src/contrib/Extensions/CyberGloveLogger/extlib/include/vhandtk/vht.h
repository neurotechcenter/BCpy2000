/********************************************************************
 FILE: $Id: vht.h,v 1.5 1999/08/19 18:18:20 ullrich Exp $
 AUTHOR: Ron Carmel
 DATE: June 1998

 DESCRIPTION:

 HISTORY:

           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/
#ifndef __VHT_H
#define __VHT_H

#define VHTMATRIX C_matrix<double>
#define VHTVECTOR Vector<double>

#include <assert.h>

#ifndef _WIN32
#include <math.h>
#endif

#ifndef MAXFLOAT
    #define MAXFLOAT (float)(1e+10)
#endif

#if !defined( _LINUX )

#if !defined( M_PI )
    #define M_PI   ((double)3.1415926535897932384626433832795028841972)
#endif

#if !defined( M_PI_4 )
    #define M_PI_4 ((double)3.1415926535897932384626433832795028841972/(double)4) 
#endif

#if !defined( M_PI_2 )
    #define M_PI_2 ((double)3.1415926535897932384626433832795028841972/(double)2)
#endif

#if !defined( M_E )
    #define M_E    ((double)2.7182818284590452354)
#endif

#if !defined( M_2PI )
    #define M_2PI  ((double)3.1415926535897932384626433832795028841972*(double)2) 
#endif

#if !defined( M_4PI )
    #define M_4PI  ((double)3.1415926535897932384626433832795028841972*(double)4) 
#endif

#if !defined( M_2_PI )
    #define M_2_PI  ((double)2/(double)3.1415926535897932384626433832795028841972) 
#endif

#if !defined( M_4_PI )
    #define M_4_PI  ((double)4/(double)3.1415926535897932384626433832795028841972) 
#endif

#endif
/*-------------------------------------------------------------------
  - the following structures and functions are utilities for writing
  and reading basic data types to and from byte arrays
-------------------------------------------------------------------*/
inline int      vhtWriteInt(    char *buf, int i);
inline int      vhtWriteFloat(  char *buf, float f);
inline int      vhtWriteDouble( char *buf, double d);
inline int      vhtReadInt(     const char *buf);
inline float    vhtReadFloat(   const char *buf);
inline double   vhtReadDouble(  const char *buf);

union vht4Bytes{
    char c[4];
    int i;
    float f;
};

union vht8Bytes{
    char c[8];
    double d;
};

/*-------------------------------------------------------------------
<def>
 ARGUMENTS:
  -
 RETURNS:
  - void
 NOTES:
  -
</def>
-------------------------------------------------------------------*/
inline int vhtWriteInt(char *buf, int i){
    assert(sizeof(int)==4);
    vht4Bytes b;
    b.i = i;

#ifdef _WIN32
    buf[0] = b.c[3];
    buf[1] = b.c[2];
    buf[2] = b.c[1];
    buf[3] = b.c[0];
#else
    buf[0] = b.c[0];
    buf[1] = b.c[1];
    buf[2] = b.c[2];
    buf[3] = b.c[3];
#endif
    return 4;
}

/*-------------------------------------------------------------------
<def>
 ARGUMENTS:
  -
 RETURNS:
  - void
 NOTES:
  -
</def>
-------------------------------------------------------------------*/
inline int vhtWriteFloat(char *buf, float f){
    assert(sizeof(float)==4);
    vht4Bytes b;
    b.f = f;

#ifdef _WIN32
    buf[0] = b.c[3];
    buf[1] = b.c[2];
    buf[2] = b.c[1];
    buf[3] = b.c[0];
#else
    buf[0] = b.c[0];
    buf[1] = b.c[1];
    buf[2] = b.c[2];
    buf[3] = b.c[3];
#endif
    return 4;
}

/*-------------------------------------------------------------------
<def>
 ARGUMENTS:
  -
 RETURNS:
  - void
 NOTES:
  -
</def>
-------------------------------------------------------------------*/
inline int vhtWriteDouble(char *buf, double d){
    assert(sizeof(double)==8);
    vht8Bytes b;
    b.d = d;

#ifdef _WIN32
    buf[0] = b.c[7];
    buf[1] = b.c[6];
    buf[2] = b.c[5];
    buf[3] = b.c[4];
    buf[4] = b.c[3];
    buf[5] = b.c[2];
    buf[6] = b.c[1];
    buf[7] = b.c[0];
#else
    buf[0] = b.c[0];
    buf[1] = b.c[1];
    buf[2] = b.c[2];
    buf[3] = b.c[3];
    buf[4] = b.c[4];
    buf[5] = b.c[5];
    buf[6] = b.c[6];
    buf[7] = b.c[7];
#endif

    return 8;
}

/*-------------------------------------------------------------------
<def>
 ARGUMENTS:
  -
 RETURNS:
  - the integer read from buf
 NOTES:
  -
</def>
-------------------------------------------------------------------*/
inline int vhtReadInt(const char *buf){
    assert(sizeof(int)==4);
    vht4Bytes b;

#ifdef _WIN32
    b.c[0] = buf[3];
    b.c[1] = buf[2];
    b.c[2] = buf[1];
    b.c[3] = buf[0];
#else
    b.c[0] = buf[0];
    b.c[1] = buf[1];
    b.c[2] = buf[2];
    b.c[3] = buf[3];
#endif

    return b.i;
}

/*-------------------------------------------------------------------
<def>
 ARGUMENTS:
  -
 RETURNS:
  - the float read from buf
 NOTES:
  -
</def>
-------------------------------------------------------------------*/
inline float vhtReadFloat(const char *buf){
    assert(sizeof(float)==4);
    vht4Bytes b;

#ifdef _WIN32
    b.c[0] = buf[3];
    b.c[1] = buf[2];
    b.c[2] = buf[1];
    b.c[3] = buf[0];
#else
    b.c[0] = buf[0];
    b.c[1] = buf[1];
    b.c[2] = buf[2];
    b.c[3] = buf[3];
#endif

    return b.f;
}

/*-------------------------------------------------------------------
<def>
 ARGUMENTS:
  -
 RETURNS:
  - the double precision float read from buf
 NOTES:
  -
</def>
-------------------------------------------------------------------*/
inline double vhtReadDouble(const char *buf){
    assert(sizeof(double)==8);
    vht8Bytes b;

#ifdef _WIN32
    b.c[0] = buf[7];
    b.c[1] = buf[6];
    b.c[2] = buf[5];
    b.c[3] = buf[4];
    b.c[4] = buf[3];
    b.c[5] = buf[2];
    b.c[6] = buf[1];
    b.c[7] = buf[0];
#else
    b.c[0] = buf[0];
    b.c[1] = buf[1];
    b.c[2] = buf[2];
    b.c[3] = buf[3];
    b.c[4] = buf[4];
    b.c[5] = buf[5];
    b.c[6] = buf[6];
    b.c[7] = buf[7];
#endif

    return b.d;
}

#endif



