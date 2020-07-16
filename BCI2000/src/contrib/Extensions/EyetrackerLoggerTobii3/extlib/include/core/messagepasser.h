
#ifndef __TOBII_SDK_CORE_MESSAGEPASSER_H__
#define __TOBII_SDK_CORE_MESSAGEPASSER_H__

#ifndef __TOBII_SDK_INCLUDING__
#	error "You cannot include 'messagepasser.h' directly."
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * tobii_sdk_connection_callback_t
 */
typedef struct tobii_sdk_callback_connection_t tobii_sdk_callback_connection_t;

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_callback_connection_destroy (
		tobii_sdk_callback_connection_t* callback_connection
	) TOBIISDK_NONNULL(1);

/*
 * tobii_sdk_message_passer_t
 */

typedef uint32_t tobii_sdk_opcode_t;

typedef struct tobii_sdk_message_passer_t tobii_sdk_message_passer_t;

typedef void (TOBIISDK_CALL *tobii_sdk_message_passer_get_handler_t) (
		tobii_sdk_error_t* error,
		tobii_sdk_message_passer_t* mpi,
		void* user_data
	);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_message_passer_get
	(
		tobii_sdk_factory_info_t* factory_info,
		tobii_sdk_mainloop_t* loop,
		tobii_sdk_message_passer_get_handler_t handler,
		void* handler_user_data,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1, 2, 3);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_message_passer_destroy (
		tobii_sdk_message_passer_t* mpi
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_message_passer_enable_nop_sending (
		tobii_sdk_message_passer_t* mpi,
		tobii_sdk_opcode_t opcode,
		const tobii_sdk_param_stack_t* params,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

TOBIISDK_API void TOBIISDK_CALL
tobii_sdk_message_passer_disable_nop_sending (
		tobii_sdk_message_passer_t* mpi,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

typedef void (TOBIISDK_CALL *tobii_sdk_response_handler_t) (
		tobii_sdk_opcode_t opcode,
		tobii_sdk_error_code_t error_code,
		const tobii_sdk_param_stack_t* params,
		void* user_data
	) TOBIISDK_NONNULL(3);

TOBIISDK_API uint32_t TOBIISDK_CALL
tobii_sdk_message_passer_execute_request (
		tobii_sdk_message_passer_t* mpi,
		tobii_sdk_opcode_t opcode,
		const tobii_sdk_param_stack_t* params,
		tobii_sdk_response_handler_t response_handler,
		void* response_user_data,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1, 4);

typedef void (TOBIISDK_CALL *tobii_sdk_data_handler_t) (
		tobii_sdk_opcode_t opcode,
		tobii_sdk_error_code_t error_code,
		const tobii_sdk_param_stack_t* params,
		void* user_data
	);

TOBIISDK_API tobii_sdk_callback_connection_t* TOBIISDK_CALL
tobii_sdk_message_passer_add_data_handler (
		tobii_sdk_message_passer_t* mpi,
		tobii_sdk_opcode_t opcode,
		tobii_sdk_data_handler_t data_handler,
		void* user_data,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1, 3);

typedef void (TOBIISDK_CALL *tobii_sdk_error_handler_t) (
		tobii_sdk_error_code_t error_code,
		void* user_data
	);

TOBIISDK_API tobii_sdk_callback_connection_t* TOBIISDK_CALL
tobii_sdk_message_passer_add_error_handler (
		tobii_sdk_message_passer_t* mpi,
		tobii_sdk_error_handler_t error_handler,
		void* user_data,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1, 3);

#ifdef __cplusplus
}
#endif

#endif // __TOBII_SDK_CORE_MESSAGEPASSER_H__
