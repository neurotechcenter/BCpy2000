
#ifndef __TOBII_SDK_CORE_PARAMSTACK_H__
#define __TOBII_SDK_CORE_PARAMSTACK_H__

#ifndef __TOBII_SDK_INCLUDING__
#	error "You cannot include paramstack.h directly."
#endif

#include "paramtype.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * tobii_sdk_param_stack_t
 */

typedef struct tobii_sdk_param_stack_t tobii_sdk_param_stack_t;

/*
 * param stack
 */

TOBIISDK_API tobii_sdk_param_stack_t* TOBIISDK_CALL
tobii_sdk_param_stack_create (
		tobii_sdk_error_t** error
	);

/* used for testing, creates a stack with the following contents:
 *
 *		vec<int32>		-1, -2, -3
 *		vec<uint32>		1, 2, 3
 *		vec<fixed15x16>	-1.1, -2.2, -3.3
 *		vec<fixed22x41>	-1.1, -2.2, -3.3
 *		vec<string>		"Hello 1", "Hello 2", "Hello 3"
 *		node_prolog		0x04020001
 *		int32			-100
 * 		uint32			100
 *		int64			-100
 * 		uint64			100
 * 		float32			-100.0f
 * 		float64			-100.0
 * 		fixed15x16  	-100.0f
 * 		fixed22x41  	-100.0
 * 		string			"Hello ParamStack"
 * 		blob			"Hello Blob" (without the terminating zero)
 */
TOBIISDK_API tobii_sdk_param_stack_t* TOBIISDK_CALL
tobii_sdk_param_stack_create_test_stack (
		tobii_sdk_error_t** error
	);

TOBIISDK_API tobii_sdk_param_stack_t* TOBIISDK_CALL
tobii_sdk_param_stack_clone (
		const tobii_sdk_param_stack_t* param,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_param_stack_destroy (
		tobii_sdk_param_stack_t* param
	) TOBIISDK_NONNULL(1);

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_param_stack_get_size (
		const tobii_sdk_param_stack_t* param,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API tobii_sdk_param_type_t TOBIISDK_CALL
tobii_sdk_param_stack_get_type (
		const tobii_sdk_param_stack_t* param,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_param_stack_append (
		tobii_sdk_param_stack_t* param_stack,
		const tobii_sdk_param_stack_t* append,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API int32_t TOBIISDK_CALL
tobii_sdk_param_stack_get_int32 (
		const tobii_sdk_param_stack_t* params,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_param_stack_get_uint32 (
		const tobii_sdk_param_stack_t* params,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API int64_t TOBIISDK_CALL
tobii_sdk_param_stack_get_int64 (
		const tobii_sdk_param_stack_t* params,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API uint64_t TOBIISDK_CALL
tobii_sdk_param_stack_get_uint64 (
		const tobii_sdk_param_stack_t* params,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API float TOBIISDK_CALL
tobii_sdk_param_stack_get_float32 (
		const tobii_sdk_param_stack_t* params,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API double TOBIISDK_CALL
tobii_sdk_param_stack_get_float64 (
		const tobii_sdk_param_stack_t* params,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API float TOBIISDK_CALL
tobii_sdk_param_stack_get_fixed_15x16_as_float32 (
		const tobii_sdk_param_stack_t* params,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API double TOBIISDK_CALL
tobii_sdk_param_stack_get_fixed_22x41_as_float64 (
		const tobii_sdk_param_stack_t* params,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_param_stack_get_node_prolog (
		const tobii_sdk_param_stack_t* params,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API const char* TOBIISDK_CALL
tobii_sdk_param_stack_get_string (
		const tobii_sdk_param_stack_t* params,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_param_stack_get_string_length (
		const tobii_sdk_param_stack_t* params,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API const unsigned char* TOBIISDK_CALL
tobii_sdk_param_stack_get_blob (
		const tobii_sdk_param_stack_t* params,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_param_stack_get_blob_length (
		const tobii_sdk_param_stack_t* params,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API tobii_sdk_vector_t* TOBIISDK_CALL
tobii_sdk_param_stack_get_vector (
		const tobii_sdk_param_stack_t* params,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_param_stack_push_int32 (
		tobii_sdk_param_stack_t* params,
		int32_t value,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_param_stack_push_uint32 (
		tobii_sdk_param_stack_t* params,
		uint32_t value,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_param_stack_push_int64 (
		tobii_sdk_param_stack_t* params,
		int64_t value,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_param_stack_push_uint64 (
		tobii_sdk_param_stack_t* params,
		uint64_t value,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_param_stack_push_string (
		tobii_sdk_param_stack_t* params,
		const char* value,
		const uint32_t length,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_param_stack_push_float32_as_fixed_15x16 (
		tobii_sdk_param_stack_t* params,
		float value,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_param_stack_push_float64_as_fixed_22x41 (
		tobii_sdk_param_stack_t* params,
		double value,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_param_stack_push_node_prolog (
		tobii_sdk_param_stack_t* params,
		uint32_t value,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_param_stack_push_float32 (
		tobii_sdk_param_stack_t* params,
		float value,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_param_stack_push_float64 (
		tobii_sdk_param_stack_t* params,
		double value,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_param_stack_push_blob (
		tobii_sdk_param_stack_t* params,
		const unsigned char* blob,
		const uint32_t length,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1, 2);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_param_stack_push_vector (
		tobii_sdk_param_stack_t* params,
		const tobii_sdk_vector_t* vector,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1, 2);

#ifdef __cplusplus
}
#endif


#endif // __TOBII_SDK_CORE_PARAMSTACK_H__
