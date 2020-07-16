#ifndef __TOBII_SDK_CORE_SYNC_MANAGER_H__
#define __TOBII_SDK_CORE_SYNC_MANAGER_H__

#ifndef __TOBII_SDK_INCLUDING__
#	error "You cannot include 'syncmanager.h' directly."
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * sync state
 */
typedef struct tobii_sdk_sync_state_t tobii_sdk_sync_state_t;

typedef uint32_t tobii_sdk_sync_state_flag_t;

enum {
	TOBII_SDK_SYNC_STATUS_UNSYNCHRONIZED,
	TOBII_SDK_SYNC_STATUS_STABILIZING,
	TOBII_SDK_SYNC_STATUS_SYNCHRONIZED
};

TOBIISDK_API tobii_sdk_sync_state_t* TOBIISDK_CALL
tobii_sdk_sync_state_clone (
		const tobii_sdk_sync_state_t* state,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_sync_state_destroy (
		tobii_sdk_sync_state_t* state
	);

TOBIISDK_API tobii_sdk_sync_state_flag_t TOBIISDK_CALL
tobii_sdk_sync_state_get_sync_state_flag (
		const tobii_sdk_sync_state_t* state,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_sync_state_get_number_of_points_in_use (
		const tobii_sdk_sync_state_t* state,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_sync_state_get_point_in_use (
		const tobii_sdk_sync_state_t* state,
		uint32_t index,
		int64_t* local_midpoint,
		int64_t* remote,
		int64_t* roundtrip,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1, 3, 4, 5);

TOBIISDK_API int64_t TOBIISDK_CALL
tobii_sdk_sync_state_get_error_approximation (
		const tobii_sdk_sync_state_t* state,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

/*
 * sync manager
 */

typedef struct tobii_sdk_sync_manager_t tobii_sdk_sync_manager_t;

typedef void (TOBIISDK_CALL *tobii_sdk_sync_manager_error_handler_t) (
		tobii_sdk_error_code_t error,
		void* user_data
	);

typedef void (TOBIISDK_CALL *tobii_sdk_sync_manager_status_changed_handler_t) (
		const tobii_sdk_sync_state_t* state,
		void* user_data
	);

TOBIISDK_API tobii_sdk_sync_manager_t* TOBIISDK_CALL
tobii_sdk_sync_manager_create(
		tobii_sdk_clock_t* clock,
		tobii_sdk_factory_info_t* factory_info,
		tobii_sdk_mainloop_t* loop,
		tobii_sdk_sync_manager_error_handler_t error_handler,
		void* error_handler_user_data,
		tobii_sdk_sync_manager_status_changed_handler_t status_handler,
		void* status_changed_user_data,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1, 2, 3);

TOBIISDK_API int64_t TOBIISDK_CALL
tobii_sdk_sync_manager_convert_from_local_to_remote(
		const tobii_sdk_sync_manager_t* sync_manager,
		int64_t local,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API int64_t TOBIISDK_CALL
tobii_sdk_sync_manager_convert_from_remote_to_local(
		const tobii_sdk_sync_manager_t* sync_manager,
		int64_t remote,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API tobii_sdk_sync_state_t* TOBIISDK_CALL
tobii_sdk_sync_manager_get_sync_state(
		const tobii_sdk_sync_manager_t* sync_manager,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_sync_manager_destroy(
		tobii_sdk_sync_manager_t* sync_manager
	) TOBIISDK_NONNULL(1);

#ifdef __cplusplus
}
#endif

#endif // __TOBII_SDK_CORE_SYNC_MANAGER_H__
