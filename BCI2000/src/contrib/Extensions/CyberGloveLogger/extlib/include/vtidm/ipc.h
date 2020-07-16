/*============================================================================

  $Id: ipc.h,v 1.2 2002/10/11 22:33:17 dgomez Exp $

  HISTORY:

 ============================================================================*/
#ifndef _VT_IPC_H_
#define _VT_IPC_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _WIN32
#include <unistd.h>
#endif

#include "types.h"

#if defined (__sgi)
#define _POSIX_SEM        1
#define _POSIX_SHM        1
#undef _SYSV_SEM
#undef _SYSV_SHM

#elif defined (_LINUX)
#define _SYSV_SHM         1
#undef _POSIX_SHM

#define _POSIX_SEM        1
#undef _SYSV_SEM         
/*  #undef _POSIX_SEM */
/*  #define _SYSV_SEM         1 */

#elif defined (_WIN32)
#define _WIN32_IPC        1

#endif /* End of OS */

/* semaphores */
#if _SYSV_SEM
typedef int semaphore;
#include <sys/sem.h>		/* for sembuf */
/* set sem_flg to SEM_UNDO somehow cause "Return Too Large" error */
static struct sembuf sembuf_inc = {0, 1, 0};/*sem_num,sem_op,sem_flg*/
static struct sembuf sembuf_trywait = {0,-1,IPC_NOWAIT};/*sem_num,sem_op,sem_flg*/
static struct sembuf sembuf_dec = {0, -1, 0}; 

#elif _POSIX_SEM
#include <semaphore.h>
typedef sem_t semaphore;

#elif _WIN32_IPC
#include <windows.h>
typedef HANDLE semaphore;

#endif /* End of Semaphores def */

/* similar to posix unname semaphore */
VT_DLL_PROC vt_error VT_CreateSem(semaphore * sem, int val);

/* for performance reason, VT_PostSem, and VT_WaitSem are defined as macros */
/* acquire the semaphore, deduct the semval by 1 */
/* block if semval < 0, otherwise, return current semval */

#ifndef _DEBUG

#if _SYSV_IPC
#define VT_WaitSem(psem)   ((semop(*(psem), &sembuf_dec, 1) == -1)? \
		       (perror("VT_WaitSem: semop"), \
                       semctl(*(psem), 1 , IPC_RMID, (union semun)0 ),VTERR_GENERIC_ERROR):\
                       VTERR_NO_ERROR)
#define VT_PostSem(psem)    ((semop(*(psem), &sembuf_inc, 1) == -1)? \
		(perror("VT_PostSem: semop"), \
        semctl(*(psem), 1 , IPC_RMID, (union semun) 0 ), VTERR_GENERIC_ERROR):\
        VTERR_NO_ERROR)
    
#elif _POSIX_SEM
#define VT_WaitSem(sem)    ((sem_wait(sem) != -1)?VTERR_NO_ERROR:\
VTERR_GENERIC_ERROR)
#define VT_PostSem(sem)  ((sem_post(sem) == 0)?VTERR_NO_ERROR:\
VTERR_GENERIC_ERROR)

#elif _WIN32_IPC
/* should use the timeout to implment more robust block */
#define VT_WaitSem(sem) ((WaitForSingleObject(sem, INFINITE) == WAIT_OBJECT_0)?\
VTERR_NO_ERROR:VTERR_GENERIC_ERROR)
#define VT_PostSem(sem)  ((ReleaseSemaphore(sem, 1, NULL)==TRUE)?VTERR_NO_ERROR\
VTERR_GENERIC_ERROR)

#endif 

#else /* end of run-time wait/VT_PostSem */
VT_DLL_PROC vt_error VT_WaitSem(semaphore * sem);
VT_DLL_PROC vt_error VT_PostSem(semaphore * sem);
#endif /* end if _DEBUG */

VT_DLL_PROC vt_error VT_TryWaitSem(semaphore * sem);
VT_DLL_PROC int  VT_GetValueSem(semaphore * sem);
VT_DLL_PROC int  VT_SetvalueSem(semaphore * sem);
VT_DLL_PROC void VT_DestroySem(semaphore *sem);

VT_DLL_PROC void * VT_CreateShm(char *name, unsigned int sz);
VT_DLL_PROC void * VT_AttachShm(char *name, unsigned int sz);
VT_DLL_PROC void VT_DetachShm(char *name, char * addr, unsigned int sz);
VT_DLL_PROC void VT_DestroyShm(char *name);

/*
  The next two functions can only operate on shared memory
  with below structure. The functions will verify magic
  fist.
  typedef struct sync_mem {
      long       magic;  0x53594e43 
      semaphore  lock;
      void * data;
  };
*/ 

#define SYNC_MEM_MAGIC 0x53594e43      /* "SYNC" */
#ifndef _SYNC_SHM_WITH_FLOCK
/* should be used as SYNC_MEM(char data[100]); */
#define SYNC_MEM(name, pName, data)   typedef struct { long magic; \
                                            semaphore lock; \
                                            data;} name, *pName;
/* prototype of sync shared memory for used with shm_lock, shm_unlock*/
typedef struct {
	long magic;
	semaphore lock;
	char data[1];		/* just a place holder, should not be used */
} syncmem_t, *syncmem;

VT_DLL_PROC vt_error VT_InitSyncMem(void * memaddr); /* set magic, create semaphore */
VT_DLL_PROC vt_error VT_InitSyncMem(void * memaddr);
#define VT_LockSyncMem(addr) ((((syncmem)addr)->magic == SYNC_MEM_MAGIC)? \
                              (VT_WaitSem(&(((syncmem)addr)->lock))):FALSE)

#define VT_UnLockSyncMem(addr) ((((syncmem)addr)->magic == SYNC_MEM_MAGIC)? \
                              (VT_PostSem(&(((syncmem)addr)->lock))):FALSE)

#else  /* sync with flock */
#define SYNC_MEM(name, pName, data) typedef struct {long magic; \
                                                    data;} name, *pName;
typedef struct {
	long magic;
	char data[1];		/* just a place holder, should not be used */
} syncmem_t, *syncmem;

#define VT_InitSyncMem(memaddr)  (((syncmem)memaddr)->magic = SYNC_MEM_MAGIC)
#define VT_InitSyncMem(emaddr) (TRUE)
#define VT_LockSyncMem(addr) (FALSE)
#define VT_UnLockSyncMem(addr) (FALSE)
#endif

#if _SHM_MSG
/* message implemented as shared memory + sempahores */
/* server recv driver message, (server block on  SVR_RECV*/
/* client will lock CLNT_SEND before activing server through unlock
   SVR_RECV, to gain exclusive access to the message buffer. */
#define MSG_MEM_MAGIC  0x4d53474d /* MSGM */
enum {CLNT_RECV = 0, CLNT_SEND, SVR_RECV, SVR_SEND}; 

#define MSG_MEM(name, pName, data) typedef struct {long magic; \
                                                   semaphore sem[4]; \
                                                   data;} name, *pName;

typedef struct {
    long      magic;		
    semaphore sem[4];
    time_t    cbtime;		/* client block time, {0,0} if unblocked */
    time_t    sbtime;		/* server block time, {0, 0} if unblocked */
    char      data[1];		/* just a place holder, should not be used */
} msgmem_t, *msgmem;

VT_DLL_PROC vt_error VT_InitMsgMem(void * memaddr); /* set magic, create semaphore */
VT_DLL_PROC vt_error VT_InitMsgMem(void * memaddr);
VT_DLL_PROC vt_error VT_ResetMsgMem(void * memaddr); /* reset to release all locks */
VT_DLL_PROC vt_error VT_IsMsgMemTimeout(void *memaddr, double to);


#define VT_ClientBeginSend(addr)  ((((msgmem)addr)->magic == MSG_MEM_MAGIC)? \
                              (VT_WaitSem(&(((msgmem)addr)->sem[CLNT_SEND]))):\
                              FALSE)
#define VT_ClientCallServer(addr)  ((((msgmem)addr)->magic == MSG_MEM_MAGIC)? \
                              (VT_PostSem(&(((msgmem)addr)->sem[SVR_RECV]))):\
                              FALSE)
#define VT_ClientGetReply(addr)  ((((msgmem)addr)->magic == MSG_MEM_MAGIC)? \
                              (VT_WaitSem(&(((msgmem)addr)->sem[CLNT_RECV]))):\
                              FALSE)
#define VT_ClientEndSend(addr)  ((((msgmem)addr)->magic == MSG_MEM_MAGIC)? \
                              (VT_PostSem(&(((msgmem)addr)->sem[CLNT_SEND]))):\
                              FALSE)

#define VT_ServerRecv(addr)  ((((msgmem)addr)->magic == MSG_MEM_MAGIC)? \
                              (VT_WaitSem(&(((msgmem)addr)->sem[SVR_RECV]))):\
                              FALSE)
#define VT_ServerReply(addr)  ((((msgmem)addr)->magic == MSG_MEM_MAGIC)? \
                              (VT_PostSem(&(((msgmem)addr)->sem[CLNT_RECV]))):\
                              FALSE)


#endif
#ifdef __cplusplus
}
#endif

#endif
