
#ifndef __TOBII_SDK_CORE_DISCOVERY_H__
#define __TOBII_SDK_CORE_DISCOVERY_H__

#ifndef __TOBII_SDK_INCLUDING__
#	error "You cannot include 'discovery.h' directly."
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct tobii_sdk_device_browser_t tobii_sdk_device_browser_t;

typedef struct tobii_sdk_device_info_t tobii_sdk_device_info_t;

typedef struct tobii_sdk_factory_info_t tobii_sdk_factory_info_t;

/*
 * iterator interface
 */

typedef struct tobii_sdk_device_info_iterator_t tobii_sdk_device_info_iterator_t;

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_device_info_iterator_move_next (
		tobii_sdk_device_info_iterator_t* iterator,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);


TOBIISDK_API const char* TOBIISDK_CALL
tobii_sdk_device_info_iterator_get_key_name (
		tobii_sdk_device_info_iterator_t* iterator,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_device_info_iterator_destroy (
		tobii_sdk_device_info_iterator_t* iterator
	) TOBIISDK_NONNULL(1);

/*
 * device info interface
 */

TOBIISDK_API tobii_sdk_device_info_t* TOBIISDK_CALL
tobii_sdk_device_info_clone (
		const tobii_sdk_device_info_t* device_info,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API tobii_sdk_device_info_t* TOBIISDK_CALL
tobii_sdk_device_info_create_test (
		tobii_sdk_error_t** error
	);

TOBIISDK_API const char* TOBIISDK_CALL
tobii_sdk_device_info_get_product_id (
		tobii_sdk_device_info_t* device_info,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API const char* TOBIISDK_CALL
tobii_sdk_device_info_get (
		tobii_sdk_device_info_t* device_info,
		const char* key,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1, 2);

TOBIISDK_API tobii_sdk_device_info_iterator_t* TOBIISDK_CALL
tobii_sdk_device_info_get_iterator (
		tobii_sdk_device_info_t* device_info,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API tobii_sdk_factory_info_t* TOBIISDK_CALL
tobii_sdk_device_info_get_factory_info (
		tobii_sdk_device_info_t* device_info,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_device_info_destroy (
		tobii_sdk_device_info_t* device_info
	) TOBIISDK_NONNULL(1);

/*
 * factory info interface
 */
TOBIISDK_API tobii_sdk_factory_info_t* TOBIISDK_CALL
tobii_sdk_factory_info_create_for_networked_eyetracker (
		const char* ip_address_or_hostname,
		uint32_t tetserver_port,
		uint32_t synchronization_port,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API const char* TOBIISDK_CALL
tobii_sdk_factory_info_get_representation (
		tobii_sdk_factory_info_t* factory_info,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_factory_info_destroy (
		tobii_sdk_factory_info_t* factory_info
	) TOBIISDK_NONNULL(1);

/*
 * the device browser
 */

typedef uint32_t tobii_sdk_on_device_browser_event_t;
#define TOBII_SDK_DEVICE_FOUND 0
#define TOBII_SDK_DEVICE_UPDATED 1
#define TOBII_SDK_DEVICE_REMOVED 2

typedef void (*tobii_sdk_device_browser_callback_t)
	(tobii_sdk_on_device_browser_event_t device_event,
		tobii_sdk_device_info_t*,
		void* user_data);

TOBIISDK_API tobii_sdk_device_browser_t* TOBIISDK_CALL
tobii_sdk_device_browser_create (
		tobii_sdk_mainloop_t* mainloop,
		tobii_sdk_device_browser_callback_t callback,
		void* user_data,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1, 2);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_device_browser_destroy (
		tobii_sdk_device_browser_t* browser
	) TOBIISDK_NONNULL(1);

typedef struct tobii_sdk_device_info_list_t tobii_sdk_device_info_list_t;

TOBIISDK_API tobii_sdk_device_info_list_t* TOBIISDK_CALL
tobii_sdk_device_browser_get_device_list (
		tobii_sdk_device_browser_t* browser,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

/*
 * device info list
 */

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_device_info_list_get_size (
		tobii_sdk_device_info_list_t* device_list,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API tobii_sdk_device_info_t* TOBIISDK_CALL
tobii_sdk_device_info_list_get_at_index (
		tobii_sdk_device_info_list_t* list,
		uint32_t index,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_device_info_list_destroy (
		tobii_sdk_device_info_list_t* device_list
	) TOBIISDK_NONNULL(1);

#ifdef __cplusplus
}
#endif

#endif // __TOBII_SDK_CORE_DISCOVERY_H__
