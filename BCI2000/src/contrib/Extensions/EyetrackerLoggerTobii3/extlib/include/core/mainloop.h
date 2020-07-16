/**@file
 * @author Mikael Olenfalk
 * @date 2009-01-22
 *
 * @brief Defines the mainloop MPI API
 *
 * This file cannot be included directly, include mpi.h instead.
 */

#ifndef __TOBII_SDK_CORE_MAINLOOP_H__
#define __TOBII_SDK_CORE_MAINLOOP_H__

#ifndef __TOBII_SDK_INCLUDING__
#	error "You cannot include 'mainloop.h' directly."
#endif


#ifdef __cplusplus
extern "C"
{
#endif

/*
 * mainloop interface
 */
typedef struct tobii_sdk_mainloop_t tobii_sdk_mainloop_t;

/**
 * @brief Creates a new mainloop and returns it.
 */
TOBIISDK_API tobii_sdk_mainloop_t* TOBIISDK_CALL
tobii_sdk_mainloop_create (
		tobii_sdk_error_t** error
	);

/**
 * @brief Executes the mainloop and processes works from the internal mainloop queue.
 *
 * This function blocks until the function tobii_sdk_mainloop_quit() is called. After
 * the quit function is called it returns as soon as remaining work has been executed.
 */
TOBIISDK_API void TOBIISDK_CALL tobii_sdk_mainloop_run (
		tobii_sdk_mainloop_t* loop,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

/**
 * @brief Signals a mainloop currently blocking in tobii_sdk_mainloop_run() to quit as soon as possible.
 *
 * This function can be called from any thread.
 */
TOBIISDK_API void TOBIISDK_CALL tobii_sdk_mainloop_quit (
		tobii_sdk_mainloop_t* loop,
		tobii_sdk_error_t** error
	) TOBIISDK_NONNULL(1);

/**
 * @brief Destroys a mainloop previously returned by tobii_sdk_mainloop_create ()
 *
 * <b>WARNING:</b> The caller is responsible for ensuring that the mainloop is not running anymore.
 *
 * Ensure that a thread executing the mainloop (waiting in tobii_sdk_mainloop_run()) has been
 * joined before calling this function.
 */
TOBIISDK_API void TOBIISDK_CALL tobii_sdk_mainloop_destroy (
		tobii_sdk_mainloop_t* loop
	) TOBIISDK_NONNULL(1);

#ifdef __cplusplus
}
#endif


#endif // __TOBII_SDK_CORE_MAINLOOP_H__
