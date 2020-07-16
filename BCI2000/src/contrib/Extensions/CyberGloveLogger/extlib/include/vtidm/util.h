/*============================================================================

  $Id: util.h,v 1.2 2002/10/11 22:33:16 dgomez Exp $

  HISTORY:

 ============================================================================*/

#ifndef _VT_UTIL_H_
#define _VT_UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "error.h"

#ifdef _WIN32
#include <windows.h>
#ifndef strcasecmp
#define strcasecmp  _stricmp
#endif
#endif

#define MAX_CONFIG_LINE_SZ   256

/* structure to parse command line options or function args*/
typedef struct
{
    int index;			/* must be a positive value */
    char *string;		/* if string=NULL, end of the indexed string */
}  vt_indexed_string_t, * vt_indexed_string;

#define VT_INVALID_STRING_INDEX  -1
#define VT_INDEXED_STRING_END    {-1, NULL}

enum {
    VT_KV_NONE = -1, 
    VT_KV_CHAR, 
    VT_KV_INT, 
    VT_KV_STRING, 
    VT_KV_BOOL,
    VT_KV_DOUBLE,
};

#define VT_DEFAULT_STRING_SZ  256

typedef struct  {
    int type;
    char * key;			/* key name */
    void * ret;			/* points to return variable */
    const char * def;		/* default value string */
} vt_key_value_t, * vt_key_value;
#define VT_KEY_VALUE_END {VT_KV_NONE, NULL, NULL, NULL}

/* a config line looks like "server=reflex.virtex.com:port=12345:proto=tcp" */
/* in which server is headkey */
typedef struct {
    char *headkey;
    char *ret;
    const char *def;
} vt_config_line_t, * vt_config_line;
#define VT_CONFIG_LINE_END {NULL, NULL, NULL}

/* look up the table to find the index of the string matching str */
VT_DLL_PROC int VT_GetIndexedStringTableSize(vt_indexed_string table);
VT_DLL_PROC int  VT_GetStringIndex(char * str, vt_indexed_string table);
VT_DLL_PROC char *  VT_GetStringFromIndex(int i, vt_indexed_string table);

/* time related */
VT_DLL_PROC double  VT_TimeElapsed(int timer);
VT_DLL_PROC double  VT_GetCurrentTime();	 /* return secs since 1970.1.1 */
#ifdef _WIN32
#define VT_Sleep         Sleep
#else   
#define VT_Sleep(msec)   usleep(msec * 1000)
#endif /* _WIN32 */

/* string token related */
VT_DLL_PROC char *  VT_GetStringToken(char * str, char * tok, char * ret,
				      int  retsz);

/* string has format like "key1=100:key2=string:key3=StRing" */
VT_DLL_PROC char *  VT_GetNextKV(char *str, char ** key, char ** value);

VT_DLL_PROC vt_bool  VT_GetKeyValueI(char * str, char *key, int * ret);
VT_DLL_PROC vt_bool  VT_GetKeyValueS(char * str, char *key, char * ret, 
				     int max);
VT_DLL_PROC vt_bool  VT_GetKeyValueF(char * str, char * key, double * ret);
VT_DLL_PROC vt_bool  VT_GetKeyValue(char *str, vt_key_value kv, vt_bool bDef);

#define VT_SetDefaultKeyValue(kvs) VT_GetKeyValue("", kvs, TRUE)

VT_DLL_PROC vt_bool  VT_GetConfigLine(char *str, vt_config_line cl);

/* yes or true = TRUE, else FALSE */
#define VT_StringToBool(str) \
( strcasecmp(str, "true") == 0 || \
  strcasecmp(str, "on") == 0 || \
  strcasecmp(str, "yes") == 0)

#ifdef __sgi            /* big endian */
enum double_byte_order {DOUBLE_BYTE_7, DOUBLE_BYTE_6, DOUBLE_BYTE_5,
			DOUBLE_BYTE_4, DOUBLE_BYTE_3, DOUBLE_BYTE_2,
			DOUBLE_BYTE_1, DOUBLE_BYTE_0};
enum float_byte_order { FLOAT_BYTE_3, FLOAT_BYTE_2, FLOAT_BYTE_1, 
			FLOAT_BYTE_0};
enum long_byte_order { LONG_BYTE_3, LONG_BYTE_2, LONG_BYTE_1, LONG_BYTE_0 };
enum short_byte_order { SHORT_BYTE_1, SHORT_BYTE_0 };
#else                          /* little endian */
enum double_byte_order {DOUBLE_BYTE_0, DOUBLE_BYTE_1, DOUBLE_BYTE_2,
			DOUBLE_BYTE_3, DOUBLE_BYTE_4, DOUBLE_BYTE_5,
			DOUBLE_BYTE_6, DOUBLE_BYTE_7};
enum float_byte_order { FLOAT_BYTE_0, FLOAT_BYTE_1,  FLOAT_BYTE_2, 
			FLOAT_BYTE_3};
enum long_byte_order { LONG_BYTE_0, LONG_BYTE_1, LONG_BYTE_2, LONG_BYTE_3 };
enum short_byte_order { SHORT_BYTE_0, SHORT_BYTE_1 };
#endif

/* just to be consistent */
#define VT_PutChar(strm, c)  *((char *) (strm)) = c
#define VT_GetChar(strm) *((char *)(strm))
#define VT_PutChar2(strm, i, c) *((char *)((strm) + i)) = c
#define VT_GetChar2(strm, i)  *((char *)((strm) + i))

/* net order is big edian */
/* !!! all the following macros have side effects, try to avoid using
   uniary operator in the arguments, e.g seq++ */
#define VT_PutShort(strm, n) *((short*)strm) = htons(n)  //\
 //((vt_uchar *)(strm))[0] = (vt_uchar)(((vt_ushort)(n)) >> 8 & 0x00FFU); \
 //((vt_uchar *)(strm))[1] = (vt_uchar)(((vt_ushort)(n)) & 0x00FFU)

#define VT_GetShort(strm) ntohs(*((short*)strm)) //\
// (((vt_ushort)((vt_uchar *)(strm))[0]) << 8 | \
// (vt_ushort)((vt_uchar *)(strm))[1])

#define VT_PutShort2(strm, i, n)  VT_PutShort((strm) + ((i) * sizeof(short)), n)
#define VT_GetShort2(strm, i)     VT_GetShort((strm) + ((i) * sizeof(short)))

#define VT_PutLong(strm, n) *((unsigned int*)strm) = htonl(n)//\
//	((vt_uchar *)(strm))[0] = (vt_uchar)(((vt_ulong)(n)) >> 24 & 0x000000FFU); \
//	((vt_uchar *)(strm))[1] = (vt_uchar)(((vt_ulong)(n)) >> 16 & 0x000000FFU); \
//	((vt_uchar *)(strm))[2] = (vt_uchar)(((vt_ulong)(n)) >> 8  & 0x000000FFU); \
//	((vt_uchar *)(strm))[3] = (vt_uchar)(((vt_ulong)(n))       & 0x000000FFU);

#define VT_GetLong(strm) ntohl(*((unsigned int*)strm))//\
//	(((vt_ulong)((vt_uchar *)(strm))[0]) << 24 | \
//	((vt_ulong)((vt_uchar *)(strm))[1]) << 16 | \
//	((vt_ulong)((vt_uchar *)(strm))[2]) << 8  | \
//	((vt_ulong)((vt_uchar *)(strm))[3]))

#define VT_PutLong2(strm, i, n)  VT_PutLong(((strm) + (i) * sizeof(long)), n)
#define VT_GetLong2(strm, i)     VT_GetLong(((strm) + (i) * sizeof(long)))
VT_DLL_PROC void VT_PutFloat(char * strm, float d);
VT_DLL_PROC float VT_GetFloat(char * strm) ;

#define VT_PutFloat2(strm, i, d) VT_PutFloat((strm) + ((i) * sizeof(float)), d)
#define VT_GetFloat2(strm, i)    VT_GetFloat((strm) + ((i) * sizeof(float)))

VT_DLL_PROC void VT_PutDouble(char * strm, double d);
VT_DLL_PROC double VT_GetDouble(char * strm) ;

#define VT_PutDouble2(strm, i, d) VT_PutDouble((strm) + ((i) * sizeof(double)), d)
#define VT_GetDouble2(strm, i)    VT_GetDouble((strm) + ((i) * sizeof(double)))

VT_DLL_PROC char * lrtrim(char * str) ;

#ifdef __cplusplus
}
#endif

#endif




