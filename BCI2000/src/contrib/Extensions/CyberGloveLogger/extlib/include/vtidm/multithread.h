/*============================================================================

  $Id: multithread.h,v 1.3 2002/10/11 22:33:16 dgomez Exp $

  HISTORY:

 ============================================================================*/

#ifndef _VT_MULTI_THREAD_H_
#define _VT_MULTI_THREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "error.h"

#ifndef INFINITE
#define INFINITE            0xFFFFFFFF  
#endif

#if defined (__sgi) || defined (_LINUX)
#define _USE_POSIX_THREAD    
#endif

#ifdef _USE_POSIX_THREAD
#include <pthread.h>
typedef  pthread_t       * vt_thread;
typedef  pthread_mutex_t * vt_mutex;
#else  /* win32 */
typedef HANDLE vt_thread;
typedef HANDLE vt_mutex;
#endif

typedef void (*VT_THREAD_WORKER_PROC)(void * arg);

VT_DLL_PROC vt_thread VT_CreateThread(VT_THREAD_WORKER_PROC thread_fn,
					 void *thread_arg);
VT_DLL_PROC vt_error  VT_KillThread(vt_thread thread_id);
VT_DLL_PROC vt_error  VT_AwaitThread(vt_thread thread_id, vt_uint msec_to_wait);

VT_DLL_PROC void  VT_ExitThread(int status);
VT_DLL_PROC void  VT_FreeThread(vt_thread thread_id);
VT_DLL_PROC void  VT_ThreadBlockSignal(int signal);

VT_DLL_PROC vt_mutex  VT_CreateMutex(char *name);
VT_DLL_PROC vt_mutex  VT_OpenMutex(char *name);
VT_DLL_PROC vt_error  VT_AcquireMutex(vt_mutex mutex_id);
VT_DLL_PROC vt_error  VT_ReleaseMutex(vt_mutex mutex_id);
VT_DLL_PROC vt_error  VT_DestroyMutex(vt_mutex mutex_id);

#ifdef __cplusplus
}
#endif

#endif
