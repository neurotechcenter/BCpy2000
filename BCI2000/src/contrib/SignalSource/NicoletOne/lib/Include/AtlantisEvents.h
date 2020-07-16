#ifndef __ATLANTISEVENTS_H__
#define __ATLANTISEVENTS_H__

#include <AtlantisEventStructs.h>

#define INSERTION_EVENT_ID -100

#define INSERT_EVENT_ID -1
#define MODIFY_EVENT_ID -2
#define DELETE_EVENT_ID -3

// Security
//#define EVENT_DELETEABLE   0x00000001
#define EVENT_EDITABLE     0x00000002
#define EVENT_PALETTEABLE  0x00000004
#define EVENT_ALARMABLE    0x00000008

#define EVENT_OPENDURATION LONG_MAX

// {B799F680-72A4-11d3-93D3-00500400C148}
static const GUID EVENTGUID = 
{ 0xb799f680, 0x72a4, 0x11d3, { 0x93, 0xd3, 0x0, 0x50, 0x4, 0x0, 0xc1, 0x48 } };

#endif //__ATLANTISEVENTS_H__
