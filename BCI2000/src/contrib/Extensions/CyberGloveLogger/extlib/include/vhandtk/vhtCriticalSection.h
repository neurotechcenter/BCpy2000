/********************************************************************
 FILE: $Id: vhtCriticalSection.h,v 1.6 2002/08/28 23:16:10 jsilver Exp $
 AUTHOR: Ron Carmel
 DATE: Aug 1998

 DESCRIPTION:
  - a platform indepentant (POSIX/Win32) implementation of a 
	critical section locking class.  to use, either derive a class from 
	this one and use the lock and unlock methods to lock and
	unlock the class instance, or instantiate a vhtCriticalSection
	and call it's lock and unlock methods lock and unlock the
	critical section.
  - NOTE: in Win32 critical sections are handled faster than mutexes so 
	the class implements a critical section using the CRITICAL_SECTION
	structure and functions in Win32, and a POSIX pthread_mutex_t and 
	associated functions in POSIX compliant environments
  - this implementation of critical section DOES NOT SUPPORT cross-
	process access (only cross-thread)

 HISTORY:
  -
		   -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1998 --
********************************************************************/

#ifndef __VHTCRITICALSECTION_H
#define __VHTCRITICALSECTION_H

#ifdef _WIN32
	/* TryEnterCriticalSection is only availalble on NT4 or newer.
	   the following definition will allow making an exectuable which 
	   checks (in runtime) the version of the OS, and if it is not
	   NT4 or newer, deal with it properly (see the tryLock function) */
	//#define _WIN32_WINNT 0x0400

	#include <windows.h>
	#include <winbase.h>
#else
	#include <pthread.h>
#endif

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_CORE_DLL__
#define VH_CORE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_CORE_DLL_EXPORT __declspec(dllimport)
#endif

#else 

#define VH_CORE_DLL_EXPORT

#endif

//: Platform independent locking mechanism.
// Provides an abstaction of the locking mechanism for applications.
//!vhtmodule: Core
class VH_CORE_DLL_EXPORT vhtCriticalSection
{
public:
	//: Construct a lock.
	vhtCriticalSection();
	//: Destruct.
	~vhtCriticalSection();

	//: Try to lock.
	// Return value indicates success.
	bool tryLock();
	//: Lock.
	// Application must assume lock has succeded.
	void	lock();
	//: Unlock
	// Free the current lock.
	void unlock();

private:

#ifdef _WIN32
	CRITICAL_SECTION	m_criticalSection;
#else
	pthread_mutexattr_t m_mutexAttr;
	pthread_mutex_t	 m_mutex;
#endif

};

#endif



