/********************************************************************
 FILE: $Id: vhtThread.h,v 1.4 2002/08/28 23:16:25 jsilver Exp $
 AUTHOR: Ron Carmel
 DATE: Aug 1998

 DESCRIPTION:
  - a set of functions that abstracts threading from the programmer.
  - meant for use on Win32 platforms and POSIX compliant platforms

 HISTORY:
  -
           -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/
/* 
NOTE: TODO should be vhtThread class, not typedef CU031999

 */
#ifndef __VHTTHREAD_H
#define __VHTTHREAD_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

#ifdef _WIN32
    /* define how many millisecs to wait between thread termination checks */
    #define THREAD_TERMINATION_CHECK_DELAY 1000

// JH Edit 9-4-09
#ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0400
#endif
    #include <windows.h>

#else
    #include <pthread.h>
#endif

#ifdef _WIN32
    typedef struct { 
        DWORD threadId; /* handle of system thread_id */ 
        HANDLE threadHandle; /* pseudo handle of thread */ 
    } vhtThread;
#else
    typedef pthread_t vhtThread;
#endif


VH_CORE_DLL_EXPORT void vhtThreadExit(void *retval);
VH_CORE_DLL_EXPORT void *vhtThreadJoin(vhtThread &thread);
VH_CORE_DLL_EXPORT bool vhtThreadCreate(vhtThread *thread, unsigned int (*threadFunc)(void*), void *param = NULL, unsigned int stackSize = 0);

#endif



