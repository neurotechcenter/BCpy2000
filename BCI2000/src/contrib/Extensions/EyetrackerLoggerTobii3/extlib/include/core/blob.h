#ifndef __TOBII_SDK_CORE_BLOB_H__
#define __TOBII_SDK_CORE_BLOB_H__

#ifndef __TOBII_SDK_INCLUDING__
#	error "You cannot include 'blob.h' directly."
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct tobii_sdk_blob_t tobii_sdk_blob_t;

TOBIISDK_API tobii_sdk_blob_t* TOBIISDK_CALL
tobii_sdk_blob_create (
		uint32_t size,
		tobii_sdk_error_t** error
	);

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_blob_get_size (
		tobii_sdk_blob_t* blob,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API uint8_t* TOBIISDK_CALL
tobii_sdk_blob_get_data (
		tobii_sdk_blob_t* blob,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_blob_destroy (
		tobii_sdk_blob_t* blob
	) TOBIISDK_NONNULL(1);



#ifdef __cplusplus
}
#endif

#endif // __TOBII_SDK_CORE_BLOB_H__
