#ifndef __TOBII_SDK_CORE_UPGRADE_H__
#define __TOBII_SDK_CORE_UPGRADE_H__

#ifndef __TOBII_SDK_INCLUDING__
#	error "You cannot include 'upgrade.h' directly."
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum tobii_sdk_upgrade_package_part_flags_t {
	TOBII_SDK_UPGRADE_PACKAGE_PART_FLAG_MUST_UNDERSTAND 		= 0x1
} tobii_sdk_upgrade_package_part_flags_t;

typedef enum tobii_sdk_upgrade_package_part_exec_flags_t {
	TOBII_SDK_UPGRADE_PACKAGE_PART_EXEC_FLAG_USE_EXPECT_CODE 	= 0x1,
	TOBII_SDK_UPGRADE_PACKAGE_PART_EXEC_FLAG_USE_PROGRESS 	 	= 0x2
} tobii_sdk_upgrade_package_part_exec_flags_t;

/*
 * upgrade package
 */
typedef struct tobii_sdk_upgrade_package_t tobii_sdk_upgrade_package_t;

typedef struct tobii_sdk_upgrade_package_part_t tobii_sdk_upgrade_package_part_t;

typedef struct tobii_sdk_upgrade_package_finalized_t tobii_sdk_upgrade_package_finalized_t;

typedef struct tobii_sdk_upgrade_progress_reporter_t tobii_sdk_upgrade_progress_reporter_t;

TOBIISDK_API tobii_sdk_upgrade_package_t* TOBIISDK_CALL
tobii_sdk_upgrade_package_create (
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API tobii_sdk_upgrade_package_t* TOBIISDK_CALL
tobii_sdk_upgrade_package_parse (
		const void* package,
		uint32_t package_size,
		uint32_t allow_unknown_parts,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_upgrade_package_destroy (
		tobii_sdk_upgrade_package_t* package
	) TOBIISDK_NONNULL(1);

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_upgrade_package_get_num_parts (
		const tobii_sdk_upgrade_package_t* package,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API tobii_sdk_upgrade_package_part_t* TOBIISDK_CALL
tobii_sdk_upgrade_package_get_part (
		tobii_sdk_upgrade_package_t* package,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_upgrade_package_append_part (
		tobii_sdk_upgrade_package_t* package,
		tobii_sdk_upgrade_package_part_t* part,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1, 2);

/*
 * finalization
 */

TOBIISDK_API tobii_sdk_upgrade_package_finalized_t* TOBIISDK_CALL
tobii_sdk_upgrade_package_finalize (
		tobii_sdk_upgrade_package_t* package,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API const uint8_t* TOBIISDK_CALL
tobii_sdk_upgrade_package_finalized_get_data (
		const tobii_sdk_upgrade_package_finalized_t* finalized,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_upgrade_package_finalized_get_size (
		const tobii_sdk_upgrade_package_finalized_t* finalized,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_upgrade_package_finalized_destroy (
		tobii_sdk_upgrade_package_finalized_t* finalized
	) TOBIISDK_NONNULL(1);

/*
 * parts
 */
TOBIISDK_API tobii_sdk_upgrade_package_part_t* TOBIISDK_CALL
tobii_sdk_upgrade_package_part_create_raw_part (
		uint32_t type,
		uint32_t flags,
		uint32_t size,
		uint32_t id,
		uint32_t order,
		const uint8_t* payload,
		tobii_sdk_error_t** error
	);

TOBIISDK_API tobii_sdk_upgrade_package_part_t* TOBIISDK_CALL
tobii_sdk_upgrade_package_part_create_legacy_metadata_part (
		uint32_t flags,
		uint32_t id,
		uint32_t order,
		const char* key,
		const char* value,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(5, 6);

TOBIISDK_API tobii_sdk_upgrade_package_part_t* TOBIISDK_CALL
tobii_sdk_upgrade_package_part_create_legacy_file_part (
		uint32_t flags,
		uint32_t id,
		uint32_t order,
		const char* target_path,
		const uint8_t* file_contents,
		uint32_t file_size,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(4, 5);

TOBIISDK_API tobii_sdk_upgrade_package_part_t* TOBIISDK_CALL
tobii_sdk_upgrade_package_part_create_legacy_exec_command_part (
		uint32_t flags,
		uint32_t id,
		uint32_t order,
		uint32_t exec_flags,
		const char* command_line,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(5);

TOBIISDK_API tobii_sdk_upgrade_package_part_t* TOBIISDK_CALL
tobii_sdk_upgrade_package_part_create_cancancel_part (
		uint32_t flags,
		uint32_t id,
		uint32_t order,
		uint32_t cancancel,
		tobii_sdk_error_t** error
	);

TOBIISDK_API const char* TOBIISDK_CALL
tobii_sdk_upgrade_package_part_get_representation (
		const tobii_sdk_upgrade_package_part_t* part,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_upgrade_package_part_destroy (
		tobii_sdk_upgrade_package_part_t* part
	) TOBIISDK_NONNULL(1);


/*
 * progress reporter
 */

typedef void (TOBIISDK_CALL *tobii_sdk_upgrade_completed_handler_t) (
		tobii_sdk_upgrade_progress_reporter_t* progress_reporter,
		tobii_sdk_error_t* error,
		void* user_data
	);

typedef void (TOBIISDK_CALL *tobii_sdk_upgrade_progress_handler_t) (
		tobii_sdk_upgrade_progress_reporter_t* progress_reporter,
		uint32_t current_step,
		uint32_t number_of_steps,
		uint32_t percentage,
		void* user_data
	);

typedef void (TOBIISDK_CALL *tobii_sdk_upgrade_cancancel_changed_handler_t) (
		tobii_sdk_upgrade_progress_reporter_t* progress_reporter,
		uint32_t can_cancel,
		void* user_data
	);

TOBIISDK_API tobii_sdk_upgrade_progress_reporter_t* TOBIISDK_CALL
tobii_sdk_upgrade_progress_reporter_create(
		tobii_sdk_upgrade_completed_handler_t completion_handler,
		tobii_sdk_upgrade_progress_handler_t progress_handler,
		tobii_sdk_upgrade_cancancel_changed_handler_t cancancel_handler,
		void* user_data,
		tobii_sdk_error_t** error);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_upgrade_progress_reporter_get_progress (
		tobii_sdk_upgrade_progress_reporter_t* progress_reporter,
		uint32_t* current_step,
		uint32_t* number_of_steps,
		uint32_t* percent,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1, 2, 3, 4);

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_upgrade_progress_get_can_cancel (
		tobii_sdk_upgrade_progress_reporter_t* progress_reporter,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_upgrade_progress_cancel (
		tobii_sdk_upgrade_progress_reporter_t* progress_reporter,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_upgrade_progress_reporter_destroy(
		tobii_sdk_upgrade_progress_reporter_t* progress_reporter
	) TOBIISDK_NONNULL(1);


/*
 * driver
 */
TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_upgrade_package_is_compatible_with_device (
		tobii_sdk_mainloop_t* mainloop,
		const tobii_sdk_upgrade_package_t* package,
		const tobii_sdk_device_info_t* device,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1, 2, 3);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_upgrade_begin (
		tobii_sdk_mainloop_t* mainloop,
		const tobii_sdk_upgrade_package_t* package,
		const tobii_sdk_device_info_t* device,
		tobii_sdk_upgrade_progress_reporter_t* progress_reporter,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1, 2, 3, 4);

#ifdef __cplusplus
}
#endif

#endif // __TOBII_SDK_CORE_UPGRADE_H__
