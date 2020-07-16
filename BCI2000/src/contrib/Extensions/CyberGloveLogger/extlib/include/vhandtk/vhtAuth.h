#ifndef _VHT_AUTH_H
#define _VHT_AUTH_H

#if defined(BUILD_WIN32_VH_DLL)

#if defined __BUILD_VH_DEVICE_DLL__
#define VH_DEVICE_DLL_EXPORT __declspec(dllexport)
#else
#define VH_DEVICE_DLL_EXPORT __declspec(dllimport)
#endif

#else

#define VH_DEVICE_DLL_EXPORT

#endif

#if defined( USE_CRYPKEY )
//#pragma comment (lib, "crp32d60.lib" )
#endif

class VH_DEVICE_DLL_EXPORT vhtAuth
{
	public:
		// Can throw vhtBadLogic exception if product is not authorized
		//static bool Authorize( void );

		static bool Authorize( unsigned long _callCode );

		static void Terminate( void );

};

#endif
