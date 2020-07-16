
#ifndef __TOBII_SDK_CORE_H__
#define __TOBII_SDK_CORE_H__

#include <stddef.h>

#define __TOBII_SDK_INCLUDING__
#include "defines.h"

#ifdef _MSC_VER
#	if !defined(TOBIISDK_EXPORTS)
#		include "detail/stdint.h"
#	else
#		include <boost/cstdint.hpp>
		using boost::int32_t;
		using boost::uint32_t;
		using boost::uint8_t;
		using boost::int64_t;
		using boost::uint64_t;
#	endif
#else
#	include <stdint.h>
#endif

// should come first
#include "core/error.h"

#include "core/blob.h"
#include "core/mainloop.h"
#include "core/discovery.h"
#include "core/vector.h"
#include "core/paramstack.h"
#include "core/messagepasser.h"
#include "core/clock.h"
#include "core/syncmanager.h"
#include "core/upgrade.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Initializes the SDK core
 *
 * Can be called any number of times.
 */
TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_init (
		tobii_sdk_error_t** error
	);

/**
 * @brief Frees a string which was previously allocated and returned by the SDK
 */
TOBIISDK_API void TOBIISDK_CALL tobii_sdk_free_string (const char* str);

TOBIISDK_API tobii_sdk_blob_t* TOBIISDK_CALL tobii_sdk_hmac_perform (uint32_t algorithm,
		const char* key,
		uint32_t key_length,
		const char* input,
		uint32_t input_length,
		tobii_sdk_error_t** error);


#ifdef __cplusplus
}
#endif

#undef __TOBII_SDK_INCLUDING__

#endif // __TOBII_SDK_CORE_H__
