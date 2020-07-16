
#pragma once

#include "mapptr.h"
#include "sockwnd.h"
#include "winsock.h"
#include <map>

//////////////////////////////////////////////////////////////////////////////

static void _ReportThreadId(char* pc, ...)
{
#ifdef _DEBUG
   char c[256];
   memset(c, 0, 256*sizeof(char));
	va_list argList;
	va_start(argList, pc);
   vsprintf(c, pc, argList);
	va_end(argList);

   SYSTEMTIME t;                                                              
   ::GetLocalTime(&t);                                                        
   long lMilli = (long)(t.wMilliseconds / 10.);                               
   _RPT1(_CRT_WARN,"ThreadID = %d ", ::GetCurrentThreadId());
   _RPT4(_CRT_WARN,"at %.2d:%.2d:%.2d:%.2d - ", t.wHour, t.wMinute, t.wSecond, lMilli);
   _RPT1(_CRT_WARN, "%s\n", c); 
#endif
}

//////////////////////////////////////////////////////////////////////////////

typedef struct _SHARED_THREAD_STUFF
{
   _SHARED_THREAD_STUFF() 
   {
      _ReportThreadId(__FUNCTION__);
      memset(this, 0, sizeof(_SHARED_THREAD_STUFF));
      m_cppSock = new CMapPtrToPtr1;
      m_cppSockDead = new CMapPtrToPtr1;
   }
   ~_SHARED_THREAD_STUFF()
   {
      _ReportThreadId(__FUNCTION__);
      delete m_cppSock;
      m_cppSock = NULL;
      delete m_cppSockDead;
      m_cppSockDead = NULL;
   }
   CMapPtrToPtr1* m_cppSock;
   CMapPtrToPtr1* m_cppSockDead;
   CSocketwnd* m_pWnd;
   volatile long m_ul;
} SHARED_THREAD_STUFF;

//////////////////////////////////////////////////////////////////////////////

class shared_thread_stuff
{
public:
   shared_thread_stuff();
   ~shared_thread_stuff();
   static shared_thread_stuff* Add();
   void Release();

   void lock_map();
   void unlock_map();

   SHARED_THREAD_STUFF* get_data();
   void init_data();
   void free_data();
   void _free();

private:
   static shared_thread_stuff* m_pthis;
   std::map<u_long, SHARED_THREAD_STUFF*> m_map;
	mutable CRITICAL_SECTION m_crit_map;
   static volatile long m_lCount;
};
