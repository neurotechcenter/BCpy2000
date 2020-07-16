#ifndef __VHT_KEY_H__
#define __VHT_KEY_H__

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_DEVICE_DLL__
#define VH_DEVICE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_DEVICE_DLL_EXPORT __declspec(dllimport)
#endif

#else

#define VH_DEVICE_DLL_EXPORT

#endif

VH_DEVICE_DLL_EXPORT unsigned long vhtInitGetKey( void );

#endif