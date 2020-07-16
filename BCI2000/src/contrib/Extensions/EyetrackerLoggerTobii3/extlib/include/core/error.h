
#ifndef __TOBII_SDK_CORE_ERROR_H__
#define __TOBII_SDK_CORE_ERROR_H__

#ifndef __TOBII_SDK_INCLUDING__
#	error "You cannot include 'error.h' directly."
#endif

#include "errorcodes.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct tobii_sdk_error_t tobii_sdk_error_t;

typedef uint32_t tobii_sdk_error_code_t;

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_error_destroy (
		tobii_sdk_error_t* error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API tobii_sdk_error_t* TOBIISDK_CALL
tobii_sdk_error_clone (
		const tobii_sdk_error_t* error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API const char* TOBIISDK_CALL
tobii_sdk_error_get_message (
		const tobii_sdk_error_t* error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API tobii_sdk_error_code_t TOBIISDK_CALL
tobii_sdk_error_get_code (
		const tobii_sdk_error_t* error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API const char* TOBIISDK_CALL
tobii_sdk_error_to_string (
		const tobii_sdk_error_t* error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_error_is_failure (
		const tobii_sdk_error_t* error
	);

TOBIISDK_API const char* TOBIISDK_CALL
tobii_sdk_error_convert_code_to_string (
		tobii_sdk_error_code_t code
	);

TOBIISDK_API tobii_sdk_error_t* TOBIISDK_CALL
tobii_sdk_error_create_generic (
		tobii_sdk_error_code_t code,
		const char* message
	);

#ifdef __cplusplus
}
#endif

#endif // __TOBII_SDK_CORE_ERROR_H__
