#ifndef __TOBII_SDK_CORE_PARAMTYPE_H__
#define __TOBII_SDK_CORE_PARAMTYPE_H__

#ifndef __TOBII_SDK_INCLUDING__
#	error "You cannot include paramtype.h directly."
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum tobii_sdk_param_type_t {
	TOBII_SDK_PARAM_TYPE_VOID,			// 0
	TOBII_SDK_PARAM_TYPE_INT32,			// 1
	TOBII_SDK_PARAM_TYPE_UINT32,		// 2
	TOBII_SDK_PARAM_TYPE_FIXED_15x16,	// 3
	TOBII_SDK_PARAM_TYPE_FIXED_22x41,	// 4
	TOBII_SDK_PARAM_TYPE_FLOAT32,		// 5
	TOBII_SDK_PARAM_TYPE_FLOAT64,		// 6
	TOBII_SDK_PARAM_TYPE_STRING,		// 7
	TOBII_SDK_PARAM_TYPE_BLOB,			// 8
	TOBII_SDK_PARAM_TYPE_VECTOR,		// 9
	TOBII_SDK_PARAM_TYPE_NODE_PROLOG,	// 10
	TOBII_SDK_PARAM_TYPE_INT64,			// 11
	TOBII_SDK_PARAM_TYPE_UINT64,		// 12
	__TOBII_SDK_PARAM_TYPE__LAST
} tobii_sdk_param_type_t;

#ifdef __cplusplus
}
#endif

#endif // __TOBII_SDK_CORE_PARAMTYPE_H__
