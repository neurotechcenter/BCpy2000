
#ifndef __TOBII_SDK_DEFINES_H__
#define __TOBII_SDK_DEFINES_H__

#ifndef __TOBII_SDK_INCLUDING__
#	error "You cannot include 'defines.h' directly."
#endif

#ifdef TOBIISDK_EXPORTS
#	ifdef _MSC_VER
#		define TOBIISDK_API __declspec(dllexport)
#		define TOBIISDK_CALL __cdecl
#		define TOBIISDK_NONNULL(...)
#	else
#		define TOBIISDK_API __attribute__ ((visibility("default")))
#		define TOBIISDK_CALL
#		define TOBIISDK_NONNULL(...) __attribute__((nonnull (__VA_ARGS__)))
#	endif
#else
#	ifdef TOBIISDK_UNIT_TESTS
#		define TOBIISDK_API
#		define TOBIISDK_CALL
#		define TOBIISDK_NONNULL(...)
#	else
#		ifdef _MSC_VER
#			define TOBIISDK_API __declspec(dllimport)
#			define TOBIISDK_CALL __cdecl
#			define TOBIISDK_NONNULL(...)
#		else
#			define TOBIISDK_API
#			define TOBIISDK_CALL
#			define TOBIISDK_NONNULL(...) __attribute__((nonnull (__VA_ARGS__)))
#		endif
#	endif
#endif

#endif // __TOBII_SDK_DEFINES_H__
