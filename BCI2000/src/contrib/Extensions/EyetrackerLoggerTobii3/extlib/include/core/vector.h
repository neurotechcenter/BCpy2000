#ifndef __TOBII_SDK_CORE_VECTOR_H__
#define __TOBII_SDK_CORE_VECTOR_H__

#ifndef __TOBII_SDK_INCLUDING__
#	error "You cannot include vector.h directly."
#endif

#include "paramtype.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct tobii_sdk_vector_t tobii_sdk_vector_t;

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_vector_type_is_compatible (
		tobii_sdk_param_type_t element_type
	);

TOBIISDK_API tobii_sdk_vector_t* TOBIISDK_CALL
tobii_sdk_vector_create (
		uint32_t size,
		tobii_sdk_param_type_t element_type,
		tobii_sdk_error_t** error
	);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_vector_destroy (
		tobii_sdk_vector_t* vector
	) TOBIISDK_NONNULL(1);

TOBIISDK_API tobii_sdk_param_type_t TOBIISDK_CALL
tobii_sdk_vector_get_element_type (
		const tobii_sdk_vector_t* vector,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_vector_get_length (
		const tobii_sdk_vector_t* vector,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API int32_t TOBIISDK_CALL
tobii_sdk_vector_get_int32 (
		const tobii_sdk_vector_t* vector,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_vector_get_uint32 (
		const tobii_sdk_vector_t* vector,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API float TOBIISDK_CALL
tobii_sdk_vector_get_fixed_15x16_as_float32 (
		const tobii_sdk_vector_t* vector,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API double TOBIISDK_CALL
tobii_sdk_vector_get_fixed_22x41_as_float64 (
		const tobii_sdk_vector_t* vector,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API const char* TOBIISDK_CALL
tobii_sdk_vector_get_string (
		const tobii_sdk_vector_t* vector,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_vector_get_string_length (
		const tobii_sdk_vector_t* vector,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_vector_set_int32 (
		tobii_sdk_vector_t* vector,
		uint32_t index,
		int32_t value,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_vector_set_uint32 (
		tobii_sdk_vector_t* vector,
		uint32_t index,
		uint32_t value,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_vector_set_fixed_15x16_as_float32 (
		tobii_sdk_vector_t* vector,
		uint32_t index,
		float value,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_vector_set_fixed_22x41_as_float64 (
		tobii_sdk_vector_t* vector,
		uint32_t index,
		double value,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_vector_set_string (
		tobii_sdk_vector_t* vector,
		uint32_t index,
		const char* string,
		uint32_t length,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);


#ifdef __cplusplus
}
#endif

#endif // __TOBII_SDK_CORE_VECTOR_H__
