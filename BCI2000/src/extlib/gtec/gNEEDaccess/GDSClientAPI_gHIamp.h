/* ========================================================================================
 * g.tec Device Service - Client API for g.HIamp devices
 * Copyright (c) 2012 Guger Technologies OG
 * ========================================================================================
 */

#ifndef GDSCLIENTAPI_GHIAMP_HPP
#define GDSCLIENTAPI_GHIAMP_HPP

#include "GDSClientAPI.h"

#ifdef __cplusplus
extern "C" {
#endif


// ========================================================================================
// Constants
// ========================================================================================

//! The maximum number of channels a g.HIamp device provides.
#define GDS_GHIAMP_CHANNELS_MAX 256


// ========================================================================================
// Structures
// ========================================================================================

//! The supported features at a specific sampling rate.
typedef struct _GDS_GHIAMP_SAMPLING_RATE_FEATURES
{
	//! The sampling rate that the features of the current instance belong to.
	uint32_t SamplingRate;

	//! The recommended number of scans for \ref SamplingRate.
	uint16_t RecommendedNumberOfScans;
} GDS_GHIAMP_SAMPLING_RATE_FEATURES, *PGDS_GHIAMP_SAMPLING_RATE_FEATURES;

//! The scaling values of each channel of a single device.
typedef struct _GDS_GHIAMP_SCALING
{
	//! The offset for each channel in µV.
	float Offset[GDS_GHIAMP_CHANNELS_MAX];

	//! The scaling factor for each channel.
	float Factor[GDS_GHIAMP_CHANNELS_MAX];
} GDS_GHIAMP_SCALING, *PGDS_GHIAMP_SCALING;

//! The configuration of the internal signal generator.
typedef struct _GDS_GHIAMP_SIGNAL_GENERATOR_CONFIGURATION
{
	//! Indicates whether the internal signal generator should be enabled.
	BOOL Enabled;

	//! The frequency of the signal generator in Hertz.
	uint16_t Frequency;
} GDS_GHIAMP_SIGNAL_GENERATOR_CONFIGURATION, *PGDS_GHIAMP_SIGNAL_GENERATOR_CONFIGURATION;

//! The configuration of a single channel.
typedef struct _GDS_GHIAMP_CHANNEL_CONFIGURATION
{
	//! Indicates whether the channel that this configuration belongs to should be included in data acquisition.
	/*!
		If set to #FALSE, the remaining settings for the specified channel number will be ignored.
	 */
	BOOL Acquire;

	//! The zero-based index of the bandpass filter to use for the channel.
	/*!
		The index is related to the list of available bandpass filters that are returned by \ref GDS_GHIAMP_GetBandpassFilters.
		A value of -1 indicates that no bandpass filter is used for the channel.
	 */
	int32_t BandpassFilterIndex;

	//! The zero-based index of the notch filter to use for the channel.
	/*!
		The index is related to the list of available notch filters that are returned by \ref GDS_GHIAMP_GetNotchFilters.
		A value of -1 indicates that no notch filter is used for the channel.
	 */
	int32_t NotchFilterIndex;

	//! The one-based number of the channel to use as reference for the current channel, or zero if no reference should be used for the current channel.
	uint16_t ReferenceChannel;
} GDS_GHIAMP_CHANNEL_CONFIGURATION, *PGDS_GHIAMP_CHANNEL_CONFIGURATION;

//! The configuration of a g.HIamp device.
typedef struct _GDS_GHIAMP_CONFIGURATION
{
	//! The sampling rate of the synchronously sampled inputs in Hertz.
	uint32_t SamplingRate;

	//! The number of scans to retrieve from the device at once, or zero to apply an automatically determined number of scans for the specified sampling rate.
	/*!
		Setting \ref NumberOfScans to a value other than zero is recommended for advanced users only.

		This value depends on the value of \ref SampleRate. The following table lists the recommended values of \ref NumberOfScans for all possible sample rates.

		\li \c 8 for a sample rate of 256 Hz
		\li \c 16 for a sample rate of 512 Hz
		\li \c 32 for a sample rate of 600 Hz
		\li \c 64 for a sample rate of 1200 Hz
		\li \c 128 for a sample rate of 2400 Hz
		\li \c 256 for a sample rate of 4800 Hz
		\li \c 256 for a sample rate of 9600 Hz
		\li \c 256 for a sample rate of 19200 Hz
		\li \c 256 for a sample rate of 38400 Hz
	 */
	uint16_t NumberOfScans;

	//! Indicates whether the synchronously sampled digital trigger input channels should be included in data acquisition.
	/*!
		If enabled, the values of the digital trigger input channels come encoded in a single value as an additional channel appended to the analog channels.
	 */
	BOOL TriggerLinesEnabled;

	//! Indicates whether the signal hold input is enabled.
	/*!
		If the signal hold input is enabled, a HIGH level on the HOLD input causes the amplifier to latch the input channels' current values and ignore the actually applied signal until the HOLD input is pulled down to LOW again.
		If the signal hold input is disabled, the signal applied to the HOLD input has no effect.
	 */
	BOOL HoldEnabled;

	//! Indicates whether a sample counter should be applied on the very first channel selected for acquisition.
	/*!
		If enabled, a sample counter is applied to the very first channel that is selected for acquisition. The actually applied signal on that channel is disregarded then.
		The counter value is stored as a 4-byte unsigned integer value at that channel.
	 */
	BOOL CounterEnabled;

	//! The configuration of the internal signal generator.
	GDS_GHIAMP_SIGNAL_GENERATOR_CONFIGURATION InternalSignalGenerator;

	//! The configuration of each channel.
	/*!
		An element at zero-based index <i>i</i> holds the configuration of the channel with one-based channel number <i>i + 1</i>.
		Channels that the device does not provide cannot be selected for acquisition. The other settings in their configuration structure will be ignored.
	 */
	GDS_GHIAMP_CHANNEL_CONFIGURATION Channels[GDS_GHIAMP_CHANNELS_MAX];

} GDS_GHIAMP_CONFIGURATION, *PGDS_GHIAMP_CONFIGURATION;


// ========================================================================================
// API Methods
// ========================================================================================

//! Gets a boolean flag for each possible channel a device in its full configuration may provide indicating whether the corresponding channel is actually provided by and available on the device.
/*!
	\param connectionHandle		The connection handle whose associated data acquisition contains the device to get the list of available channels for.
	\param deviceName			The name of the device to get the list of available channels for.
	\param availableChannels	An allocated array of length #GDS_GHIAMP_CHANNELS_MAX where an element at index <i>i</i> receives a boolean flag indicating whether the corresponding channel with one-based channel number <i>i + 1</i> is available on the device.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GHIAMP_GetAvailableChannels(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], BOOL (&availableChannels)[GDS_GHIAMP_CHANNELS_MAX]);

//! Gets a list of supported sampling rates and their supported features.
/*!
	\a supportedSamplingRates is a pointer to the first element of an array that must be allocated by the caller before. To determine the number of elements of the array to allocate, call \ref GDS_GHIAMP_GetSupportedSamplingRates with \a supportedSamplingRates set to \c NULL.
	The function then returns the number of elements of the \a supportedSamplingRates array in \a supportedSamplingRatesCount. Allocate a \ref GDS_GHIAMP_SAMPLING_RATE_FEATURES array of that size and pass it on to another call of \ref GDS_GHIAMP_GetSupportedSamplingRates to retrieve the list of supported sampling rates.

	\param connectionHandle				The connection handle whose associated data acquisition contains the device to get the list of supported sampling rates for.
	\param deviceName					The name of the device to get the list of supported sampling rates for.
	\param supportedSamplingRates		A pointer to the first element of an allocated array of type \ref GDS_GHIAMP_SAMPLING_RATE_FEATURES and size \a supportedSamplingRatesCount that receives the list of supported sampling rates, or \c NULL to receive the number of supported sampling Rates in \a supportedSamplingRatesCount only.
	\param supportedSamplingRatesCount	A pointer to a variable that receives the number of supported sampling rates for the specified device.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GHIAMP_GetSupportedSamplingRates(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GHIAMP_SAMPLING_RATE_FEATURES *supportedSamplingRates, size_t *supportedSamplingRatesCount);

//! Gets the list of available bandpass filters for the specified g.HIamp device of the data acquisition session associated with the specified connection handle.
/*!
	\a bandpassFilters is a pointer to the first element of an array that must be allocated by the caller before. To determine the number of elements of the array to allocate, call \ref GDS_GHIAMP_GetBandpassFilters with \a bandpassFilters set to \c NULL.
	The function then returns the number of elements of the \a bandpassFilters array in \a bandpassFiltersCount. Allocate a \ref GDS_FILTER_INFO array of that size and pass it on to another call of \ref GDS_GHIAMP_GetBandpassFilters to retrieve the list of available bandpass filters.

	\param connectionHandle		The connection handle whose associated data acquisition contains the device to get the list of available bandpass filters for.
	\param deviceName			The name of the device to get the list of available bandpass filters for.
	\param bandpassFilters		A pointer to the first element of an allocated array of type \ref GDS_FILTER_INFO and size \a bandpassFiltersCount, or \c NULL to receive the number of available bandpass filters in \a bandpassFiltersCount only.
	\param bandpassFiltersCount	A pointer to a variable that receives the number of available bandpass filters for the specified device.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GHIAMP_GetBandpassFilters(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_FILTER_INFO *bandpassFilters, size_t *bandpassFiltersCount);

//! Gets the list of available notch filters for the specified g.HIamp device of the data acquisition session associated with the specified connection handle.
/*!
	\a notchFilters is a pointer to the first element of an array that must be allocated by the caller before. To determine the number of elements of the array to allocate, call \ref GDS_GHIAMP_GetNotchFilters with \a notchFilters set to \c NULL.
	The function then returns the number of elements of the \a notchFilters array in \a notchFiltersCount. Allocate a \ref GDS_FILTER_INFO array of that size and pass it on to another call of \ref GDS_GHIAMP_GetNotchFilters to retrieve the list of available notch filters.

	\param connectionHandle		The connection handle whose associated data acquisition contains the device to get the list of available notch filters for.
	\param deviceName			The name of the device to get the list of available notch filters for.
	\param notchFilters			A pointer to the first element of an allocated array of type \ref GDS_FILTER_INFO and size \a notchFiltersCount, or \c NULL to receive the number of available notch filters in \a notchFiltersCount only.
	\param notchFiltersCount	A pointer to a variable that receives the number of available notch filters for the specified device.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GHIAMP_GetNotchFilters(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_FILTER_INFO *notchFilters, size_t *notchFiltersCount);

//! Gets the scaling values initially set by the manufacturer for the specified g.HIamp device of the data acquisition session associated with the specified connection handle.
/*!
	\param connectionHandle	The connection handle whose associated data acquisition session contains the device to get the factory scaling values for.
	\param deviceName		The name of the device to get the factory scaling values for.
	\param scaling			A pointer to an allocated \ref GDS_GHIAMP_SCALING structure that receives the factory scaling values of the specified device.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GHIAMP_GetFactoryScaling(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GHIAMP_SCALING *factoryScaling);

//! Gets the scaling values for the specified g.HIamp device of the data acquisition session associated with the specified connection handle.
/*!
	\param connectionHandle	The connection handle whose associated data acquisition session contains the device to get the scaling values for.
	\param deviceName		The name of the device to get the scaling values for.
	\param scaling			A pointer to an allocated \ref GDS_GHIAMP_SCALING structure that receives the scaling values of the specified device.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GHIAMP_GetScaling(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GHIAMP_SCALING *scaling);

//! Sets the scaling values for the specified g.HIamp device of the data acquisition session associated with the specified connection handle.
/*!
	\ref GDS_GHIAMP_Calibrate may return NaN values for channels it cannot calculate scaling values for.
	Any NaN values in \a scaling's \ref GDS_GHIAMP_SCALING::Factor array will be replaced by 1 before they're sent to the device.
	Any NaN values in \a scaling's \ref GDS_GHIAMP_SCALING::Offset array will be replaced by 0 before they're sent to the device.

	\param connectionHandle	The connection handle whose associated data acquisition session contains the device to set the scaling values for.
	\param deviceName		The name of the device to set the scaling values for.
	\param scaling			A pointer to an initialized \ref GDS_GHIAMP_SCALING structure that contains the scaling values to set.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GHIAMP_SetScaling(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GHIAMP_SCALING *scaling);

//! Calculates, but does not yet apply new scaling values for each synchronously sampled analog input channel under the current conditions if the calibration set is applied.
/*!
	All channels must be shorted to ground (GND). This is done by applying the calibration equipment, consisting of four connectors, one for each group (A, B, C, and D). They connect all channel inputs of a group with ground (GND). 
	The returned values will not be applied unless they're explicitly set with \ref GDS_GHIAMP_SetScaling.

	Calibration is required if amplitude and/or offset of the known input signal differs from the measured signal.

	The function returns quiet NaN values (\c std::numeric_limits<float>::quiet_NaN()) as scaling factor and offset for channels that failed calibration.

	\param connectionHandle	The connection handle whose associated data acquisition session contains the device to calibrate.
	\param deviceName		The name of the device to calibrate.
	\param scaling			A pointer to an allocated \ref GDS_GHIAMP_SCALING structure that receives the new scaling values calculated under the current conditions for each synchronously sampled analog input channel.

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GHIAMP_Calibrate(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], GDS_GHIAMP_SCALING *scaling);

//! Measures impedance of each available synchronously sampled analog input channel for active or passive electrodes in ohm.
/*!
	Channel 1 plays a special role in impedance measurement and must always be connected. However, its impedance cannot be determined. Channel 1 will always receive NaN as impedance value.
	The method fails if an invalid reference signal is measured on channel 1 (maybe due to a not connected electrode), which does not allow calculation of impedance values of all other channels.

	All returned impedance values are rounded to the nearest integral kilo-ohm value. The method returns quiet NaN values (\c std::numeric_limits<double>::quiet_NaN()) for channel 1 and for not available channels. The availability of a channel can be determined using \ref GDS_GHIAMP_GetAvailableChannels.
	All measured values are clipped within a range from 5,000 ohm to 500,000 ohm. However, if the impedance value cannot be measured exactly, the method returns a value of 4,000 ohm for any estimated impedance value below 5,000 ohm, and 501,000 for any estimated impedance value above 500,000 ohm.

	The following table lists the recommended classification of returned impedances for passive gel electrodes:

	\li \c 4000 for very good impedance (below 5 kilo-ohm)
	\li \c 5000..10000 for good impedance
	\li \c 11000..20000 for medium impedance
	\li \c 21000..500000 for bad impedance
	\li \c 501000 for very bad impedance (above 500 kilo-ohm, maybe due to a not connected electrode)
	\li \c std::numeric_limits<double>::quiet_NaN() for channel 1 and for not available channels.
	
	\param connectionHandle		The connection handle whose associated data acquisition session contains the device to measure impedance for.
	\param deviceName			The name of the device to measure impedance for.
	\param isActiveElectrode	An initialized array where an element at zero-based index <i>i</i> indicates whether the electrode connected to one-based channel number <i>i + 1</i> is active (\c TRUE) or passive (\c FALSE). The caller must pass this array to the method since the device cannot determine if active or passive electrodes are connected.
	\param impedances			An allocated array that receives the impedance values in ohm for each channel. An element at zero-based index <i>i</i> receives the impedance value in ohm of one-based channel number <i>i + 1</i>, or \c std::numeric_limits<double>::quiet_NaN() (see extended description of the method for details).

	\returns A \ref GDS_RESULT structure containing information about the operation's success.
 */
GDSCLIENTAPI_API GDS_RESULT __stdcall GDS_GHIAMP_GetImpedance(GDS_HANDLE connectionHandle, const char (&deviceName)[DEVICE_NAME_LENGTH_MAX], const BOOL (&isActiveElectrode)[GDS_GHIAMP_CHANNELS_MAX], double (&impedances)[GDS_GHIAMP_CHANNELS_MAX]);


#ifdef __cplusplus
}
#endif

#endif //GDSCLIENTAPI_GHIAMP_HPP