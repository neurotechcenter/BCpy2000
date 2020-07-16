/* ========================================================================================
 * g.tec Device Service - Client API for g.USBamp devices
 * Copyright (c) 2012 Guger Technologies OG
 * ========================================================================================
 */

#ifndef GDSCLIENTAPI_GUSBAMP_HPP
#define GDSCLIENTAPI_GUSBAMP_HPP

#include "GDSClientAPI.h"

#ifdef __cplusplus
extern "C" {
#endif


// ========================================================================================
// Constants
// ========================================================================================

//! The maximum number of channels a g.USBamp device can provide.
#define GDS_GUSBAMP_CHANNELS_MAX 16

//! The number of groups of a g.USBamp device that can be connected to separate or common ground and reference channels.
#define GDS_GUSBAMP_GROUPS_MAX 4

//! The number of asynchronous digital I/O channels a g.USBamp device provides.
/*!
	g.USBamp version 3.0 devices provide 4 digital output channels.
	g.USBamp version 2.0 devices provide 2 digital input channels and 2 digital output channels.
 */
#define GDS_GUSBAMP_ASYNC_DIGITAL_IO_CHANNELS_MAX 4


// ========================================================================================
// Enumerations
// ========================================================================================

//! The wave shapes provided by the internal signal generator.
enum GDS_GUSBAMP_WAVESHAPE
{
	//! A square wave.
	GDS_GUSBAMP_WAVESHAPE_SQUARE,

	//! A saw tooth.
	GDS_GUSBAMP_WAVESHAPE_SAWTOOTH,

	//! A sine wave.
	GDS_GUSBAMP_WAVESHAPE_SINE,

	//! Driven Right Leg (DRL).
	GDS_GUSBAMP_WAVESHAPE_DRL,

	//! Noise.
	GDS_GUSBAMP_WAVESHAPE_NOISE
};


// ========================================================================================
// Structures
// ========================================================================================

//! Describes the state of an asynchronous digital I/O channel.
typedef struct _GDS_GUSBAMP_ASYNC_DIGITAL_IO_CHANNEL
{
	//! The one-based channel number of the digital I/O channel described by this instance.
	unsigned int ChannelNumber;

	//! Indicates if the channel described by this instance is a digital input or output channel.
	GDS_CHANNEL_DIRECTION Direction;

	//! The digital value of the channel.
	BOOL Value;
} GDS_GUSBAMP_ASYNC_DIGITAL_IO_CHANNEL, *PGDS_GUSBAMP_ASYNC_DIGITAL_IO_CHANNEL;

//! The scaling values of each channel of a single device.
typedef struct _GDS_GUSBAMP_SCALING
{
	//! The offset for each chanel in microvolts.
	float Offset[GDS_GUSBAMP_CHANNELS_MAX];

	//! The scaling factor for each channel.
	float ScalingFactor[GDS_GUSBAMP_CHANNELS_MAX];
} GDS_GUSBAMP_SCALING, *PGDS_GUSBAMP_SCALING;

//! The configuration of the internal signal generator.
typedef struct _GDS_GUSBAMP_SIGNAL_GENERATOR_CONFIGURATION
{
	//! Indicates whether the internal signal generator should be enabled.
	BOOL Enabled;

	//! The wave shape of the signal generator.
	GDS_GUSBAMP_WAVESHAPE WaveShape;

	//! The frequency of the signal generator in hertz.
	/*!
		Valid values range from 1 Hz to 100 Hz.
	 */
	uint32_t Frequency;

	//! The amplitude of the signal generator in mV.
	/*!
		Valid values range from -250 mV to +250 mV.
	 */
	int32_t Amplitude;

	//! The offset of the signal generator in mV.
	/*!
		Valid values range from -200 mV to +200 mV.
	 */
	int32_t Offset;

} GDS_GUSBAMP_SIGNAL_GENERATOR_CONFIGURATION, *PGDS_GUSBAMP_SIGNAL_GENERATOR_CONFIGURATION;

//! The configuration of a single channel.
typedef struct _GDS_GUSBAMP_CHANNEL_CONFIGURATION
{
	//! Indicates whether the channel that this configuration belongs to should be included in data acquisition.
	BOOL Acquire;

	//! The bipolar derivation setting for the channel.
	/*!
		Set this value to zero if unipolar derivation should be used with ground and reference electrodes that are connected to the ground and reference socket of the group that this channel belongs to.
		Otherwise, if bipolar derivation should be performed for the channel that this configuration belongs to, set \ref BipolarChannel to the one-based channel number whose signal should be subtracted from the measured signal of this channel.
	 */
	uint32_t BipolarChannel;

	//! The zero-based index of the bandpass filter to use for the channel.
	/*!
		The index is related to the list of available bandpass filters that are returned by \ref GDS_GUSBAMP_GetBandpassFilters.
		A value of -1 indicates that no bandpass filter is used for the channel.
	 */
	int32_t BandpassFilterIndex;

	//! The zero-based index of the notch filter to use for the channel.
	/*!
		The index is related to the list of available notch filters that are returned by \ref GDS_GUSBAMP_GetNotchFilters.
		A value of -1 indicates that no notch filter is used for the channel.
	 */
	int32_t NotchFilterIndex;
} GDS_GUSBAMP_CHANNEL_CONFIGURATION, *PGDS_GUSBAMP_CHANNEL_CONFIGURATION;

//! The configuration of a g.USBamp device.
typedef struct _GDS_GUSBAMP_CONFIGURATION
{
	//! The sample rate of the analog inputs in hertz.
	/*!
		Valid values are: 32, 64, 128, 256, 512, 600, 1200, 2400, 4800, 9600, 19200, 38400.
		Eight-channel g.USBamp devices support the following sample rates only: 128, 256, 512.
	 */
	uint32_t SampleRate;

	//! The number of scans to retrieve from the device at once, or zero to determine the recommended number of scans for the specified sampling rate automatically.
	/*!
		This value depends on the value of \ref SampleRate. The following table lists the recommended values of \ref NumberOfScans for all possible sample rates.

		\li \c 1 for a sample rate of 32 Hz
		\li \c 2 for a sample rate of 64 Hz
		\li \c 4 for a sample rate of 128 Hz
		\li \c 8 for a sample rate of 256 Hz
		\li \c 16 for a sample rate of 512 Hz
		\li \c 32 for a sample rate of 600 Hz
		\li \c 64 for a sample rate of 1200 Hz
		\li \c 128 for a sample rate of 2400 Hz
		\li \c 256 for a sample rate of 4800 Hz
		\li \c 512 for a sample rate of 9600 Hz
		\li \c 512 for a sample rate of 19200 Hz
		\li \c 512 for a sample rate of 38400 Hz
	 */
	size_t NumberOfScans;

	//! Indicates whether the short cut socket is enabled.
	/*!
		If short cut is enabled, a HIGH level on the SC input socket of the amplifier disconnects the electrodes from the amplifier input stage and holds the lastly measured values as long as the HIGH level is applied.
		If short cut is disabled, the level on the SC input socket of the amplifier is ignored and the amplifier always delivers the currently measured values on its input channels.
	 */
	BOOL ShortCutEnabled;

	//! Indicates whether a sample counter should be applied on channel 16 instead of the measured signal if selected for acquisition (overrun at 1000000 samples).
	BOOL CounterEnabled;

	//! Indicates whether the synchronously sampled digital trigger input channels should be included in data acquisition in an additional logical channel that is appended to the analog synchronously sampled input channels.
	BOOL TriggerEnabled;

	//! The configuration of the internal signal generator.
	GDS_GUSBAMP_SIGNAL_GENERATOR_CONFIGURATION InternalSignalGenerator;

	//! The common ground setting for each of the four groups A, B, C, and D.
	/*!
		Each element in the array indicates whether the ground socket of the corresponding group (A, B, C, or D) should be connected to common ground. The common ground setting for group A (channels 1 to 4) is represented by the element at index zero, group B (channels 5 to 8) at index 1, group C (channels 9 to 12) at index 2, and group D (channels 13 to 16) at index 3.
		A value of zero of an element in the array indicates that the ground socket of the corresponding group is not connected to common ground. 
		A non-zero element indicates that the ground socket of the corresponding group is connected to common ground.
	 */
	BOOL CommonGround[GDS_GUSBAMP_GROUPS_MAX];

	//! The common reference setting for each of the four groups A, B, C, and D.
	/*!
		Each element in the array indicates whether the reference socket of the corresponding group (A, B, C, or D) should be connected to common reference. The common reference setting for group A (channels 1 to 4) is represented by the element at index zero, group B (channels 5 to 8) at index 1, group C (channels 9 to 12) at index 2, and group D (channels 13 to 16) at index 3.
		A value of zero of an element in the array indicates that the reference socket of the corresponding group is not connected to common reference.
		A non-zero element indicates that the reference socket of the corresponding group is connected to common reference.
	 */
	BOOL CommonReference[GDS_GUSBAMP_GROUPS_MAX];

	//! The configuration of each channel.
	/*!
		The element at zero-based index <i>i</i> holds the configuration for the channel with one-based channel number <i>i + 1</i>.
	 */
	GDS_GUSBAMP_CHANNEL_CONFIGURATION Channels[GDS_GUSBAMP_CHANNELS_MAX];

} GDS_GUSBAMP_CONFIGURATION, *PGDS_GUSBAMP_CONFIGURATION;


// ========================================================================================
// API Methods
// ========================================================================================

//! Gets the list of available bandpass filters for the specified g.USBamp device of the data acquisition session associated with the specified connection handle.
/*!
	\a bandpassFilters is a pointer to an array that must be allocated by the caller before. To determine the number of elements of the array to allocate, call \ref GDS_GUSBAMP_GetBandpassFilters with \a bandpassFilters set to \c NULL.
	The function then returns the number of elements of the \a bandpassFilters array in \a bandpassFiltersCount. Allocate a \ref GDS_FILTER_INFO array of that size and pass it on to another call of \ref GDS_GUSBAMP_GetBandpassFilters to retrieve the list of available bandpass filters.

	\param connectionHandle		The connection handle whose associated data acquisition contains the device to get the list of available bandpass filters for.
	\param deviceName			The name of the device to get the list of available bandpass filters for.
	\param bandpassFilters		A pointer to the first element of an allocated array of type \ref GDS_FILTER_INFO and size \a bandpassFiltersCount, or \c NULL to receive the number of available bandpass filters in \a bandpassFiltersCount only.
	\param bandpassFiltersCount	A pointer to a variable that receives the number of available bandpass filters for the specified device.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GUSBAMP_GetBandpassFilters(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_FILTER_INFO *bandpassFilters, size_t *bandpassFiltersCount);

//! Gets the list of available notch filters for the specified g.USBamp device of the data acquisition session associated with the specified connection handle.
/*!
	\a notchFilters is a pointer to an array that must be allocated by the caller before. To determine the number of elements of the array to allocate, call \ref GDS_GUSBAMP_GetNotchFilters with \a notchFilters set to \c NULL.
	The function then returns the number of elements of the \a notchFilters array in \a notchFiltersCount. Allocate a \ref GDS_FILTER_INFO array of that size and pass it on to another call of \ref GDS_GUSBAMP_GetNotchFilters to retrieve the list of available notch filters.

	\param connectionHandle		The connection handle whose associated data acquisition contains the device to get the list of available notch filters for.
	\param deviceName			The name of the device to get the list of available notch filters for.
	\param notchFilters			A pointer to the first element of an allocated array of type \ref GDS_FILTER_INFO and size \a notchFiltersCount, or \c NULL to receive the number of available notch filters in \a notchFiltersCount only.
	\param notchFiltersCount	A pointer to a variable that receives the number of available notch filters for the specified device.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GUSBAMP_GetNotchFilters(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_FILTER_INFO *notchFilters, size_t *notchFiltersCount);

//! Gets the current values of all asynchronous digital input and output channels for the specified g.USBamp device of the data acquisition session associated with the specified connection handle.
/*!
	\param connectionHandle		The connection handle whose associated data acquisition session contains the device to get the current asynchronous digital I/O channels's current values for.
	\param deviceName			The name of the device to get the asynchronous digital I/O channels's current values for.
	\param asyncDigitalIOs		A reference to a fixed size array of a number of exactly \ref GDS_GUSBAMP_ASYNC_DIGITAL_IO_CHANNELS_MAX elements of type \ref GDS_GUSBAMP_ASYNC_DIGITAL_IO_CHANNEL that receives the current values of all provided asynchronous digital input and output channels.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GUSBAMP_GetAsyncDigitalIOs(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GUSBAMP_ASYNC_DIGITAL_IO_CHANNEL (&asyncDigitalIOs)[GDS_GUSBAMP_ASYNC_DIGITAL_IO_CHANNELS_MAX]);

//! Sets or changes the values of the specified asynchronous digital output channels for the specified g.USBamp device of the data acquisition session associated with the specified connection handle.
/*!
	Only the output channels that are specified in the \a asyncDigitalOutputs array are modified, others preserve their current state.
	g.USBamp version 3.0 devices accept channel numbers from 1 to 4.
	g.USBamp version 2.0 devices accept channel numbers from 1 to 2.

	\param connectionHandle			The connection handle whose associated data acquisition session contains the device whose asynchronous digital output channel values should be set or changed.
	\param deviceName				The name of the device whose asynchronous digital output channel values should be set or changed.
	\param asyncDigitalOutputs		Contains the channels to modify together with their new values.
	\param asyncDigitalOutputsCount	The number of elements in the specified \a asyncDigitalOutputs array.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GUSBAMP_SetAsyncDigitalOutputs(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GUSBAMP_ASYNC_DIGITAL_IO_CHANNEL *asyncDigitalOutputs, size_t asyncDigitalOutputsCount);

//! Gets the scaling values for the specified g.USBamp device of the data acquisition session associated with the specified connection handle.
/*!
	\param connectionHandle	The connection handle whose associated data acquisition session contains the device to get the scaling values for.
	\param deviceName		The name of the device to get the scaling values for.
	\param scaling			A pointer to an allocated \ref GDS_GUSBAMP_SCALING structure that receives the scaling values for the specified device.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GUSBAMP_GetScaling(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GUSBAMP_SCALING *scaling);

//! Sets the scaling values for the specified g.USBamp device of the data acquisition session associated with the specified connection handle.
/*!
	\param connectionHandle	The connection handle whose associated data acquisition session contains the device to set the scaling values for.
	\param deviceName		The name of the device to set the scaling values for.
	\param scaling			A pointer to an initialized \ref GDS_GUSBAMP_SCALING structure that contains the scaling values to set.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GUSBAMP_SetScaling(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GUSBAMP_SCALING *scaling);

//! Calculates new scaling values for each channel under the current conditions of the specified g.USBamp device of the data acquisition session associated with the specified connection handle.
/*!
	The calculated scaling values are not applied on the device yet. Use \ref GDS_GUSBAMP_SetScaling with the returned \a scaling structure to apply them on the device.

	\param connectionHandle	The connection handle whose associated data acquisition session contains the device to calibrate.
	\param deviceName		The name of the device to calibrate.
	\param scaling			A pointer to an allocated \ref GDS_GUSBAMP_SCALING structure that receives the calculated scaling values for the specified device.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GUSBAMP_Calibrate(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GUSBAMP_SCALING *scaling);

//! Measures the impedance at the specified channel of the specified g.USBamp device of the data acquisition session associated with the specified connection handle.
/*!
	All grounds are connected to common ground during impedance measurement. Impedance is measured between the specified channel and ground.

	Impedance can be measured of ordinary synchronous analog input channels and each group's reference channel.
	The following table lists valid values for \a channelNumber:

	\li \c 1..16 for synchronous analog input channels 1 to 16
	\li \c 17 for reference channel of group A (REFA)
	\li \c 18 for reference channel of group B (REFB)
	\li \c 19 for reference channel of group C (REFC)
	\li \c 20 for reference channel of group D (REFD)

	\param connectionHandle	The connection handle whose associated data acquisition session contains the device to use for impedance measurement.
	\param deviceName		The name of the device to use for impedance measurement.
	\param channelNumber	The one-based number of the channel whose impedance should be measured.
	\param impedance		A pointer to a variable that receives the impedance value of the specified channel in ohm.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GUSBAMP_GetImpedance(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], unsigned char channelNumber, double *impedance);


#ifdef __cplusplus
}
#endif

#endif //GDSCLIENTAPI_GUSBAMP_HPP