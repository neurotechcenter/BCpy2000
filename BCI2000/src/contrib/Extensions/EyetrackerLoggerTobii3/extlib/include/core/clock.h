#ifndef __TOBII_SDK_CORE_CLOCK_H__
#define __TOBII_SDK_CORE_CLOCK_H__

#ifndef __TOBII_SDK_INCLUDING__
#	error "You cannot include 'clock.h' directly."
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct tobii_sdk_clock_t tobii_sdk_clock_t;

typedef int64_t (TOBIISDK_CALL *tobii_sdk_clock_get_time_fn_t) (
		void* instance_data,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

typedef int64_t (TOBIISDK_CALL *tobii_sdk_clock_get_resolution_fn_t) (
		void* instance_data,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

typedef void (TOBIISDK_CALL *tobii_sdk_clock_destroy_fn_t) (
		void* instance_data
	) TOBIISDK_NONNULL(1);


TOBIISDK_API tobii_sdk_clock_t* TOBIISDK_CALL
tobii_sdk_clock_get_default (
		tobii_sdk_error_t** error
	);

TOBIISDK_API tobii_sdk_clock_t* TOBIISDK_CALL
tobii_sdk_clock_create_user_defined (
		tobii_sdk_clock_get_time_fn_t time_func,
		tobii_sdk_clock_get_resolution_fn_t res_func,
		tobii_sdk_clock_destroy_fn_t destructor,
		void* instance_data,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1, 2, 3);


TOBIISDK_API int64_t TOBIISDK_CALL
tobii_sdk_clock_get_time(
		tobii_sdk_clock_t* clock,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API int64_t TOBIISDK_CALL
tobii_sdk_clock_get_resolution(
		tobii_sdk_clock_t* clock,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);


TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_clock_destroy(
		tobii_sdk_clock_t* clock
	) TOBIISDK_NONNULL(1);




#ifdef __cplusplus
}
#endif

#endif // __TOBII_SDK_CORE_CLOCK_H__
