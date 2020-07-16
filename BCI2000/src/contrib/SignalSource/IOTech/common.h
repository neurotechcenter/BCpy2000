#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

//include header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//global constants
#define SERVERPORT 2100
#define MAX_FILENAME 255
#define MAX_CHANNEL 64
#define MAX_SAMPLEDATA 100000
#define MAX_MMDATA 100000
#define MAX_CSPPICKS 64

//new type definitions
typedef unsigned char byte;

//global constants
enum BMI_RESULT {
    BMI_CLASSIFIED = 1,
    BMI_OK       =  0,
    BMI_ERROR    = -1,
    BMI_EOF	     = -2,
    BMI_NOTIMP   = -3
};

enum TPROMPT {
    PROMPT_UNKNOWN = 0,
    PROMPT_LEFT = -1,
    PROMPT_RIGHT = 1,
    PROMPT_REST = 2,
	PROMPT_RIGHT_LEG = 3,
	PROMPT_LEFT_LEG = 4
};

enum TCLASSES{
    CLASS_NONE = 0,
    CLASS_LEFT = -1,
    CLASS_RIGHT = 1
};

enum TPROTOCOL {
    PRO_START = 10,
    PRO_PRETRAIN = 11,
    PRO_PRERUN = 12,
    PRO_RUN = 13,
    PRO_CYCLE = 14,
    PRO_TASK = 15,
    PRO_PRELIMINARYTRIAL = 20,
    PRO_TRIAL = 21,
    PRO_REST = 22,
    PRO_END = 99
};

#define OSYSWARNING_ENABLED
#define OSYSPROTOCOLPRINT_ENABLED
//#define OSYSDEBUG_ENABLED
//#define OSYSDEBUGDAQ_ENABLED
//#define OSYSDEBUGACQ

//unconditional system logs
#define OSYSPRINT(...)       printf(__VA_ARGS__)
#define OSYSERR(...)         fprintf(stderr, __VA_ARGS__)

//conditional system logs
#ifdef OSYSDEBUG_ENABLED
  #define OSYSDEBUG(...)     printf(__VA_ARGS__)
#else
  #define OSYSDEBUG(...)
#endif

#ifdef OSYSWARNING_ENABLED
  #define OSYSWARNING(...)   printf(__VA_ARGS__)
#else
  #define OSYSWARNING(...)
#endif

#ifdef OSYSDEBUGDAQ_ENABLED
  #define OSYSDEBUGDAQ(...)   OSYSDEBUG(__VA_ARGS__)
#else
  #define OSYSDEBUGDAQ(...)
#endif

#ifdef OSYSPROTOCOLPRINT_ENABLED
  #define OSYSPROTOCOLPRINT(...)   OSYSPRINT(__VA_ARGS__)
#else
  #define OSYSPROTOCOLPRINT(...)
#endif

#endif // COMMON_H_INCLUDED
